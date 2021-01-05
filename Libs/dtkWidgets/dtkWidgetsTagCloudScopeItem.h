// Version: $Id: b7a25c68f78b741d082724f9c9a2ec469dc17c69 $
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

#include <QtWidgets>
#include <dtkWidgetsExport.h>

class dtkWidgetsTagCloudScopeItemPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudScopeItem : public QWidget
{
    Q_OBJECT

public:
    dtkWidgetsTagCloudScopeItem(QWidget *parent = 0);
    ~dtkWidgetsTagCloudScopeItem(void);

signals:
    void clicked(void);

public:
    QSize sizeHint(void) const;

public:
    QString text(void);

public:
    void setDark(void);
    void setDoom(void);

public slots:
    void setText(const QString &text);
    void setCount(int count);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

protected:
    void mouseReleaseEvent(QMouseEvent *);

protected:
    dtkWidgetsTagCloudScopeItemPrivate *d;
};

//
// dtkWidgetsTagCloudScopeItem.h ends here
