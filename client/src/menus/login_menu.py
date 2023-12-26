'''login menu for books client'''
import cmd
import getpass
from ..menu_helpers import banner, clear, line_brackets, format_line
from src.session import Session
from src.client_socket import send_request
from .main_menu import MainMenu


class LoginMenu(cmd.Cmd):
    '''login menu class'''
    prompt = ">> "

    def __init__(self, session):
        super().__init__()
        self.session = session

    def preloop(self):
        '''display menu when __init__ is called'''
        self.display_menu()

    def display_menu(self):
        '''print menu to screen'''
        clear()
        print(banner())
        print(format_line)
        print(line_brackets(f"Welcome to the Librarium, please select a choice"))
        print(format_line)
        print(line_brackets("1. Login"))
        print(line_brackets("2. Create Login"))
        print(line_brackets("3. Exit"))
        print(format_line)

    def do_login(self, arg):
        '''log into system'''
        if self.session.is_logged_in():
            print("You are already logged in")
            return
        
        username = input("Username >> ")
        password = getpass.getpass("Password >> ")

        response = send_request("login", username, password)
        if response == "Login successful":
            self.session.login(username)
            print("Welcome to the Librarium")
            MainMenu(self._session).cmdloop()
        else:
            print(response)

    def do_create_login(self, arg):
        username = input("Choose a username: ")
        password = getpass.getpass("Choose a password: ")

        response = send_request("create_account", username, password)
        if response == "Account Creation Successful":
            print(f"Account created for {username}. You can now login.")
        else:
            print(response)

    def do_logout(self, arg):
        '''log out of system'''
        if not self.session.is_logged_in():
            print("You are not logged in")
            return
        send_request("logout", self.session.username, "")
        self.session.logout()
        print("Logged out successfully")

    def do_exit(self, arg):
        '''exit program'''
        send_request("exit", self.session.username, "")
        print("Exiting. God be with ye.")
        return True