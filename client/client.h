#ifndef CLIENT_H
#define CLIENT_H
#include <QtNetwork>
#include<QtCore>
#include<QLoggingCategory>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSettings *settings, QObject *parent = nullptr);
    void sendGetRequest(const QString &url);
    void sendPostRequest(const QString &url, QJsonObject &data);
    QSettings *settings;
    QString hostPort;
private:
    QNetworkAccessManager *manager;
signals:
    void getReplyReceived(QByteArray responseData);
    void postReplyReceived(QByteArray responseData);
private slots:
    void onGetReply(QNetworkReply *reply);
    void onPostReply(QNetworkReply *reply);
};

#endif // CLIENT_H
