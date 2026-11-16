# Sample inputs:
# Port: 5014 (or press Enter for default)

import socket

HOST = "0.0.0.0"

def run_echo_server(port):
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    server_sock.bind((HOST, port))
    server_sock.listen(1)
    print("=" * 55)
    print(f"      TCP/IP ECHO SERVER LISTENING ON PORT {port}")
    print("=" * 55)
    print("[Server] Waiting for client to connect...")
    
    conn, addr = server_sock.accept()
    print(f"[Server] Client connected from IP: {addr[0]}, Port: {addr[1]}")
    
    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                print(f"[Server] Client {addr} disconnected.")
                break
            message = data.decode(errors="replace")
            print(f"[Server] Received: '{message}' ({len(data)} bytes)")
            
            # Echo exact payload back to client
            conn.sendall(data)
            print("[Server] Echoed back to client.")
            
    server_sock.close()
    print("[Server] Echo server closed.")

if __name__ == "__main__":
    p_in = input("Enter port to listen on [default: 5014]: ").strip()
    port = int(p_in) if p_in else 5014
    run_echo_server(port)

"""
================================================================================
LAB EXPLANATION:
Lab 10 covers the implementation of a standard TCP/IP Echo Server (RFC 862).

1. Echo Server Responsibilities:
   - Sets up a persistent listening socket on a designated port.
   - Accepts incoming TCP connections from clients.
   - Continuously receives byte streams from the client and transmits the exact identical
     payload back over the same socket connection without modifying any bytes.
   - Used extensively for network latency testing, throughput measurement, and circuit verification.

CODE EXPLANATION:
- 'run_echo_server(port)': Binds to '0.0.0.0:port', accepts a client connection, reads incoming chunks
  with 'conn.recv(1024)', immediately calls 'conn.sendall(data)' to echo them back, and terminates when
  the client disconnects.
================================================================================
"""
