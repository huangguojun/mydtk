/* dtkLogModel.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Mar  5 10:25:48 2012 (+0100)
 * Version: $Id: 09e44fe7c7908afce24d91b9f9250279365e9366 $
 * Last-Updated: Mon Mar  5 12:29:52 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 18
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkLogModel.h"

class dtkLogModelPrivate
{
public:
    QStringList logs;
};

dtkLogModel::dtkLogModel(QObject *parent) : QAbstractListModel(parent), d(new dtkLogModelPrivate) {}

void dtkLogModel::append(const QString &message)
{
    int row = d->logs.count();

    beginInsertRows(QModelIndex(), row, row);

    d->logs.append(message);

    endInsertRows();
}

int dtkLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d->logs.count();
}

QVariant dtkLogModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= d->logs.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return d->logs.at(index.row());

    return QVariant();
}

Qt::ItemFlags dtkLogModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
            | Qt::ItemIsDropEnabled;
}

bool dtkLogModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < d->logs.size()
        && (role == Qt::EditRole || role == Qt::DisplayRole)) {
        d->logs.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool dtkLogModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        d->logs.insert(row, QString());

    endInsertRows();

    return true;
}

bool dtkLogModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        d->logs.removeAt(row);

    endRemoveRows();

    return true;
}

static bool ascendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first < s2.first;
}

static bool decendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first > s2.first;
}

void dtkLogModel::sort(int, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();

    QList<QPair<QString, int>> list;

    for (int i = 0; i < d->logs.count(); ++i)
        list.append(QPair<QString, int>(d->logs.at(i), i));

    if (order == Qt::AscendingOrder)
        qSort(list.begin(), list.end(), ascendingLessThan);
    else
        qSort(list.begin(), list.end(), decendingLessThan);

    d->logs.clear();
    QVector<int> forwarding(list.count());

    for (int i = 0; i < list.count(); ++i) {
        d->logs.append(list.at(i).first);
        forwarding[list.at(i).second] = i;
    }

    QModelIndexList oldList = persistentIndexList();
    QModelIndexList newList;

    for (int i = 0; i < oldList.count(); ++i)
        newList.append(index(forwarding.at(oldList.at(i).row()), 0));

    changePersistentIndexList(oldList, newList);

    emit layoutChanged();
}

Qt::DropActions dtkLogModel::supportedDropActions(void) const
{
    return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}
