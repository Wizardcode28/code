# Sample inputs:
# Mode: server (run in terminal 1) or client (run in terminal 2)
# Host IP: 127.0.0.1 (or server LAN IP for different PCs)
# Messages: any text message; /quit to exit

import socket, threading, sys

def receive_loop(conn):
    while True:
        try:
            data = conn.recv(1024)
            if not data:
                print("\n[Peer disconnected]")
                break
            print(f"\nPeer: {data.decode()}\nYou: ", end="", flush=True)
        except:
            break

def chat(conn):
    with conn:
        threading.Thread(target=receive_loop, args=(conn,), daemon=True).start()
        print("Chat connected! Type message and press Enter (/quit to exit):")
        while True:
            try:
                msg = input("You: ")
                if msg.strip() == "/quit":
                    break
                conn.sendall(msg.encode())
            except:
                break

def run_server(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((host, port))
        s.listen(1)
        print(f"Waiting for incoming connection on {host}:{port}...")
        conn, addr = s.accept()
        print("Connected with", addr)
        chat(conn)

def run_client(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    print(f"Connected to server {host}:{port}")
    chat(s)

if __name__ == "__main__":
    mode = input("Enter mode (server/client) [default: server]: ").strip().lower() or "server"
    host = input("Enter host IP [default: 127.0.0.1]: ").strip() or "127.0.0.1"
    port_in = input("Enter port [default: 5002]: ").strip()
    port = int(port_in) if port_in else 5002
    if mode == "server":
        run_server(host, port)
    else:
        run_client(host, port)
