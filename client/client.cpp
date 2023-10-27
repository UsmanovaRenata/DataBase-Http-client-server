#include "client.h"
#include <iostream>


Client::Client(QSettings *settings, QObject *parent)
    : settings(settings),
      QObject(parent)
{
    manager = new QNetworkAccessManager;
    hostPort = "http://" + settings->value("host").toString();
    hostPort += ":" + settings->value("port").toString();
    qInfo() << "the client has started work";
}

void Client::sendGetRequest(const QString &url)
{
    qInfo() << "get request is sent";
    auto request = QNetworkRequest(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        this->onGetReply(reply);
        reply->deleteLater();
    });


}

void Client::sendPostRequest(const QString &url, QJsonObject &data)
{
    qInfo() << "post request is sent";
    auto request = QNetworkRequest(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc(data);
    QByteArray byteData = doc.toJson();
    QNetworkReply *reply = manager->post(request,byteData);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        this->onPostReply(reply);
        reply->deleteLater();
    });

}

Client::~Client()
{
    delete manager;
    delete settings;
}

void Client::onGetReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
         qInfo() << "get replay is received";
        emit getReplyReceived(responseData);
    } else {
        qCritical() << "GET Error: " << reply->errorString();
    }

    reply->deleteLater();
}

void Client::onPostReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qInfo() << "post replay is received";
        emit postReplyReceived(responseData);
    } else {
       qCritical() << "POST Error: " << reply->errorString();
    }

    reply->deleteLater();
}
