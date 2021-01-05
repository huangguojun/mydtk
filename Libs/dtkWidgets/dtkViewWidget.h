// Version: $Id: de305018ed309384a4e4fc9a8ea8c2c3d3ee821e $
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

class DTKWIDGETS_EXPORT dtkViewWidget : public QWidget
{
    Q_OBJECT

public:
    dtkViewWidget(QWidget *parent = Q_NULLPTR);
    ~dtkViewWidget(void);

signals:
    void focused(void);
    void unfocused(void);

public:
    virtual QWidget *widget(void);
    virtual QWidget *inspector(void);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    class dtkViewWidgetPrivate *d;
};

//
// dtkViewWidget.h ends here
