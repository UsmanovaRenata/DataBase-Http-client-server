from flask import Flask, request, jsonify
from logger import logger
from config import check_credentials, save_to_config
from model import db, Student
import base64
import datetime

app = Flask(__name__)

@app.route('/login', methods=['GET', 'POST'])
def authorization(newUser=False):
    request_data = request.json
    username = request_data.get('username')
    password = request_data.get('password')
    if username and password:
        if check_credentials(username, password):
            if(newUser):
                responseData = {"message": 403}
                logger.info("the user already exists")
            else:
                responseData = {"message": 200}
                logger.info("authorization was successful")
        else:
            if(newUser):
                save_to_config(username, password)
                responseData = {"message": 200}
                logger.info("user added successfully")
            else:
                responseData = {"message": 403}
                logger.warning("the user does not exist")
    else:
        responseData = {"message": 400}
        logger.warning("incorrect data")

    return jsonify(responseData)

@app.route('/add_user', methods=['GET', 'POST'])
def add_user():
    return authorization(True)

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
        return jsonify(message=200, students=students_json)
    else:
        logger.critical("error sending data")
        return jsonify({"message": 500})

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
        return jsonify({"message": 200})
    else:
        logger.critical("data modification error")
        return jsonify({"message": 500})


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
            "message": 200,
            "new_student": new_student_info
        }

        logger.info("new student successfully added")
        return jsonify(response_data)
    else:
        logger.critical("error adding a new student")
        return jsonify({"message": 500})

@app.route('/delete_student', methods=['POST'])
def delete_student():
    delete_student_data = request.json
    id = delete_student_data.get('id')
    student = Student.query.get(id)
    if student:
        db.session.delete(student)
        db.session.commit()
        logger.info("student successfully removed")
        return jsonify({"message": 200})
    logger.critical("student deletion error")
    return jsonify({"message": 500})

