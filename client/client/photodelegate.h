#ifndef PHOTODELEGATE_H
#define PHOTODELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QBuffer>
class PhotoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PhotoDelegate(QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PHOTODELEGATE_H
