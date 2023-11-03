#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QSpinBox>
#include <QDateEdit>
#include <QPainter>

class TableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TableDelegate(QObject *parent = nullptr) : QItemDelegate{parent} {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // TABLEDELEGATE_H
