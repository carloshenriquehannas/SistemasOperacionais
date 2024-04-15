import socket
import struct
from PIL import Image

HOST = '192.168.0.246'  # Endereço IP do servidor
PORT = 8032       # Porta para conexão

def receive_data():
    # Crie um socket TCP/IP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)

    print('Aguardando conexão...')

    # Aguarde a conexão do cliente
    client_socket, address = server_socket.accept()
    print('Conexão estabelecida com', address)

    # Receba os dados
    data = b''

    data = client_socket.recv(4096)

    # Feche a conexão do socket
    client_socket.close()
    server_socket.close()

    return data

data = receive_data()

xres, yres = (int.from_bytes(data[0:4], "little"), int.from_bytes(data[4:8], "little"))

print("X resolution: ", xres)
print("Y resolution: ", yres)
