#include "loginwindow.h"
#include "ui_loginwindow.h"
#include<QJsonObject>
#include<QMessageBox>
#include<QCryptographicHash>
LoginWindow::LoginWindow(Client *client, QWidget *parent) :
    client(client),
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    table = nullptr;
    ui->usernameEdit->setPlaceholderText("login");
    ui->passwordEdit->setPlaceholderText("password");
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    connect(client, &Client::postReplyReceived, this, &LoginWindow::postFinished);

}

LoginWindow::~LoginWindow()
{
    delete ui;
    if(table != nullptr){
        delete table;
    }
}


void LoginWindow::on_addNewClient_clicked()
{
    this->hide();
    NewClientWindow *newClient = new NewClientWindow(client, this);
    newClient->exec();
    this->show();
    delete newClient;

}


void LoginWindow::postFinished(const QByteArray &responseData)
{
    if (isVisible()) {
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = doc.object();
        QJsonValue messageValue = jsonObj.value("message");
        int message = messageValue.toInt();
        qDebug() << "response code" << message;
        switch (message) {
        case 0:
            QMessageBox::information(this, "Вход не выполнен", "Неверный логин или пароль");
            qWarning() << "authorization failed: the user does not exist";
            break;
        case 1:
            QMessageBox::information(this, "Вход выполнен", "Авторизация прошла успешно");
            qInfo() << "authorization was successful";
            table = new TableWindow(client);
            this->close();
            table->show();
            break;
        default:
            QMessageBox::information(this, "Вход не выполнен", "Введены некорректные данные");
            qWarning() << "authorization failed: incorrect data";
            break;

        }
    }

}

void LoginWindow::sendUser()
{
    QJsonObject user;
    client->settings->setValue("user", ui->usernameEdit->text());
    client->settings->setValue("password", ui->passwordEdit->text());
    user["username"] = ui->usernameEdit->text();
    user["password"] = QString(QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Md5).toHex());
    client->sendPostRequest(client->hostPort + "/login", user);
    qInfo() << "start of authorization";
}


void LoginWindow::on_pushButton_clicked()
{
    sendUser();
}
