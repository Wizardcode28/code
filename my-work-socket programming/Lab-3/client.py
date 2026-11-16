import socket

client= socket.socket(socket.AF_INET,socket.SOCK_STREAM)

client.connect(("0.0.0.0",5000))

while True:
    msg= input("You: ")

    client.send(msg.encode())

    if msg.lower()=='bye':
        break

    reply= client.recv(1024).decode()

    print("Server: ",reply)

    if(reply.lower()=="bye"):
        break

client.close()