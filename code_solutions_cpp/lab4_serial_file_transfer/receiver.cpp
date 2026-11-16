/*
Sample inputs:
Serial Port: COM2 (Windows) or /dev/ttyUSB1 (Linux)
Output File Path: received.txt (or press Enter for default)
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

void receive_file(const string& port, const string& out_filename) {
    cout << "\n[Receiver] Listening on serial port " << port << " at 9600 baud...\n";
#ifdef _WIN32
    HANDLE hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cout << "[Receiver] Error opening serial port " << port << " (Ensure virtual COM pair or adapter is plugged in).\n";
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

    long size = 0;
    DWORD bytesRead;
    ReadFile(hSerial, &size, sizeof(size), &bytesRead, nullptr);
    if (bytesRead == 0) {
        cout << "[Receiver] Timed out waiting for file size header.\n";
        CloseHandle(hSerial);
        return;
    }
    cout << "[Receiver] Incoming file size: " << size << " bytes.\n";

    ofstream out(out_filename, ios::binary);
    char buf[512];
    long received = 0;
    while (received < size) {
        DWORD to_read = static_cast<DWORD>(min<long>(sizeof(buf), size - received));
        ReadFile(hSerial, buf, to_read, &bytesRead, nullptr);
        if (bytesRead == 0) break;
        out.write(buf, bytesRead);
        received += bytesRead;
        cout << "\r[Receiver] Progress: " << received << "/" << size << " bytes received." << flush;
    }
    CloseHandle(hSerial);
#else
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        cout << "[Receiver] Error opening serial port " << port << "\n";
        return;
    }
    long size = 0;
    read(fd, &size, sizeof(size));
    cout << "[Receiver] Incoming file size: " << size << " bytes.\n";
    
    ofstream out(out_filename, ios::binary);
    char buf[512];
    long received = 0;
    while (received < size) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) break;
        out.write(buf, n);
        received += n;
        cout << "\r[Receiver] Progress: " << received << "/" << size << " bytes received." << flush;
    }
    close(fd);
#endif
    cout << "\n[Receiver] File successfully received and saved to '" << out_filename << "'!\n";
}

int main() {
    string port = "COM2";
    string file = "received.txt";
    string line;

    cout << "Enter receiver serial port (e.g. COM2 or /dev/ttyUSB1) [default COM2]: ";
    if (getline(cin, line) && !line.empty()) port = line;

    cout << "Enter output file path to save [default received.txt]: ";
    if (getline(cin, line) && !line.empty()) file = line;

    receive_file(port, file);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 4 deals with physical layer and data link layer asynchronous serial communication
(RS-232 standard / UART) to transfer binary or text files point-to-point between two PCs.

1. Serial Receiver Workflow:
   - Synchronizes with incoming serial pulses based on baud rate clock.
   - Reads the 8-byte file size header.
   - Continues reading incoming bytes into an output file stream until total received
     bytes match the expected size.

CODE EXPLANATION:
- 'receive_file()': Connects to the designated serial port, unpacks the file size header,
  reads blocks into an 'ofstream' binary buffer, and reports file reception progress.
================================================================================
*/
