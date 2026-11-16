import socket

server= socket.socket(socket.AF_INET,socket.SOCK_STREAM)

server.bind(("0.0.0.0", 5000))

server.listen()
print("Waiting for client...")

client, addr= server.accept()

print("Connected to: ",addr)

while True:
    msg= client.recv(1024).decode()

    print("Client: ",msg)

    if msg.lower()=='bye':
        print("Client ended the chat")
        break

    reply= input("You: ")
    client.send(reply.encode())

    if(reply.lower()=="bye"):
        print("Server ended the chat")
        break

server.close()
client.close()