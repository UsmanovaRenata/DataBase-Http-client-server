#include "tabledelegate.h"
#include <QSpinBox>
#include <QDateEdit>
#include <QPainter>
QWidget *TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2) {
            QSpinBox *spinBox = new QSpinBox(parent);
            spinBox->setMinimum(1);
            spinBox->setMaximum(6);
            return spinBox;
        } else if (index.column() == 4) {
            QDateEdit *dateEdit = new QDateEdit(parent);
            dateEdit->setDisplayFormat("dd.MM.yyyy");
            return dateEdit;
        } else {
            return QItemDelegate::createEditor(parent, option, index);
        }
}

void TableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 2) {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            spinBox->setValue(value);
        }
    } else if (index.column() == 4) {
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            QString dateStr = index.model()->data(index, Qt::EditRole).toString();
            QDate date = QDate::fromString(dateStr, Qt::ISODate);
            dateEdit->setDate(date);
        }
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void TableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 2) {
        QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            int value = spinBox->value();
            model->setData(index, value, Qt::EditRole);
        }
    } else if (index.column() == 4) {
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            QString date = dateEdit->date().toString(Qt::ISODate);
            model->setData(index, date, Qt::EditRole);
        }
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

