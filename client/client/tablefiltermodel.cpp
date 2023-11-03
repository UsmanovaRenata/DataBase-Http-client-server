#include "tablefiltermodel.h"

TableFilterModel::TableFilterModel(QObject *parent) : QSortFilterProxyModel{parent} {}

void TableFilterModel::setFiltersKey(int colums_count)
{
    for (int i = 0; i < colums_count; i++) {
        filters.insert(i, QString());
    }
}

void TableFilterModel::setFilters(int column, QString filter)
{
    filters[column] = filter;
    invalidateFilter();
}

void TableFilterModel::clearFilters()
{
    filters.clear();
    invalidate();
}

bool TableFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(filters.isEmpty()){
        return true;
    }
    bool ret = false;
    for(QMap<int, QString>::const_iterator iter = filters.constBegin(); iter != filters.constEnd(); iter++){
        QModelIndex index = sourceModel()->index(source_row, iter.key(), source_parent);
        ret = index.data().toString().contains(iter.value());
        if(!ret){
            return ret;
        }
    }
    return ret;
}
