// Version: $Id: fae3605c55c9ca740f2287326f1143d1869b21bb $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewWidget.h"

class dtkViewWidgetPrivate
{
public:

};

dtkViewWidget::dtkViewWidget(QWidget *parent) : QWidget(parent)
{
    d = new dtkViewWidgetPrivate;
}

dtkViewWidget::~dtkViewWidget(void)
{
    delete d;
}

QWidget *dtkViewWidget::widget(void)
{
    return NULL;
}

QWidget *dtkViewWidget::inspector(void)
{
    return NULL;
}

void dtkViewWidget::mousePressEvent(QMouseEvent *event)
{
    dtkViewWidget::mousePressEvent(event);

    emit focused();
}

//
// dtkViewWidget.cpp ends here
