#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include"tablemodel.h"
#include"client.h"
#include"photodelegate.h"
#include "tabledelegate.h"
#include "tablefiltermodel.h"
//#include "tablefiltermodel.h"
namespace Ui {
class TableWindow;
}

class TableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWindow(Client *client, QWidget *parent = nullptr);
    ~TableWindow();
private:
    Ui::TableWindow *ui;
    Client *client;
    TableModel *model;
    TableFilterModel *filterModel;
    QList<QLineEdit*> lineEdits;
    void getAllData();
    void setFilterLineEdits();

private slots:
    void getAllDataFinished(QByteArray responseData);
    void changeData(QStandardItem *item);
    void on_addStudentButton_clicked();
    void postFinished(QByteArray responseData);
    void postDataFinished(QByteArray responseData);
    QString encodePhoto(QString path);
    void clearFilters();
    void on_pushButton_clicked();
    void on_addPhoto_clicked();
};
#endif // TABLEWINDOW_H
