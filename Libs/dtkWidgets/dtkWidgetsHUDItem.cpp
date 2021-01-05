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

#include "dtkWidgetsHUDItem.h"

#include "dtkWidgetsLayoutItem.h"
#include "dtkWidgetsOverlayPane.h"

#include <dtkThemes>

#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsHUDItemPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsHUDItemPrivate
{
public:
    QLabel *label;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkWidgetsHUDItem::dtkWidgetsHUDItem(fa::icon icon, QWidget *parent)
    : QFrame(parent), d(new dtkWidgetsHUDItemPrivate)
{
    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("scale-factor", 1.0);
    dtkFontAwesome::instance()->setDefaultOption("color",
                                                 dtkThemesEngine::instance()->color("@fg"));

    d->label = new QLabel(this);
    d->label->setPixmap(dtkFontAwesome::instance()->icon(icon).pixmap(16, 16));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(d->label);

    this->setLayout(layout);
    this->setFixedWidth(30);
    this->setFixedHeight(30);
}

dtkWidgetsHUDItem::~dtkWidgetsHUDItem(void)
{
    delete d;
}

void dtkWidgetsHUDItem::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}

void dtkWidgetsHUDItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(dtkThemesEngine::instance()->color("@fg"));
    painter.setBrush(dtkThemesEngine::instance()->color("@bg"));
    painter.drawRoundedRect(event->rect(), event->rect().size().width() / 2,
                            event->rect().size().height() / 2);

    QFrame::paintEvent(event);
}

//
// dtkWidgetsHUDItem.cpp ends here
