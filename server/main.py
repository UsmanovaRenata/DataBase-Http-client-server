import base64
import datetime
import logging
from flask import Flask, request, jsonify
from configparser import ConfigParser
from flask_sqlalchemy import SQLAlchemy

def database_settings():
    config = ConfigParser()
    config.read('config.ini')
    password = config.get('DATABASE', 'password')
    host = config.get('DATABASE', 'host')
    return f"{password}@{host}"

db = SQLAlchemy()
app = Flask(__name__)
logger = logging.getLogger(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = f"postgresql://postgres:{database_settings()}"
db.init_app(app)

class Student(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name_student = db.Column(db.String(100), unique=True, nullable=False)
    course = db.Column(db.Integer)
    group = db.Column(db.Integer)
    date = db.Column(db.Date)
    photo = db.Column(db.LargeBinary)
    def serialize(self):
        return {
            'id': self.id,
            'name_student': self.name_student,
            'course': self.course,
            'group': self.group,
            'date': self.date,
            'photo': self.photo
        }

with app.app_context():
    db.create_all()

def get_next_user_id(config):
    section_count = sum(1 for _ in config.sections() if _.startswith("USER"))
    return section_count + 1

def save_to_config(username, password):
    config = ConfigParser()
    config.read('config.ini')
    userId = get_next_user_id(config)
    config.add_section("USER" + str(userId))
    config.set('USER'+ str(userId), 'username', username)
    config.set('USER'+ str(userId), 'password', password)
    with open('config.ini', 'w') as configfile:
        config.write(configfile)

def check_credentials(username, password):
    config = ConfigParser()
    config.read('config.ini')
    for section in config.sections():
        if section.startswith("USER"):
            if config.has_option(section, 'username') and config.has_option(section, 'password'):
                stored_username = config.get(section, 'username')
                stored_password = config.get(section, 'password')
                if username == stored_username and password == stored_password:
                    return True

    return False

def logging_settings():
    config = ConfigParser()
    config.read('config.ini')
    log_level = config.get('LOGGING', 'level')
    logger.setLevel(log_level)
    log_file = config.get('LOGGING', 'logFile')
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(funcName)s - %(message)s')
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

@app.route('/add_user', methods=['GET', 'POST'])
def add_user():
    newUserData = request.json
    username = newUserData.get('username')
    password = newUserData.get('password')
    if username and password:
        if check_credentials(username, password):
            responseData = {"message": 0}
            logger.warning("the user already exists")
        else:
            save_to_config(username, password)
            responseData = {"message": 1}
            logger.info("authorization was successful")
    else:
        responseData = {"message": 2}
        logger.warning("incorrect data")
    return jsonify(responseData)

@app.route('/login', methods=['GET', 'POST'])
def authorization():
    request_data = request.json
    username = request_data.get('username')
    password = request_data.get('password')
    if username and password:
        if check_credentials(username, password):
            responseData = {"message": 1}
            logger.info("authorization was successful")
        else:
            responseData = {"message": 0}
            logger.warning("the user does not exist")
    else:
            responseData = {"message": 2}
            logger.warning("incorrect data")

    return jsonify(responseData)

@app.route('/all', methods=['GET'])
def all_students():
    students = Student.query.all()
    students_json = []

    for student in students:
        if student.photo:
            photo = base64.b64encode(student.photo).decode()
        else:
            photo = None
        if student.date:
            date = datetime.date.isoformat(student.date)
        else:
            date = None
        student_data = student.serialize()
        student_data['photo'] = photo
        student_data['date'] = date
        students_json.append(student_data)
    if len(students_json) != 0:
        logger.info("all data has been sent")
        return jsonify(students=students_json)
    else:
        logger.critical("error sending data")

@app.route('/change_data', methods=['POST'])
def change_data():
    changed_data = request.json
    data_id = changed_data.get('id')
    column_name = changed_data.get('column')
    new_value = changed_data.get('new_value')
    student = Student.query.get(data_id)
    if student:
        if column_name == 'name_student':
            student.name_student = new_value
        elif column_name == 'course':
            student.course = new_value
        elif column_name == 'group':
            student.group = new_value
        elif column_name == 'date':
            student.date = datetime.date.fromisoformat(new_value)
        elif column_name == 'photo':
            student.photo = base64.b64decode(new_value)
        db.session.commit()

        logger.info("data changed successfully")
        return jsonify({"message": 1})
    else:
        logger.critical("data modification error")
        return jsonify({"message": 0})


@app.route('/add_student', methods=['POST'])
def add_student():
    new_student_data = request.json
    name = new_student_data.get('name_student')
    course = new_student_data.get('course')
    group = new_student_data.get('group')
    date_str = new_student_data.get('date')
    photo = new_student_data.get('photo')
    date = datetime.date.fromisoformat(date_str)
    photo_bytes = base64.b64decode(photo)

    new_student = Student(name_student=name, course=course, group=group, date=date, photo=photo_bytes)
    db.session.add(new_student)
    db.session.commit()
    new_student_info = {
        "id": new_student.id,
        "name_student": new_student.name_student,
        "course": new_student.course,
        "group": new_student.group,
        "date": date_str,
        "photo": photo
    }

    if new_student_info:
        response_data = {
            "message": 1,
            "new_student": new_student_info
        }

        logger.info("new student successfully added")
        return jsonify(response_data)
    else:
        logger.critical("error adding a new student")
        return jsonify({"message": 0})

@app.route('/delete_student', methods=['POST'])
def delete_student():
    delete_student_data = request.json
    id = delete_student_data.get('id')
    student = Student.query.get(id)
    if student:
        db.session.delete(student)
        db.session.commit()
        logger.info("student successfully removed")
        return jsonify({"message": 1})
    logger.critical("student deletion error")
    return jsonify({"message": 0})



if __name__ == "__main__":
    logging_settings()
    logger.info("starting the server")
    app.run(debug=True)