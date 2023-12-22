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
