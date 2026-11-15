/*
Sample inputs:
Mode: 1 (Server) or 2 (Client)
Messages: any text to echo; /quit to exit
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
    cout << "Enter mode (1: Echo Server, 2: Echo Client) [default 1]: ";
    string line;
    if (getline(cin, line) && !line.empty()) {
        try { mode = stoi(line); } catch (...) { mode = 1; }
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(5014);

    if (mode == 1) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        listen(s, 1);
        cout << "TCP Echo Server listening on 127.0.0.1:5014...\n";

        int cl = sizeof(addr);
        int conn = accept(s, reinterpret_cast<sockaddr*>(&addr), &cl);
        cout << "Client connected!\n";

        char buf[512];
        int n;
        while ((n = recv(conn, buf, sizeof(buf) - 1, 0)) > 0) {
            send(conn, buf, n, 0);
        }
        closesocket(conn);
        closesocket(s);
    } else {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        cout << "Connected to TCP Echo Server! Type message (/quit to exit):\n";

        char buf[512];
        while (true) {
            cout << "You: ";
            string msg;
            if (!getline(cin, msg)) break;
            if (msg == "/quit") break;

            send(s, msg.c_str(), static_cast<int>(msg.length()), 0);
            int n = recv(s, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            cout << "Echo: " << buf << "\n";
        }
        closesocket(s);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
