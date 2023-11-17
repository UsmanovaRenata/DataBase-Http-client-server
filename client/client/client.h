#ifndef CLIENT_H
#define CLIENT_H
#include <QtNetwork>
#include<QtCore>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSettings *settings, QObject *parent = nullptr);
    void sendGetRequest(const QString &url);
    void sendPostRequest(const QString &url, const QJsonObject &data);
    QSettings *settings;
    QString hostPort;
    ~Client();
private:
    QNetworkAccessManager *manager;
signals:
    void getReplyReceived(const QByteArray &responseData);
    void postReplyReceived(const QByteArray &responseData);
private slots:
    void onGetReply(QNetworkReply *reply);
    void onPostReply(QNetworkReply *reply);
};

#endif // CLIENT_H
