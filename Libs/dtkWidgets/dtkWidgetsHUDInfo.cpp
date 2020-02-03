// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsHUDInfo.h"

#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsHUDInfoPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsHUDInfoPrivate
{
public:
    QString text;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsHUDInfo
// ///////////////////////////////////////////////////////////////////

dtkWidgetsHUDInfo::dtkWidgetsHUDInfo(const QString& text, QWidget *parent) : QFrame(parent), d(new dtkWidgetsHUDInfoPrivate)
{
    d->text = text;

    this->setVisible(true);
}

dtkWidgetsHUDInfo::~dtkWidgetsHUDInfo(void)
{
    delete d;
}

QSize dtkWidgetsHUDInfo::sizeHint(void) const
{
    return QSize(200, 50);
}

void dtkWidgetsHUDInfo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QColor("#cc2d2e2e"));
    painter.setBrush(QColor("#cc2d2e2e"));
    painter.drawRoundedRect(event->rect(), 25 + 2, 25 + 2);
    painter.setPen(QColor("#bebfbf"));
    painter.drawText(event->rect(), d->text, QTextOption(Qt::AlignCenter));

    QFrame::paintEvent(event);
}

//
// dtkWidgetsHUDInfo.cpp ends here
