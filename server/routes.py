from flask import Flask, request, jsonify, abort, make_response
from flask_login import LoginManager, login_user, login_required
from logger import logger
from config import check_credentials, User
from model import db, Student
from jsonschema import validate
import base64
import datetime

app = Flask(__name__)
login_manager = LoginManager(app)

schema = {
    "type": "object",
    "properties": {
        "id": {"type": "number"},
        "column": {
            "type": "string",
            "enum": ["name_student", "course", "group", "date", "photo"]
        },
        "new_value": {
            "type": "string"
        }
    }
}


@login_manager.user_loader
def load_user(user_id):
    return User(user_id)


@app.route('/login', methods=['GET', 'POST'])
def authorization():
    request_data = request.json
    username = request_data.get('username')
    password = request_data.get('password')
    if username and password:
        if check_credentials(username, password):
            user = User(username)
            login_user(user)
            logger.info("authorization was successful")
            return jsonify({"message": 200}), 200
        else:
            logger.warning("the user does not exist")
            return jsonify({"message": 403}), 403
    else:
        logger.warning("incorrect data")
        return jsonify({"message": 404}), 404


@app.route('/all', methods=['GET'])
@login_required
def all_students():
    students = Student.query.all()
    students_json = []
    for student in students:
        photo = base64.b64encode(student.photo).decode()
        date = datetime.date.isoformat(student.date)
        student_data = student.serialize()
        student_data['photo'] = photo
        student_data['date'] = date
        students_json.append(student_data)
    if len(students_json) != 0:
        logger.info("all data has been sent")
        return jsonify(message=200, students=students_json), 200
    else:
        logger.critical("error sending data")
        return jsonify({"message": 500}), 500


@app.route('/change_data', methods=['POST'])
@login_required
def change_data():
    changed_data = request.json
    try:
        validate(changed_data, schema=schema)
        data_id = changed_data.get('id')
        column_name = changed_data.get('column')
        new_value = changed_data.get('new_value')
        student = Student.query.get(data_id)
        if column_name == 'date':
            new_value = datetime.date.fromisoformat(new_value)
        if column_name == 'photo':
            new_value = base64.b64decode(new_value)
        setattr(student, column_name, new_value)
        db.session.commit()
        logger.info("data changed successfully")
        return jsonify({"message": 200}), 200
    except:
        logger.critical("data modification error")
        return jsonify({"message": 500}), 500


@app.route('/add_student', methods=['POST'])
@login_required
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
            "message": 200,
            "new_student": new_student_info
        }

        logger.info("new student successfully added")
        return jsonify(response_data), 200
    else:
        logger.critical("error adding a new student")
        return jsonify({"message": 500}), 500


@app.route('/delete_student/<id>', methods=['DELETE'])
@login_required
def delete_student(id):
    student = Student.query.get(id)
    if student:
        db.session.delete(student)
        db.session.commit()
        logger.info("student successfully removed")
        return jsonify({"message": 200}), 200
    logger.critical("student deletion error")
    return jsonify({"message": 500}), 500
