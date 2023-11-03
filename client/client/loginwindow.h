#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include"newclientwindow.h"
#include <QDialog>
#include"tablewindow.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(Client *client, QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_addNewClient_clicked();
    void postFinished(QByteArray responseData);
    void on_pushButton_clicked();
private:
    void sendUser();
    Ui::LoginWindow *ui;
    Client *client;
    TableWindow *table;
};

#endif // LOGINWINDOW_H
