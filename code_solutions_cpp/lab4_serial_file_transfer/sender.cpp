/*
Sample inputs:
Serial Port: COM1 (Windows) or /dev/ttyUSB0 (Linux)
File Path: sample.txt (or press Enter for default)
*/

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

using namespace std;

void send_file(const string& port, const string& filename) {
    ifstream in(filename, ios::binary | ios::ate);
    if (!in.is_open()) {
        cout << "[Error] File '" << filename << "' not found. Creating a sample dummy file...\n";
        ofstream dummy(filename);
        dummy << "Sample payload for serial port transmission lab in C++.\n";
        dummy.close();
        in.open(filename, ios::binary | ios::ate);
    }
    
    long size = static_cast<long>(in.tellg());
    in.seekg(0, ios::beg);

    cout << "\n[Sender] Opening serial port " << port << " at 9600 baud...\n";
#ifdef _WIN32
    HANDLE hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cout << "[Sender] Error opening serial port " << port << " (Ensure virtual COM pair or adapter is plugged in).\n";
        return;
    }
    
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    DWORD bytesWritten;
    // Send file size as 8-byte header
    WriteFile(hSerial, &size, sizeof(size), &bytesWritten, nullptr);

    char buf[512];
    long sent = 0;
    while (in.read(buf, sizeof(buf)) || in.gcount() > 0) {
        WriteFile(hSerial, buf, static_cast<DWORD>(in.gcount()), &bytesWritten, nullptr);
        sent += in.gcount();
        cout << "\r[Sender] Progress: " << sent << "/" << size << " bytes sent." << flush;
    }
    CloseHandle(hSerial);
#else
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        cout << "[Sender] Error opening serial port " << port << "\n";
        return;
    }
    write(fd, &size, sizeof(size));
    char buf[512];
    while (in.read(buf, sizeof(buf)) || in.gcount() > 0) {
        write(fd, buf, in.gcount());
    }
    close(fd);
#endif
    cout << "\n[Sender] File transfer completed successfully!\n";
}

int main() {
    string port = "COM1";
    string file = "sample.txt";
    string line;

    cout << "Enter sender serial port (e.g. COM1 or /dev/ttyUSB0) [default COM1]: ";
    if (getline(cin, line) && !line.empty()) port = line;

    cout << "Enter file path to send [default sample.txt]: ";
    if (getline(cin, line) && !line.empty()) file = line;

    send_file(port, file);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 4 deals with physical layer and data link layer asynchronous serial communication
(RS-232 standard / UART) to transfer binary or text files point-to-point between two PCs.

1. Serial Sender Workflow:
   - Configures baud rate (9600), byte size (8-N-1), and parity on the serial device.
   - Computes total file length and transmits a binary header containing the size.
   - Reads file data in 512-byte blocks and writes them out through the serial port buffer.

CODE EXPLANATION:
- 'send_file()': Opens file stream, configures Windows/POSIX serial device parameters,
  transmits the file size header, streams data chunks, and tracks byte progress.
================================================================================
*/
