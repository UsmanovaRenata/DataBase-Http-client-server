
#ifndef TABLEFILTERMODEL_H
#define TABLEFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class TableFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TableFilterModel(QObject *parent = nullptr);
    void setFiltersKey(const int &colums_count);
    void setFilters(const int &column, const QString &filter);
    void clearFilters();
private:
    bool filterAcceptsRow(int &source_row, const QModelIndex &source_parent) const;
    QMap<int, QString> filters;
};

#endif // TABLEFILTERMODEL_H
