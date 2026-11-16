/*
Sample inputs:
Port to listen on: 5014 (or press Enter for default)
*/

#include <iostream>
#include <string>

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

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int port = 5014;
    string line;
    cout << "Enter port to listen on [default 5014]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5014; }
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));
    bind(server_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    listen(server_sock, 1);

    cout << "============================================================\n";
    cout << "      TCP/IP ECHO SERVER LISTENING ON PORT " << port << "\n";
    cout << "============================================================\n";
    cout << "[Server] Waiting for client to connect...\n";

    sockaddr_in client_addr{};
    int addrlen = sizeof(client_addr);
    int conn = accept(server_sock, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
    cout << "[Server] Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n";

    char buf[1024];
    int n;
    while ((n = recv(conn, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[n] = '\0';
        cout << "[Server] Received: '" << buf << "' (" << n << " bytes)\n";
        send(conn, buf, n, 0);
        cout << "[Server] Echoed back to client.\n";
    }

    cout << "[Server] Client disconnected.\n";
    closesocket(conn);
    closesocket(server_sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 10 covers the implementation of a standard TCP/IP Echo Server (RFC 862).

1. Echo Server Responsibilities:
   - Sets up a listening TCP socket on a specified port.
   - Accepts connection requests from echo clients.
   - Reads incoming byte stream buffers and immediately sends them back unmodified.

CODE EXPLANATION:
- 'main()': Binds socket to 0.0.0.0:port, accepts a client, loops over 'recv()' and
  mirrors data back using 'send()'.
================================================================================
*/
