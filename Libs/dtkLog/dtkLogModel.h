/* dtkLogModel.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Mar  5 10:13:44 2012 (+0100)
 * Version: $Id: d4f9f8bbe3a8400ccd40a3b8b568d17321e931ff $
 * Last-Updated: Mon Mar  5 11:14:39 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 27
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKLOGMODEL_H
#define DTKLOGMODEL_H

#include <dtkLogExport.h>

#include <QtCore>

class dtkLogModelPrivate;

class DTKLOG_EXPORT dtkLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    dtkLogModel(QObject *parent = 0);

public:
    void append(const QString &message);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

public:
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:
    Qt::ItemFlags flags(const QModelIndex &index) const;

public:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public:
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
    Qt::DropActions supportedDropActions(void) const;

private:
    dtkLogModelPrivate *d;
};

#endif
