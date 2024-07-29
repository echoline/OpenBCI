import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(bytes("save", "utf-8"), ("127.0.0.1", 12346))
