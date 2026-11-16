/*
Sample inputs:
Port to bind: 5001 (or press Enter for default)
Server reply: Acknowledgement from Server (or press Enter for default)
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

void run_server(int port, const string& reply_msg) {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    bind(server_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    listen(server_fd, 1);

    cout << "============================================================\n";
    cout << "   TCP SERVER BOUND & LISTENING ON 127.0.0.1:" << port << "\n";
    cout << "============================================================\n";
    cout << "[Server] Waiting for client to connect...\n";

    sockaddr_in client_addr{};
    int addrlen = sizeof(client_addr);
    int conn = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
    cout << "[Server] Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n";

    char buf[1024] = {0};
    recv(conn, buf, sizeof(buf) - 1, 0);
    cout << "[Server] Message received from client: '" << buf << "'\n";

    send(conn, reply_msg.c_str(), static_cast<int>(reply_msg.length()), 0);
    cout << "[Server] Sent reply: '" << reply_msg << "'\n";

    closesocket(conn);
    closesocket(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    cout << "[Server] Socket closed.\n";
}

int main() {
    int port = 5001;
    string line;
    cout << "Enter port to bind [default 5001]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5001; }
    }

    string reply = "Acknowledgement from Server";
    cout << "Enter server reply [default: Acknowledgement from Server]: ";
    if (getline(cin, line) && !line.empty()) reply = line;

    run_server(port, reply);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 2 demonstrates the server side of the Berkeley Sockets API in C++.

1. Server Socket Flow:
   - socket(AF_INET, SOCK_STREAM, 0): Creates a stream-oriented TCP socket.
   - setsockopt(..., SO_REUSEADDR, ...): Avoids port binding collision during fast restarts.
   - bind(): Maps the local IP address and port to the created socket file descriptor.
   - listen(backlog): Places the server socket into passive state to queue incoming connection requests.
   - accept(): Extracts the first connection request, creates a new connected socket descriptor.
   - recv() / send(): Reads client bytes into a buffer and writes responses back.
   - closesocket() / close(): Deallocates socket descriptors.

CODE EXPLANATION:
- 'run_server()': Initializes socket, binds to 127.0.0.1:port, listens for 1 incoming client,
  accepts the client connection, prints received data, sends confirmation reply, and cleans up.
================================================================================
*/
