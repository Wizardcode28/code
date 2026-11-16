import socket

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_ip = input("Enter server IP: ")

client.connect((server_ip, 5000))

filename = input("Enter file name: ")

client.send(filename.encode())

with open("received_" + filename, "wb") as file:
    while True:
        data = client.recv(1024)

        if not data:
            break

        file.write(data)

print("File received successfully.")

client.close()