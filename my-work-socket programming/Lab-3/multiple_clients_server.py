import socket
import threading

def handle_client(client,address):
    print("Connected:", address)

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

    print("Disconnected:", address)


server= socket.socket(socket.AF_INET,socket.SOCK_STREAM)

server.bind(("0.0.0.0", 5000))

server.listen(5)

print("Server started..")
print("Waiting for client...")

while True:
    
    client, address= server.accept()

    thread= threading.Thread(
        target= handle_client,
        args= (client,address)
    )

    thread.start()

    print("Connected to: ",address)