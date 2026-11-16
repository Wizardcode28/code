# Sample inputs:
# Server Host IP: 127.0.0.1 (or press Enter for default)
# Server Port: 5001 (or press Enter for default)
# Client message: Hello Server from Client (or press Enter for default)

import socket
import time

def run_client(host, port, client_msg):
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("=" * 55)
    print(f"   CONNECTING CLIENT TO {host}:{port}")
    print("=" * 55)
    
    try:
        client_sock.connect((host, port))
        print("[Client] Connected to server successfully!")
        
        time.sleep(0.3)
        print(f"[Client] Sending message: '{client_msg}'")
        client_sock.sendall(client_msg.encode())
        
        reply = client_sock.recv(1024).decode()
        print(f"[Client] Received reply from server: '{reply}'")
    except ConnectionRefusedError:
        print("[Client Error] Connection refused. Make sure server.py is running first!")
    finally:
        client_sock.close()
        print("[Client] Connection closed.")

if __name__ == "__main__":
    h_in = input("Enter server IP [default: 127.0.0.1]: ").strip() or "127.0.0.1"
    p_in = input("Enter server port [default: 5001]: ").strip()
    port = int(p_in) if p_in else 5001
    c_msg = input("Enter message to send [default: Hello Server from Client]: ").strip() or "Hello Server from Client"
    
    run_client(h_in, port, c_msg)

"""
================================================================================
LAB EXPLANATION:
Lab 2 demonstrates the client side of the TCP client-server architecture.

1. Client Socket Lifecycle:
   - socket(AF_INET, SOCK_STREAM): Creates an active IPv4 TCP socket.
   - connect((HOST, PORT)): Establishes connection to the remote server via the 3-Way Handshake (SYN -> SYN-ACK -> ACK).
   - sendall(data): Sends data across the reliable virtual circuit.
   - recv(bufsize): Blocks until response data is received from the server.
   - close(): Gracefully closes the connection via 4-Way TCP teardown (FIN -> ACK -> FIN -> ACK).

CODE EXPLANATION:
- 'run_client(host, port, client_msg)': Connects to the designated server endpoint, transmits the user's
  message, awaits the server's reply, prints the response, and handles connection errors cleanly.
================================================================================
"""
