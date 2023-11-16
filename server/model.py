from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Student(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name_student = db.Column(db.String(100), unique=True, nullable=False)
    course = db.Column(db.Integer, nullable=False)
    group = db.Column(db.Integer, nullable=False)
    date = db.Column(db.Date, nullable=False)
    photo = db.Column(db.LargeBinary, nullable=False)
    def serialize(self):
        return {
            'id': self.id,
            'name_student': self.name_student,
            'course': self.course,
            'group': self.group,
            'date': self.date,
            'photo': self.photo
        }