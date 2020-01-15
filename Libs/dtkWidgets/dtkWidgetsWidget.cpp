// Version: $Id: 9cac1080f3b43915e891d6bcb0007bc3d92e2a09 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsWidget.h"
#include "dtkApplication.h"
#include "dtkWidgetsMainWindow.h"

dtkWidgetsWidget::dtkWidgetsWidget(QWidget *parent) : QWidget(parent)
{

}

QWidget *dtkWidgetsWidget::widget(void)
{
    return nullptr;
}

QWidget *dtkWidgetsWidget::inspector(void)
{
    return nullptr;
}

void dtkWidgetsWidget::update(void)
{
}

void dtkWidgetsWidget::mousePressEvent(QMouseEvent *event)
{
    emit focused();

    QWidget::mousePressEvent(event);
}

void dtkWidgetsWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // needed for mouse tracking propagation
 //   if (qApp && dynamic_cast<dtkApplication*>(qApp))
   //     dtkApp->window()->touch();
}

//
// dtkWidgetsWidget.cpp ends here
