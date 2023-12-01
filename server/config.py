from configparser import ConfigParser
from flask_login import UserMixin
config = ConfigParser()
config.read('config.ini')

class User():
    def __init__(self, username):
        self.user = username
    def is_authenticated(self):
        return True
    def is_active(self):
        return True
    def is_anonymous(self):
        return False
    def get_id(self):
        return self.user

def database_settings():
    password = config.get('DATABASE', 'password')
    host = config.get('DATABASE', 'host')
    return f"{password}@{host}"

def check_credentials(username, password):
    stored_username = config.get('USER', 'username')
    stored_password = config.get('USER', 'password')
    if username == stored_username and password == stored_password:
        return True
    return False

