import logging
from config import config

logger = logging.getLogger(__name__)

def logging_settings():
    log_level = config.get('LOGGING', 'level')
    logger.setLevel(log_level)
    log_file = config.get('LOGGING', 'logFile')
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(funcName)s - %(message)s')
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)