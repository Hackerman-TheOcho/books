'''session mananger for books program'''
from src import singleton


@singleton
class Session:
    '''session class'''
    def __init__(self):
        self.logged_in = False
        self.username = None

    def login(self, username):
        self.username = username
        self.logged_in = True

    def logout(self):
        self.username = None
        self.logged_in = False

    def is_logged_in(self):
        return self.logged_in