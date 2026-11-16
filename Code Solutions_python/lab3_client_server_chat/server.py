# Sample inputs:
# Port: 5002 (or press Enter for default)
# Messages: Type message and press Enter; type '/quit' to exit

import socket
import threading

HOST = "0.0.0.0"

def receive_messages(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                print("\n[Client disconnected]")
                break
            msg = data.decode()
            if msg.strip() == "/quit":
                print("\n[Client left the chat]")
                break
            print(f"\nClient: {msg}\nServer: ", end="", flush=True)
        except (ConnectionResetError, ConnectionAbortedError, OSError):
            break

def run_chat_server(port):
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.bind((HOST, port))
    server_sock.listen(1)
    
    print("=" * 55)
    print(f"      CHAT SERVER ACTIVE ON PORT {port}")
    print("=" * 55)
    print("[Server] Waiting for client to join chat...")
    conn, addr = server_sock.accept()
    print(f"[Server] Client joined from {addr}")
    print("[Server] Type messages (/quit to exit chat):\n")
    
    recv_thread = threading.Thread(target=receive_messages, args=(conn,), daemon=True)
    recv_thread.start()
    
    while True:
        try:
            msg = input("Server: ")
            if not msg:
                continue
            conn.sendall(msg.encode())
            if msg.strip() == "/quit":
                print("[Server exited chat]")
                break
        except (KeyboardInterrupt, EOFError):
            try:
                conn.sendall(b"/quit")
            except OSError:
                pass
            break
            
    conn.close()
    server_sock.close()
    print("[Server] Chat server shut down.")

if __name__ == "__main__":
    p_in = input("Enter port to listen on [default: 5002]: ").strip()
    port = int(p_in) if p_in else 5002
    run_chat_server(port)

"""
================================================================================
LAB EXPLANATION:
Lab 3 demonstrates a full-duplex TCP chat server.

1. Full-Duplex Multithreading:
   - To achieve simultaneous two-way message exchange without blocking, a background thread runs
     'receive_messages()' to continuously listen for incoming packets from the client.
   - The main thread handles terminal keyboard input and transmits text to the client via 'sendall()'.
   - This architecture allows messages to arrive and display at any time, even while the user is typing.

CODE EXPLANATION:
- 'receive_messages(sock)': Background listener thread displaying incoming messages from the client.
- 'run_chat_server(port)': Binds to all available interfaces (0.0.0.0), accepts the incoming client,
  launches the receive thread, and handles the interactive terminal send loop.
================================================================================
"""
