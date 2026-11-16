/*
Sample inputs:
Server IP: 127.0.0.1 (or press Enter for default)
Server Port: 5014 (or press Enter for default)
Messages to echo: Type any text message; '/quit' to exit
*/

#include <iostream>
#include <string>
#include <chrono>

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

    string host = "127.0.0.1";
    int port = 5014;
    string line;

    cout << "Enter server IP [default 127.0.0.1]: ";
    if (getline(cin, line) && !line.empty()) host = line;

    cout << "Enter server port [default 5014]: ";
    if (getline(cin, line) && !line.empty()) {
        try { port = stoi(line); } catch (...) { port = 5014; }
    }

    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    cout << "============================================================\n";
    cout << "      TCP/IP ECHO CLIENT CONNECTING TO " << host << ":" << port << "\n";
    cout << "============================================================\n";

    if (connect(client_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        cout << "[Client Error] Connection failed! Make sure server.cpp is running first.\n";
        closesocket(client_sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    cout << "[Client] Connected to Echo Server successfully!\n";
    cout << "Type messages to send (/quit to disconnect):\n\n";

    char buf[1024];
    while (true) {
        cout << "You: ";
        string msg;
        if (!getline(cin, msg)) break;
        if (msg.empty()) continue;
        if (msg == "/quit") {
            cout << "[Client] Disconnecting...\n";
            break;
        }

        auto start = chrono::high_resolution_clock::now();
        send(client_sock, msg.c_str(), static_cast<int>(msg.length()), 0);

        int n = recv(client_sock, buf, sizeof(buf) - 1, 0);
        auto end = chrono::high_resolution_clock::now();

        if (n <= 0) {
            cout << "[Client] Server closed connection.\n";
            break;
        }
        buf[n] = '\0';
        chrono::duration<double, milli> rtt = end - start;
        cout << "Server Echo: '" << buf << "' (Round-Trip Time: " << rtt.count() << " ms)\n\n";
    }

    closesocket(client_sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 10 covers the client side of the TCP/IP Echo Protocol.

1. Echo Client Operation & Round-Trip Time (RTT):
   - Transmits message to server and awaits identical mirrored byte stream.
   - Calculates Round-Trip Time (RTT):
     RTT = t_received - t_transmitted
   - Verifies network throughput, latency, and packet integrity.

CODE EXPLANATION:
- 'main()': Connects to host:port, prompts user messages in a loop, records high-resolution
  timestamps before send and after recv, displays echoed string, and calculates RTT latency in milliseconds.
================================================================================
*/
