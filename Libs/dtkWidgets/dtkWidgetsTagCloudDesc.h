/* dtkWidgetsTagCloudDesc.h ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 14:36:15 2013 (+0200)
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudDescPrivate;

class dtkWidgetsTagCloudDesc : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsTagCloudDesc(QWidget *parent = 0);
    ~dtkWidgetsTagCloudDesc(void);

signals:
    void back(void);

public slots:
    void clear(void);

public slots:
    void setDescription(const QString& description);

protected:
    dtkWidgetsTagCloudDescPrivate *d;
};
