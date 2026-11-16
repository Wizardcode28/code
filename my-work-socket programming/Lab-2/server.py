import socket

# Create socket
# socket.socket(Address family,Socket type)
server= socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# AF_INET means IPv4 eg. 127.0.0.1, 192.168.1.5
# SOCK_STREAM- means TCP Protocol, reliable communication
# For UDP Protocol, use socket.SOCK_DGRAM

# Bind Socket to IP & Port
server.bind(("127.0.0.1", 5000))

# Wait for client
# Server starts waiting- Client? Client? Client?
server.listen(1)
# this requires backlog as argument 
# maximum no. of pending connection requests that can wait in queue before they are accepted.
# listen(1) means only one pending connection is queued.
# If more arrive before server accepts one, they may be refused or delayed depending on OS
# In practise servers use larger values like listen(10)

print("Server is waiting...")

client, addr= server.accept()
# When client connects, connection is established
# accept() returns (Socket object, Client Address) address is (IP,Port)
# eg. (
# <socket object>,
# ('127.0.0.1',53218)
# )


print("Connected by: ",addr)

# Receive maximum 1024 bytes (maximum buffer size for receive call)
# decode()- converts bytes to string
# encode()- converts string to bytes
msg= client.recv(1024).decode()

print("Client: ",msg)

client.send("Hello Client".encode())

# Close connection
client.close()
server.close()