
#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QStandardItemModel(parent)
{
    setHorizontalHeaderLabels(QStringList() << "id" << "name" << "course" << "group" << "date" << "photo");

}

void TableModel::setData(const QJsonObject &students)
{
    QJsonArray studentsArray = students["students"].toArray();
    int rowCount = studentsArray.size();
    if(!rowCount){
        addNewStudent(students);
        return;
    }
    for (int row = 0; row < rowCount; ++row) {
        QJsonObject studentObj = studentsArray[row].toObject();
        addNewStudent(studentObj);
    }
}

int TableModel::getId(const QModelIndex &index)
{
    int row = index.row();
    QModelIndex idIndex = this->index(row, 0);
    return data(idIndex).toInt();
}

QString TableModel::headerData(const QModelIndex &index)
{
    int section = index.column();
    switch (section) {
    case 0: return "id";
    case 1: return "name_student";
    case 2: return "course";
    case 3: return "group";
    case 4: return "date";
    case 5: return "photo";
    default:
        return "";
        break;
    }
}

void TableModel::addNewStudent(const QJsonObject &newStudent)
{
    QList<QStandardItem*> items;
    int id = newStudent["id"].toInt();
    QString name = newStudent["name_student"].toString();
    int course = newStudent["course"].toInt();
    int group = newStudent["group"].toInt();
    QString date = newStudent["date"].toString();
    QString photo = newStudent["photo"].toString();
    items << new QStandardItem(QString::number(id)) << new QStandardItem(name)
          << new QStandardItem(QString::number(course)) << new QStandardItem(QString::number(group)) << new QStandardItem(date) << new QStandardItem(photo);
    items[0]->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    appendRow(items);
}

void TableModel::deleteStudent(const int &studentId)
{
    QModelIndex indexToRemove;
    for (int row = 0; row < rowCount(); ++row) {
        QModelIndex idIndex = index(row, 0);
        int id = data(idIndex).toInt();
        if (id == studentId) {
            indexToRemove = index(row, 0);
            break;
        }
    }

    if (indexToRemove.isValid()) {
        removeRow(indexToRemove.row());
    }
}

TableModel::~TableModel()
{
    if(rowCount()){
        removeRows(0,rowCount());
    }
}






