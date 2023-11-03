#include "photodelegate.h"

PhotoDelegate::PhotoDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *PhotoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLabel *editorLabel = new QLabel(parent);
    editorLabel->setGeometry(option.rect);
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите фото", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        editorLabel->setProperty("photo", filePath);
    }
    return editorLabel;
}

void PhotoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLabel *label = qobject_cast<QLabel*>(editor);
    if (label) {
        QString filePath = label->property("photo").toString();
        if (!filePath.isEmpty()) {
            QPixmap pixmap = QPixmap(filePath);
            pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setPixmap(pixmap);
        }
    }
}

void PhotoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, editor->property("photo").toString(), Qt::EditRole);
}

void PhotoDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString photo = index.model()->data(index, Qt::EditRole).toString();

    if (!photo.isEmpty()) {
        if(photo.size() > 1000){
            QByteArray photoByte = QByteArray::fromBase64(photo.toUtf8());
            QPixmap pixmap;
            pixmap.loadFromData(photoByte);
            painter->drawPixmap(option.rect, pixmap);
        }else{
            QPixmap pixmap(photo);
            painter->drawPixmap(option.rect, pixmap);
        }
    } else {
        QPixmap icon("C:\\Users\\Honor\\Downloads\\free-icon-add-photo-7010068.png");
        QRect cellRect = option.rect;
        QSize desiredSize(25, 25);
        QPixmap scaledIcon = icon.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = cellRect.left() + (cellRect.width() - scaledIcon.width()) / 2;
        int y = cellRect.bottom() - scaledIcon.height();
        painter->drawPixmap(x, y, scaledIcon);
    }
}
