/*
Sample inputs:
Server IP: 127.0.0.1 (or press Enter for default)
Server Port: 5002 (or press Enter for default)
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
            cout << "\n[Server disconnected]\n";
            break;
        }
        buf[n] = '\0';
        if (string(buf) == "/quit") {
            cout << "\n[Server closed chat session]\n";
            break;
        }
        cout << "\nServer: " << buf << "\nClient: " << flush;
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    string host = "127.0.0.1";
    int port = 5002;
    string line;

    cout << "Enter server IP [default 127.0.0.1]: ";
    if (getline(cin, line) && !line.empty()) host = line;

    cout << "Enter server port [default 5002]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5002; }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    cout << "============================================================\n";
    cout << "      CONNECTING TO CHAT SERVER AT " << host << ":" << port << "\n";
    cout << "============================================================\n";

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        cout << "[Client Error] Connection refused. Make sure server.cpp is running first!\n";
        closesocket(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    cout << "[Client] Connected to Chat Server successfully!\n";
    cout << "[Client] Type messages (/quit to exit):\n\n";

    thread t(receive_loop, sock);
    t.detach();

    while (true) {
        cout << "Client: ";
        string msg;
        if (!getline(cin, msg)) break;
        if (msg.empty()) continue;

        send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
        if (msg == "/quit") {
            cout << "[Client exited chat]\n";
            break;
        }
    }

    closesocket(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 3 demonstrates the client side of a two-way full-duplex TCP chat in C++.

1. Bidirectional Chat Flow:
   - A client connects to the server and creates a dedicated receiver thread.
   - The receiver thread catches incoming server text and formats it on the screen.
   - The main thread reads console messages and streams them over the socket.
   - Handles '/quit' for clean connection shutdown.

CODE EXPLANATION:
- 'receive_loop()': Runs in a background thread to print messages received from the server.
- 'main()': Connects to host:port, launches the background receiver, and enters the user input loop.
================================================================================
*/
