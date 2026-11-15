/*
Sample inputs:
Mode: 1 (Send) or 2 (Receive)
Serial Port: COM1
File Path: sample.txt
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
        cout << "Error: File '" << filename << "' not found.\n";
        return;
    }
    long size = static_cast<long>(in.tellg());
    in.seekg(0, ios::beg);

#ifdef _WIN32
    HANDLE hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cout << "Error opening serial port " << port << "\n";
        return;
    }
    DWORD bytesWritten;
    WriteFile(hSerial, &size, sizeof(size), &bytesWritten, nullptr);

    char buf[512];
    while (in.read(buf, sizeof(buf)) || in.gcount() > 0) {
        WriteFile(hSerial, buf, static_cast<DWORD>(in.gcount()), &bytesWritten, nullptr);
    }
    CloseHandle(hSerial);
#else
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        cout << "Error opening serial port " << port << "\n";
        return;
    }
    write(fd, &size, sizeof(size));
    char buf[512];
    while (in.read(buf, sizeof(buf)) || in.gcount() > 0) {
        write(fd, buf, in.gcount());
    }
    close(fd);
#endif
    cout << "File " << filename << " (" << size << " bytes) sent over " << port << ".\n";
}

void receive_file(const string& port, const string& out_filename) {
#ifdef _WIN32
    HANDLE hSerial = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        cout << "Error opening serial port " << port << "\n";
        return;
    }
    long size = 0;
    DWORD bytesRead;
    ReadFile(hSerial, &size, sizeof(size), &bytesRead, nullptr);

    ofstream out(out_filename, ios::binary);
    char buf[512];
    long received = 0;
    while (received < size) {
        DWORD to_read = static_cast<DWORD>(min<long>(sizeof(buf), size - received));
        ReadFile(hSerial, buf, to_read, &bytesRead, nullptr);
        if (bytesRead == 0) break;
        out.write(buf, bytesRead);
        received += bytesRead;
    }
    CloseHandle(hSerial);
#else
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        cout << "Error opening serial port " << port << "\n";
        return;
    }
    long size = 0;
    read(fd, &size, sizeof(size));
    ofstream out(out_filename, ios::binary);
    char buf[512];
    long received = 0;
    while (received < size) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) break;
        out.write(buf, n);
        received += n;
    }
    close(fd);
#endif
    cout << "Received file saved to " << out_filename << ".\n";
}

int main() {
    int mode = 1;
    string port = "COM1";
    string file = "sample.txt";

    cout << "Enter mode (1: Send, 2: Receive) [default 1]: ";
    string line;
    if (getline(cin, line) && !line.empty()) {
        try { mode = stoi(line); } catch (...) { mode = 1; }
    }

    cout << "Enter port [default COM1]: ";
    if (getline(cin, line) && !line.empty()) port = line;

    cout << "Enter file path [default sample.txt]: ";
    if (getline(cin, line) && !line.empty()) file = line;

    if (mode == 1) send_file(port, file);
    else receive_file(port, file);
    return 0;
}
