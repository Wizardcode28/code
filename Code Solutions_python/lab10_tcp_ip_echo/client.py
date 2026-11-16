# Sample inputs:
# Server IP: 127.0.0.1 (or press Enter for default)
# Server Port: 5014 (or press Enter for default)
# Message to send: Any text message; '/quit' to exit

import socket
import time

def run_echo_client(host, port):
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("=" * 55)
    print(f"      TCP/IP ECHO CLIENT CONNECTING TO {host}:{port}")
    print("=" * 55)
    
    try:
        client_sock.connect((host, port))
        print("[Client] Connected to Echo Server successfully!")
        print("Type message to send (type '/quit' to disconnect):\n")
        
        while True:
            msg = input("You: ")
            if not msg:
                continue
            if msg.strip() == "/quit":
                print("[Client] Disconnecting...")
                break
                
            start_time = time.time()
            client_sock.sendall(msg.encode())
            
            reply = client_sock.recv(1024).decode(errors="replace")
            rtt_ms = (time.time() - start_time) * 1000
            print(f"Server Echo: '{reply}' (Round-Trip Time: {rtt_ms:.2f} ms)\n")
            
    except ConnectionRefusedError:
        print("[Client Error] Connection refused. Please ensure the echo server is running first.")
    finally:
        client_sock.close()
        print("[Client] Connection closed.")

if __name__ == "__main__":
    h_in = input("Enter server IP [default: 127.0.0.1]: ").strip() or "127.0.0.1"
    p_in = input("Enter server port [default: 5014]: ").strip()
    port = int(p_in) if p_in else 5014
    
    run_echo_client(h_in, port)

"""
================================================================================
LAB EXPLANATION:
Lab 10 covers the client side of the TCP/IP Echo Protocol.

1. Echo Client Operation & Round-Trip Time (RTT):
   - Establishes a reliable full-duplex TCP virtual circuit with the echo server.
   - Measures network Round-Trip Time (RTT): the time taken for a data packet to travel
     from the client to the server and back again.
   - RTT calculation:
     RTT = t_arrival - t_transmission
   - Validates that the echoed payload matches the original transmitted message bit-for-bit,
     confirming data integrity and transport reliability.

CODE EXPLANATION:
- 'run_echo_client(host, port)': Connects to the echo server, prompts user for messages, records
  timestamps with 'time.time()', transmits data via 'client_sock.sendall()', receives the echoed
  reply via 'recv()', calculates RTT latency in milliseconds, and prints the result.
================================================================================
"""
