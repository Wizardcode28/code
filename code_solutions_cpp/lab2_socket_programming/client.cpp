/*
Sample inputs:
Server IP: 127.0.0.1 (or press Enter for default)
Server Port: 5001 (or press Enter for default)
Client message: Hello Server from Client (or press Enter for default)
*/

#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

using namespace std;

void run_client(const string& host, int port, const string& msg) {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
    serv_addr.sin_port = htons(port);

    cout << "============================================================\n";
    cout << "   CONNECTING CLIENT TO " << host << ":" << port << "\n";
    cout << "============================================================\n";

    if (connect(client_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        cout << "[Client Error] Connection failed! Make sure server.cpp is running first.\n";
        closesocket(client_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    cout << "[Client] Connected to server successfully!\n";
    cout << "[Client] Sending message: '" << msg << "'\n";
    send(client_fd, msg.c_str(), static_cast<int>(msg.length()), 0);

    char buf[1024] = {0};
    int bytes = recv(client_fd, buf, sizeof(buf) - 1, 0);
    if (bytes > 0) {
        buf[bytes] = '\0';
        cout << "[Client] Received server reply: '" << buf << "'\n";
    }

    closesocket(client_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    cout << "[Client] Connection closed.\n";
}

int main() {
    string host = "127.0.0.1";
    int port = 5001;
    string msg = "Hello Server from Client";
    string line;

    cout << "Enter server IP [default 127.0.0.1]: ";
    if (getline(cin, line) && !line.empty()) host = line;

    cout << "Enter server port [default 5001]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5001; }
    }

    cout << "Enter message to send [default: Hello Server from Client]: ";
    if (getline(cin, line) && !line.empty()) msg = line;

    run_client(host, port, msg);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 2 demonstrates the client side of socket programming in C++.

1. Client Socket Concepts:
   - Client initiates connection to a remote server listening at an IP:Port tuple.
   - connect(): Triggers the TCP three-way handshake (SYN, SYN-ACK, ACK).
   - send() / recv(): Communicates stream data with full TCP reliability (checksum, ACK, flow control).
   - closesocket(): Tears down the TCP connection gracefully.

CODE EXPLANATION:
- 'run_client()': Creates socket, connects to host:port, sends the input message string,
  receives and displays the server's reply buffer, and safely closes the socket.
================================================================================
*/
