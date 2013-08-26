import socket

address = ('127.0.0.1', 369)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

while True:
    msg = raw_input()
    
    if not msg:
        break
    s.sendto(msg, address)

    data = s.recv(1024)
    print data

s.close()
