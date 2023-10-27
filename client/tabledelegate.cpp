#include "tabledelegate.h"

TableDelegate::TableDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 2) {
            // Второй столбец (нумерация начинается с 0)
            QSpinBox *spinBox = new QSpinBox(parent);
            spinBox->setMinimum(1); // Устанавливаем минимальное значение
            spinBox->setMaximum(6);
            return spinBox;
        } else if (index.column() == 4) {
            // Четвертый столбец
            QDateEdit *dateEdit = new QDateEdit(parent);
            dateEdit->setDisplayFormat("dd.MM.yyyy"); // Устанавливаем формат даты по вашему выбору
            return dateEdit;
        } else {
            return QItemDelegate::createEditor(parent, option, index); // Возвращаем редактор по умолчанию
        }
}

void TableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // Устанавливаем значение редактора на основе данных из модели
    if (index.column() == 2) {
        // Второй столбец
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            spinBox->setValue(value);
        }
    } else if (index.column() == 4) {
        // Четвертый столбец
        //QDate date = index.model()->data(index, Qt::EditRole).toString();
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            QString dateStr = index.model()->data(index, Qt::EditRole).toString();
            QDate date = QDate::fromString(dateStr, Qt::ISODate);// Преобразование строки в QDate
            dateEdit->setDate(date);
        }
    } else {
        QItemDelegate::setEditorData(editor, index); // Устанавливаем данные редактора по умолчанию
    }
}

void TableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // Сохраняем значение из редактора в модель
    if (index.column() == 2) {
        // Второй столбец
        QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            int value = spinBox->value();
            model->setData(index, value, Qt::EditRole);
        }
    } else if (index.column() == 4) {
        // Четвертый столбец
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            QString date = dateEdit->date().toString(Qt::ISODate);
            model->setData(index, date, Qt::EditRole);
        }
    } else {
        QItemDelegate::setModelData(editor, model, index); // Сохраняем данные модели по умолчанию
    }
}

