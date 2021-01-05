/* dtkWidgetsTag.h ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 11:51:10 2013 (+0200)
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkWidgetsTagPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTag
{
public:
    dtkWidgetsTag(void);
    dtkWidgetsTag(QString text, int instances);
    dtkWidgetsTag(QString text, int instances, QStringList items);
    dtkWidgetsTag(QString text, int instances, QStringList items, QString color);
    dtkWidgetsTag(const dtkWidgetsTag &other);
    ~dtkWidgetsTag(void);

    int count(void) const;
    QString text(void) const;
    QString color(void) const;
    QStringList items(void) const;

    void setCount(int count);
    void setText(QString text);
    void setColor(QString color);
    void setItems(QStringList items);

protected:
    dtkWidgetsTagPrivate *d;
};
