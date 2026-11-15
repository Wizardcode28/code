/*
Sample inputs:
Client message: Hello Server
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

    string message = "Hello Server";
    cout << "Enter message to send to server [default: Hello Server]: ";
    string line;
    if (getline(cin, line) && !line.empty()) {
        message = line;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(5001);

    bind(server_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    listen(server_fd, 1);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));

    sockaddr_in client_addr{};
    int addrlen = sizeof(client_addr);
    int conn = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);

    send(client_fd, message.c_str(), static_cast<int>(message.length()), 0);

    char buf[256] = {0};
    recv(conn, buf, sizeof(buf) - 1, 0);
    cout << "[Server] Received from client: " << buf << "\n";

    string reply = "ACK: Connection and binding verified";
    send(conn, reply.c_str(), static_cast<int>(reply.length()), 0);

    memset(buf, 0, sizeof(buf));
    recv(client_fd, buf, sizeof(buf) - 1, 0);
    cout << "[Client] Server reply: " << buf << "\n";

    closesocket(conn);
    closesocket(client_fd);
    closesocket(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
