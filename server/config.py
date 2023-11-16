from configparser import ConfigParser


config = ConfigParser()
config.read('config.ini')

def database_settings():
    password = config.get('DATABASE', 'password')
    host = config.get('DATABASE', 'host')
    return f"{password}@{host}"

def get_next_user_id():
    section_count = sum(1 for _ in config.sections() if _.startswith("USER"))
    return section_count + 1

def save_to_config(username, password):
    userId = get_next_user_id()
    config.add_section("USER" + str(userId))
    config.set('USER'+ str(userId), 'username', username)
    config.set('USER'+ str(userId), 'password', password)
    with open('config.ini', 'w') as configfile:
        config.write(configfile)

def check_credentials(username, password):
    for section in config.sections():
        if section.startswith("USER"):
            if config.has_option(section, 'username') and config.has_option(section, 'password'):
                stored_username = config.get(section, 'username')
                stored_password = config.get(section, 'password')
                if username == stored_username and password == stored_password:
                    return True

    return False

