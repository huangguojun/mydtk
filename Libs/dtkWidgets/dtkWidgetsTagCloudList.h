// Version: $Id: f0bfbda5344a2778cc093b15c0e123b779b6fc5b $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkWidgetsTagCloudItem;
class dtkWidgetsTagCloudListPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudList : public QListWidget
{
    Q_OBJECT

public:
    dtkWidgetsTagCloudList(QWidget *parent = 0);
    ~dtkWidgetsTagCloudList(void);

    void addItem(QString name);
    void addItem(QString name, QString description);
    void addItem(QString name, QString description, QStringList tags);
    void addItem(QString name, QString description, QStringList tags, QString kind, QString type);
    void addItem(dtkWidgetsTagCloudItem item);

    void clear(void);

public:
    void setDark(void);
    void setDoom(void);

signals:
    void itemClicked(const QString &description);

protected slots:
    void onItemClicked(QListWidgetItem *item);

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const;
    QStringList mimeTypes(void) const;

protected:
    dtkWidgetsTagCloudListPrivate *d;

protected:
    friend class dtkWidgetsTagCloudListLightDelegate;
    friend class dtkWidgetsTagCloudListDarkDelegate;
    friend class dtkWidgetsTagCloudListDoomDelegate;
};

//
// dtkWidgetsTagCloudList.h ends here
