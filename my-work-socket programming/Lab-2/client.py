import socket

client= socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# We don't need to give client IP & Port
# Operating System assign them automatically
# These are called ephemeral (temporary) ports.
# eg. Server IP : 127.0.0.1 Port : 5000, Client automatically gets something like IP : 127.0.0.1 Port : 53218

# Can we bind client also client.bind(("127.0.0.1",6000))
client.connect(("127.0.0.1",5000))

client.send("Hello Server".encode())

msg= client.recv(1024).decode()

print("Server: ",msg)

client.close()