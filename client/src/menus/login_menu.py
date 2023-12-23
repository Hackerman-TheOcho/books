'''login menu for books client'''
import cmd
import getpass
from ..menu_helpers import banner, clear, line_brackets, format_line
from src.session import Session
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
        self.do_login_choice("")

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

    def do_login_choice(self, arg):
        choice = input("Enter 1 to login or 2 to create account >> ")
        if choice == '1':
            self.do_login("")
        elif choice == '2':
            self.do_create_login("")
        elif choice == '3':
            print("Exiting. God be with ye.")
            return True
        else:
            print("Invalid selection.")
            self.display_menu()

    def do_login(self, arg):
        '''log into system'''
        if self.session.is_logged_in():
            print("You are already logged in")
            return
        
        username = input("Username >> ")
        password = getpass.getpass("Password >> ")

        self.session.login(username)
        print("Login successful")
        MainMenu(self._session).cmdloop()

    def do_create_login(self, arg):
        username = input("Choose a username: ")
        password = getpass.getpass("Choose a password: ")

        print(f"Account created for {username}. You can now login.")

    # def do_logout(self, arg):
    #     '''log out of system'''
    #     if not self.session.is_logged_in():
    #         print("You are not logged in")
    #         return
    #     self.session.logout()
    #     self.prompt = '(not logged in)>> '
    #     print("Logged out successfully")

    def do_exit(self, arg):
        '''exit program'''
        print("God be with ye")
        return True