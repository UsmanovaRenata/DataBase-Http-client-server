#include "tablewindow.h"
#include "ui_tablewindow.h"
#include<QFileDialog>
TableWindow::TableWindow(Client *client, QWidget *parent) :
    client(client),
    QMainWindow(parent),
    ui(new Ui::TableWindow)
{
    ui->setupUi(this);
    model = new TableModel();
    filterModel = new TableFilterModel();
    connect(client, &Client::getReplyReceived, this, &TableWindow::getAllDataFinished);
    connect(model, &TableModel::itemChanged, this, &TableWindow::changeData);
    ui->tableView->setItemDelegateForColumn(5, new PhotoDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(2, new TableDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(4, new TableDelegate(ui->tableView));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setDefaultSectionSize(150);
    getAllData();
}

TableWindow::~TableWindow()
{
    delete ui;
    delete model;
    delete filterModel;
}

void TableWindow::getAllData()
{
    client->sendGetRequest(client->hostPort + "/all");
    qInfo() << "getting a table";
}

void TableWindow::setFilterLineEdits()
{
    filterModel->setFilterKeyColumn(5);
    for (int i = 0; i < 5; ++i) {
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText(model->headerData(model->index(0,i)));
        lineEdits.append(lineEdit);
        ui->filtersLayout->addWidget(lineEdit);
        connect(lineEdit, &QLineEdit::textChanged, [this, i, lineEdit](const QString &text) {
            filterModel->setFilters(i, text);
        });
    }
    QPushButton *clearFilters = new QPushButton("Очистить фильтры");
    ui->filtersLayout->addWidget(clearFilters);
    connect(clearFilters, &QPushButton::clicked, this, &TableWindow::clearFilters);
}


void TableWindow::getAllDataFinished(const QByteArray &responseData)
{
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = doc.object();
    QJsonValue messageValue = jsonObj.value("message");
    if(messageValue != 200){
        qCritical() << "the table was not received";
    }else{
        qInfo() << "the table is received";
        QJsonObject jsonObj = doc.object();
        model->setData(jsonObj);
        filterModel->setSourceModel(model);
        ui->tableView->setModel(filterModel);
        setFilterLineEdits();
    }
}

void TableWindow::changeData(QStandardItem *item)
{
    qInfo() << "the item has been changed";
    QJsonObject newData;
    newData["id"] = model->getId(item->index());
    newData["column"] = model->headerData(item->index());
    qDebug() << "modified item id: " << model->getId(item->index()) << " column: " << model->headerData(item->index());
    if (newData["column"] == "photo") {
        newData["new_value"] = encodePhoto(model->data(item->index()).toString());
    } else {
        newData["new_value"] = model->data(item->index()).toString();
    }
    connect(client, &Client::postReplyReceived, this, &TableWindow::postFinished);
    client->sendPostRequest(client->hostPort + "/change_data", newData);
}

void TableWindow::on_addStudentButton_clicked()
{
    QJsonObject newStudent;
    newStudent["name_student"] = ui->name->text();
    newStudent["course"] = ui->course->text();
    newStudent["group"] = ui->group->text();
    newStudent["date"] = ui->date->date().toString(Qt::ISODate);
    newStudent["photo"] = encodePhoto(ui->photoPath->text());
    ui->name->clear();
    ui->course->clear();
    ui->group->clear();
    ui->photoPath->clear();
    ui->date->clear();
    connect(client, &Client::postReplyReceived, this, &TableWindow::postDataFinished);
    client->sendPostRequest(client->hostPort + "/add_student", newStudent);
    qInfo() << "a new student has been sent";
}

void TableWindow::postFinished(const QByteArray &responseData)
{
    disconnect(client, &Client::postReplyReceived, this, &TableWindow::postFinished);
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = doc.object();
    QJsonValue messageValue = jsonObj.value("message");
    int message = messageValue.toInt();
    if(message == 200){
        qInfo() << "data changed successfully";
    }else{
        qCritical() << "data modification error";
    }
}

void TableWindow::deleteFinished(const QByteArray &responseData)
{
    disconnect(client, &Client::deleteReplyReceived, this, &TableWindow::deleteFinished);
    postFinished(responseData);
}

void TableWindow::postDataFinished(const QByteArray &responseData)
{
    disconnect(client, &Client::postReplyReceived, this, &TableWindow::postDataFinished);
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = doc.object();
    QJsonValue messageValue = jsonObj.value("message");
    int message = messageValue.toInt();
    if (message == 200) {
        QJsonObject newStudent = jsonObj.value("new_student").toObject();
        model->setData(newStudent);
        qInfo() << "new student successfully added";
    }else{
        qCritical() << "error adding a new student";
    }
}

QString TableWindow::encodePhoto(QString path)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    QPixmap pixmap(path);
    QString formatstr = path.left(path.indexOf('.'));
    QByteArray ba = path.remove(0, formatstr.length() + 1).toLocal8Bit();
    const char* format = ba.data();
    pixmap.save(&buffer, format);
    QString encoded = buffer.data().toBase64();
    return encoded;
}


void TableWindow::clearFilters()
{
    for(QLineEdit *lineEdit : lineEdits){
        lineEdit->clear();
    }
    filterModel->clearFilters();
}


void TableWindow::on_pushButton_clicked()
{
    connect(client, &Client::deleteReplyReceived, this, &TableWindow::deleteFinished);
    client->sendDeleteRequest(client->hostPort + "/delete_student", ui->id->text());
    model->deleteStudent(ui->id->text().toInt());
    ui->id->clear();
    qInfo() << "a student has been sent for deletion";
}


void TableWindow::on_addPhoto_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите фото", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        ui->photoPath->setText(filePath);
    }
}

