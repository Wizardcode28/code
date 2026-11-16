# Sample inputs:
# Server message reply: Acknowledgement from Server (or press Enter for default)
# Port: 5001 (or press Enter for default)

import socket
import time

HOST = "127.0.0.1"

def run_server(port, server_reply):
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    server_sock.bind((HOST, port))
    server_sock.listen(1)
    print("=" * 55)
    print(f"   TCP SERVER BOUND & LISTENING ON {HOST}:{port}")
    print("=" * 55)
    print("[Server] Waiting for client to connect...")
    
    conn, addr = server_sock.accept()
    print(f"[Server] Connection established with client at {addr}")
    
    with conn:
        data = conn.recv(1024)
        print(f"[Server] Message received from client: '{data.decode()}'")
        time.sleep(0.5)
        print(f"[Server] Sending reply: '{server_reply}'")
        conn.sendall(server_reply.encode())
    
    server_sock.close()
    print("[Server] Connection served and socket closed.")

if __name__ == "__main__":
    p_in = input("Enter port to bind [default: 5001]: ").strip()
    port = int(p_in) if p_in else 5001
    s_reply = input("Enter server reply [default: Acknowledgement from Server]: ").strip() or "Acknowledgement from Server"
    
    run_server(port, s_reply)

"""
================================================================================
LAB EXPLANATION:
Lab 2 demonstrates the fundamentals of socket programming and client-server binding
over TCP (Transmission Control Protocol).

1. Server Socket Lifecycle:
   - socket(AF_INET, SOCK_STREAM): Creates an IPv4 TCP streaming socket.
   - setsockopt(SOL_SOCKET, SO_REUSEADDR, 1): Allows immediate reuse of the port without TIME_WAIT locking.
   - bind((HOST, PORT)): Associates the socket endpoint with the local network interface IP and port number.
   - listen(backlog): Transitions the socket into passive listening mode waiting for incoming requests.
   - accept(): Blocks execution until a client initiates a connection, returning a new dedicated connection socket.
   - recv() / sendall(): Reliably receives data from the client and transmits the response back.
   - close(): Shuts down and cleans up resources.

CODE EXPLANATION:
- 'run_server(port, server_reply)': Binds the server socket to '127.0.0.1:port', listens for 1 incoming
  connection, receives the message from the connected client, prints client details, transmits the reply,
  and safely closes the socket connection.
================================================================================
"""
