# Sample inputs:
# Server IP: 127.0.0.1 (or press Enter for default)
# Server Port: 5002 (or press Enter for default)
# Messages: Type message and press Enter; type '/quit' to exit

import socket
import threading

def receive_messages(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                print("\n[Server disconnected]")
                break
            msg = data.decode()
            if msg.strip() == "/quit":
                print("\n[Server closed the chat session]")
                break
            print(f"\nServer: {msg}\nClient: ", end="", flush=True)
        except (ConnectionResetError, ConnectionAbortedError, OSError):
            break

def run_chat_client(host, port):
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("=" * 55)
    print(f"      CONNECTING TO CHAT SERVER AT {host}:{port}")
    print("=" * 55)
    
    try:
        client_sock.connect((host, port))
        print("[Client] Connected to Chat Server successfully!")
        print("[Client] Type messages (/quit to exit chat):\n")
        
        recv_thread = threading.Thread(target=receive_messages, args=(client_sock,), daemon=True)
        recv_thread.start()
        
        while True:
            try:
                msg = input("Client: ")
                if not msg:
                    continue
                client_sock.sendall(msg.encode())
                if msg.strip() == "/quit":
                    print("[Client exited chat]")
                    break
            except (KeyboardInterrupt, EOFError):
                try:
                    client_sock.sendall(b"/quit")
                except OSError:
                    pass
                break
    except ConnectionRefusedError:
        print("[Client Error] Connection refused. Please start server.py first!")
    finally:
        client_sock.close()
        print("[Client] Chat socket closed.")

if __name__ == "__main__":
    h_in = input("Enter server IP [default: 127.0.0.1]: ").strip() or "127.0.0.1"
    p_in = input("Enter server port [default: 5002]: ").strip()
    port = int(p_in) if p_in else 5002
    
    run_chat_client(h_in, port)

"""
================================================================================
LAB EXPLANATION:
Lab 3 demonstrates the client side of the two-way TCP chat application.

1. Client Chat Operation:
   - Connects to the chat server's IP and port.
   - Spawns a background worker thread executing 'receive_messages()' to continuously monitor
     the socket for incoming server text.
   - The primary thread accepts user input from 'sys.stdin' and streams it to the server.
   - Graceful termination is handled through the '/quit' command, notifying both sides cleanly.

CODE EXPLANATION:
- 'receive_messages(sock)': Runs as a daemon thread, receiving incoming server payloads and formatting output.
- 'run_chat_client(host, port)': Manages connection setup, multithreading initialization, and the user message loop.
================================================================================
"""
