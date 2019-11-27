/* dtkWidgetsTagCloud.h ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 11:58:05 2013 (+0200)
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>
#include "dtkWidgetsTag.h"

#include <QtWidgets>

class dtkWidgetsTagCloudPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloud : public QTextBrowser
{
    Q_OBJECT

public:
    enum SortingType {
        Alpha,
        Num
    };

    enum SortingOrder {
        Asc,
        Desc
    };

public:
    dtkWidgetsTagCloud(QWidget *parent = 0);
    ~dtkWidgetsTagCloud(void);

#pragma mark -
#pragma mark Tag management

    void addTag(QString text, int instances);
    void addTag(QString text, int instances, QStringList items);
    void addTag(QString text, int instances, QStringList items, QString color);
    void addTag(dtkWidgetsTag tag);

#pragma mark -
#pragma mark Font management

    void setFontSize(int size);
    void setFontRange(int range);

#pragma mark -
#pragma mark Sorting

    void setSortingType(SortingType type);
    void setSortingOrder(SortingOrder order);

signals:
    void tagClicked(QString tag);
    void tagClicked(QString tag, QStringList items);

public slots:
    void sort(void);
    void clear(void);
    void render(void);

protected slots:
    void onLinkClicked(const QUrl& item);

protected:
    dtkWidgetsTagCloudPrivate *d;
};
