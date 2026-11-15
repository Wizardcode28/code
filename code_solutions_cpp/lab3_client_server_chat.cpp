/*
Sample inputs:
Mode: 1 (Server) or 2 (Client)
Messages: any text to send; /quit to exit
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

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int mode = 1;
    cout << "Enter mode (1: Server, 2: Client) [default 1]: ";
    string line;
    if (getline(cin, line) && !line.empty()) {
        try { mode = stoi(line); } catch (...) { mode = 1; }
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(5002);

    int sock = -1;
    if (mode == 1) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        listen(s, 1);
        cout << "Server waiting on 127.0.0.1:5002...\n";
        int cl = sizeof(addr);
        sock = accept(s, reinterpret_cast<sockaddr*>(&addr), &cl);
        cout << "Connected to peer!\n";
        closesocket(s);
    } else {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        cout << "Connected to server!\n";
    }

    char buf[512];
    cout << "Type messages (/quit to exit):\n";
    while (true) {
        if (mode == 2) {
            cout << "You: ";
            string msg;
            if (!getline(cin, msg)) break;
            send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
            if (msg == "/quit") break;

            int n = recv(sock, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            cout << "Peer: " << buf << "\n";
            if (string(buf) == "/quit") break;
        } else {
            int n = recv(sock, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            cout << "Peer: " << buf << "\n";
            if (string(buf) == "/quit") break;

            cout << "You: ";
            string msg;
            if (!getline(cin, msg)) break;
            send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
            if (msg == "/quit") break;
        }
    }

    closesocket(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
