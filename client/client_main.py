#!/usr/bin/env python3
from src.client_socket import create_client_socket
'''client main for books project'''


def main():
    '''main function'''
    server_ip = '127.0.0.1'
    server_port = 8080

    try:
        # create client socket
        sock = create_client_socket(server_ip, server_port)

        # send data
        message = 'Hello, bitch'
        print(f"Sending: {message}")
        sock.sendall(message.encode())

        # recieve response
        response = sock.recv(1024)
        print(f"Recieved: {response.decode()}")
    finally:
        # close socket
        sock.close()
        print("Connection closed.\n")

if __name__ == '__main__':
    main()