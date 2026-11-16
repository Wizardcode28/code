/*
Sample inputs:
Port to listen on: 5002 (or press Enter for default)
Messages: Type any message and press Enter; type '/quit' to exit
*/

#include <iostream>
#include <string>
#include <thread>
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

void receive_loop(int sock) {
    char buf[1024];
    while (true) {
        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            cout << "\n[Client disconnected]\n";
            break;
        }
        buf[n] = '\0';
        if (string(buf) == "/quit") {
            cout << "\n[Client left the chat]\n";
            break;
        }
        cout << "\nClient: " << buf << "\nServer: " << flush;
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int port = 5002;
    string line;
    cout << "Enter port to listen on [default 5002]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5002; }
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
    cout << "      CHAT SERVER ACTIVE ON PORT " << port << "\n";
    cout << "============================================================\n";
    cout << "[Server] Waiting for client to connect...\n";

    sockaddr_in client_addr{};
    int addrlen = sizeof(client_addr);
    int client_sock = accept(server_sock, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
    cout << "[Server] Client connected from " << inet_ntoa(client_addr.sin_addr) << "\n";
    cout << "[Server] Type messages (/quit to exit):\n\n";

    thread t(receive_loop, client_sock);
    t.detach();

    while (true) {
        cout << "Server: ";
        string msg;
        if (!getline(cin, msg)) break;
        if (msg.empty()) continue;

        send(client_sock, msg.c_str(), static_cast<int>(msg.length()), 0);
        if (msg == "/quit") {
            cout << "[Server exited chat]\n";
            break;
        }
    }

    closesocket(client_sock);
    closesocket(server_sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 3 demonstrates a full-duplex TCP chat server written in C++.

1. Multithreading in C++ for Socket I/O:
   - Reading from a socket ('recv') is blocking.
   - Reading from the keyboard ('getline(cin, msg)') is also blocking.
   - Using 'std::thread(receive_loop, client_sock).detach()' isolates the socket reading
     operation in the background so the main thread remains responsive to keyboard input.

CODE EXPLANATION:
- 'receive_loop(sock)': Background thread function continuously reading messages from the client.
- 'main()': Sets up the server socket, accepts the client connection, spawns the receiver thread,
  and loops over 'getline()' to transmit server messages.
================================================================================
*/
