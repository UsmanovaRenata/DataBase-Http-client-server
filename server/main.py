
from config import config, database_settings
from logger import logger, logging_settings
from model import db
from routes import app

if __name__ == "__main__":
    app.secret_key = "key"
    app.config["SQLALCHEMY_DATABASE_URI"] = f"postgresql://postgres:{database_settings()}"
    db.init_app(app)
    with app.app_context():
        db.create_all()
    logging_settings()
    logger.info("starting the server")
    app.run(debug=True)