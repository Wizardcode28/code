# Sample inputs:
# Client message: Hello Server (or press Enter for default)

import socket, threading, time

HOST, PORT = "127.0.0.1", 5001

def server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen(1)
        conn, addr = s.accept()
        with conn:
            data = conn.recv(1024)
            print(f"\n[Server] Received from client {addr}: {data.decode()}")
            conn.sendall(b"Acknowledgement: Connection established and message received.")

def client(msg):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(msg.encode())
        reply = s.recv(1024).decode()
        print(f"[Client] Server replied: {reply}")

if __name__ == "__main__":
    msg = input("Enter message to send from client [default: Hello Server]: ").strip() or "Hello Server"
    t = threading.Thread(target=server, daemon=True)
    t.start()
    time.sleep(0.2)
    client(msg)
    t.join(timeout=1)
