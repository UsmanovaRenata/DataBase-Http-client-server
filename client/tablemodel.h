#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QStandardItemModel>
#include <QObject>
#include<QJsonObject>
#include<QJsonArray>
class TableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TableModel();
    void setData(const QJsonObject &students);
    int getId(const QModelIndex index);
    QString headerData(const QModelIndex index);
    void addNewStudent(QJsonObject newStudent);
    void deleteStudent(int studentId);
};


#endif // TABLEMODEL_H
