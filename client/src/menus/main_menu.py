'''main menu for books client'''
import cmd
from ..menu_helpers import banner, clear, line_brackets, format_line


class MainMenu(cmd.Cmd):
    '''main menu class'''
    def __init__(self, session):
        super().__init__()
        self._session = session
        self.prompt = f'({self.session.username})>> '

    def preloop(self):
        '''display menu when __init__ is called'''
        self.display_menu()

    def display_menu(self):
        '''print menu to screen'''
        clear()
        print(banner())
        print(format_line)
        print(line_brackets(f"Welcome to the Librarium, {self.session.username}"))
        print(format_line)
        print(line_brackets("1. View Inventory"))
        print(line_brackets("2. Return a book"))
        print(line_brackets("3. Logout"))
        print(format_line)

    def do_view_inventory(self, arg):
        '''view books in inventory'''
        print("Inventory...")

    def do_checkout(self, arg):
        '''checkout a book'''
        print("Checkout out a book...")

    def do_return(self, arg):
        '''return a book'''
        print("Returning book...")

    def do_logout(self, arg):
        '''logout and return to login screen'''
        self.session.logout()
        return True
