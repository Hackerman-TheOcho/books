import socket


def create_client_socket(server_ip, server_port):
    '''create TCP/IP socket and connect to server'''
    # create
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # connect
    server_address = (server_ip, server_port)
    print(f"Connecting to {server_ip} port {server_port}")
    sock.connect(server_address)

    return sock

def send_request(request_type, username, password):
    '''send requests to server'''
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('127.0.0.1', 8080))

        # send request
        message = f"{request_type}:{username}:{password}"
        s.sendall(message.encode())

        # receive response
        response = s.recv(1024).decode()
        return response