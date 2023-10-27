#ifndef NEWCLIENTWINDOW_H
#define NEWCLIENTWINDOW_H
#include "client.h"
#include <QDialog>
#include<QJsonObject>
#include<QMessageBox>
#include<QCryptographicHash>
namespace Ui {
class NewClientWindow;
}

class NewClientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewClientWindow(Client *client, QWidget *parent = nullptr);
    ~NewClientWindow();

private slots:
    void postFinished(QByteArray responseData);
    void on_pushButton_clicked();

private:
    void sendNewUser();
    Client *client;
    Ui::NewClientWindow *ui;
};

#endif // NEWCLIENTWINDOW_H
