import socket

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind(("0.0.0.0", 5000))

server.listen(1)

print("Waiting for client...")

client, address = server.accept()

print("Connected:", address)

filename = client.recv(1024).decode()

print("File requested:", filename)

with open(filename, "rb") as file:
    while True:
        data = file.read(1024)

        if not data:
            break

        client.sendall(data)

print("File sent successfully.")

client.close()
server.close()