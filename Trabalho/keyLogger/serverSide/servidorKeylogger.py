import socket

SERVER_IP = '192.168.0.246'
SERVER_PORT = 8011

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((SERVER_IP, SERVER_PORT))
    server_socket.listen(1)
    print(f"Servidor ouvindo em {SERVER_IP}:{SERVER_PORT}")

    while True:
        client_socket, _ = server_socket.accept()
        
        while True:
            # Recebe a mensagem do cliente
            message = client_socket.recv(1024).decode()
            print(message)

if __name__ == '__main__':
    start_server()
