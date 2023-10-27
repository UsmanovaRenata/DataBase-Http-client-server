#include "newclientwindow.h"
#include "ui_newclientwindow.h"

NewClientWindow::NewClientWindow(Client *client, QWidget *parent) :
    client(client),
    QDialog(parent),
    ui(new Ui::NewClientWindow)
{
    ui->setupUi(this);
    ui->usernameEdit->setPlaceholderText("login");
    ui->passwordEdit->setPlaceholderText("password");
    connect(client, &Client::postReplyReceived, this, &NewClientWindow::postFinished);
}

void NewClientWindow::sendNewUser()
{
    QJsonObject newUser;
    client->settings->setValue("user", ui->usernameEdit->text());
    client->settings->setValue("password", ui->passwordEdit->text());
    newUser["username"] = ui->usernameEdit->text();
    newUser["password"] = QString(QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Md5).toHex());
    client->sendPostRequest(client->hostPort + "/add_user", newUser);
    qInfo() << "start adding a new user";

}

NewClientWindow::~NewClientWindow()
{
    delete ui;
}


void NewClientWindow::postFinished(QByteArray responseData)
{
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = doc.object();
    QJsonValue messageValue = jsonObj.value("message");
    int message = messageValue.toInt();
    qDebug() << "response code" << message;
    switch (message) {
    case 0:
        QMessageBox::information(this, "Ошибка", "Такой пользователь уже существует");
        qWarning() << "authorization failed: the user already exists";
        break;
    case 1:
        QMessageBox::information(this, "Успех!", "Пользователь успешно добавлен");
        qInfo() << "authorization was successful";
        break;
    default:
        QMessageBox::information(this, "Ошибка", "Введены некорректные данные");
        qWarning() << "authorization failed: incorrect data";
        break;
    }
}

void NewClientWindow::on_pushButton_clicked()
{
    sendNewUser();
}

