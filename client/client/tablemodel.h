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
    explicit TableModel(QObject *parent = nullptr);
    void setData(const QJsonObject &students);
    int getId(const QModelIndex &index);
    QString headerData(const QModelIndex &index);
    void addNewStudent(const QJsonObject &newStudent);
    void deleteStudent(const int &studentId);
    ~TableModel();
};


#endif // TABLEMODEL_H
