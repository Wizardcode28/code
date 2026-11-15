# Sample inputs:
# Mode: server or client
# Host: 127.0.0.1
# Port: 5014
# Message: any text to echo; /quit to exit

import socket, sys

def server(host="127.0.0.1", port=5014):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((host, port))
        s.listen(1)
        print(f"Echo server listening on {host}:{port}")
        conn, addr = s.accept()
        with conn:
            print("Connected by", addr)
            while data := conn.recv(1024):
                conn.sendall(data)

def client(host="127.0.0.1", port=5014):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print(f"Connected to server {host}:{port}. Type message to echo (/quit to exit):")
        while True:
            msg = input("You: ")
            if msg == "/quit":
                break
            s.sendall(msg.encode())
            print("Echo:", s.recv(1024).decode())

if __name__ == "__main__":
    mode = input("Enter mode (server/client) [default: server]: ").strip().lower() or "server"
    host = input("Enter host IP [default: 127.0.0.1]: ").strip() or "127.0.0.1"
    port_in = input("Enter port [default: 5014]: ").strip()
    port = int(port_in) if port_in else 5014
    server(host, port) if mode == "server" else client(host, port)
