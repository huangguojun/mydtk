// Version: $Id: 416c3881f3ee861a0529ccb6ed31c568b46f9331 $
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

class dtkWidgetsTagCloudItemPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudItem : public QListWidgetItem
{
public:
     dtkWidgetsTagCloudItem(QString name);
     dtkWidgetsTagCloudItem(QString name, QString description);
     dtkWidgetsTagCloudItem(QString name, QString description, QStringList tags);
     dtkWidgetsTagCloudItem(QString name, QString description, QStringList tags, QString kind, QString type);
     dtkWidgetsTagCloudItem(const dtkWidgetsTagCloudItem& item);
    ~dtkWidgetsTagCloudItem(void);

public:
    QString name(void) const;
    QString description(void) const;
    QStringList tags(void) const;
    QString kind(void) const;
    QString type(void) const;

protected:
    dtkWidgetsTagCloudItemPrivate *d;
};

//
// dtkWidgetsTagCloudItem.h ends here
