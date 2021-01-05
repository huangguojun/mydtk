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

#include "dtkWidgetsMenu+ux.h"
#include "dtkWidgetsMenuItem+custom.h"
#include "dtkWidgetsParameter.h" // TODO: Rename to dtkWidgetsParameterBase.h
#include "dtkWidgetsParameter.h"

#include <dtkFonts/dtkFontAwesome>

#include <dtkThemes/dtkThemesEngine>
//#include <dtkCore/dtkCoreParameter.h>

#include <QtGui>

// ///////////////////////////////////////////////////////////////////
// TODO: Substitute *inplace* with dtkThemesEngine
// ///////////////////////////////////////////////////////////////////

static QColor bg_color = QColor("#88aaaaaa");
static QColor fg_color = QColor("#88ffffff");

static int padding = 5;
static int margin = 10;

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuHelper
// ///////////////////////////////////////////////////////////////////

namespace dtkWidgetsMenuHelper {

static int margin = 16 / 2;

QWidget *build(QObject *source)
{
    if (dtkWidgetsMenu *menu = dynamic_cast<dtkWidgetsMenu *>(source)) {

        return new dtkWidgetsMenuInnerFacade(menu);

    } else if (dtkWidgetsMenuItem *item = dynamic_cast<dtkWidgetsMenuItem *>(source)) {

        if (item->isParameter()) {

            return new dtkWidgetsMenuItemParameterFacade(
                    static_cast<dtkWidgetsMenuItemParameter *>(item));

        } else if (item->isCustom()) {

            return new dtkWidgetsMenuItemCustomFacade(
                    static_cast<dtkWidgetsMenuItemCustom *>(item));

        } else if (item->isSeparator()) {

            return new dtkWidgetsMenuItemSpacerFacade(margin, margin);

        } else {

            return new dtkWidgetsMenuItemFacade(item);
        }
    }

    return nullptr;
}

} // namespace dtkWidgetsMenuHelper

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuNavigationFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuNavigationFacade::dtkWidgetsMenuNavigationFacade(QWidget *parent) : QFrame(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

dtkWidgetsMenuNavigationFacade::dtkWidgetsMenuNavigationFacade(dtkWidgetsMenu *source,
                                                               QWidget *parent)
    : QFrame(parent), menu(source)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

dtkWidgetsMenuNavigationFacade::~dtkWidgetsMenuNavigationFacade(void)
{
    this->menu = nullptr;
}

void dtkWidgetsMenuNavigationFacade::setMenu(dtkWidgetsMenu *menu)
{
    this->menu = menu;

    this->update();
}

void dtkWidgetsMenuNavigationFacade::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    dtkFontAwesome::instance()->setDefaultOption("color",
                                                 dtkThemesEngine::instance()->color("@base0"));

    QRect rect(5, 10, 28, 32);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(dtkThemesEngine::instance()->color("@base7"));
    painter.drawRoundedRect(rect, 5, 5);
    painter.drawPixmap(10, 20, 16, 16,
                       dtkFontAwesome::instance()->icon(fa::chevronleft).pixmap(16, 16));
    painter.setPen(dtkThemesEngine::instance()->color("@base8"));
    if (this->menu)
        painter.drawText(::margin * 4, ::margin * 2 + 2 + 10, this->menu->title());
    else
        painter.drawText(::margin * 4, ::margin * 2 + 2 + 10, "All Items");
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuHeaderFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuHeaderFacade::dtkWidgetsMenuHeaderFacade(dtkWidgetsMenu *source, QWidget *parent)
    : QFrame(parent), menu(source)
{
    Q_ASSERT_X(this->menu, Q_FUNC_INFO, "Input dtkWidgetsMenu must be non void.");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

dtkWidgetsMenuHeaderFacade::~dtkWidgetsMenuHeaderFacade(void)
{
    this->menu = nullptr;
}

void dtkWidgetsMenuHeaderFacade::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    dtkFontAwesome::instance()->setDefaultOption("color",
                                                 dtkThemesEngine::instance()->color("@fg"));

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(dtkThemesEngine::instance()->color("@fg"));
    painter.drawPixmap(::margin, ::margin,
                       dtkFontAwesome::instance()->icon(this->menu->icon()).pixmap(16, 16));
    painter.drawText(::margin + 16 + ::margin, ::margin * 2 + 2, this->menu->title());
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuInnerFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuInnerFacade::dtkWidgetsMenuInnerFacade(dtkWidgetsMenu *source, QWidget *parent)
    : QFrame(parent), menu(source)
{
    Q_ASSERT_X(this->menu, Q_FUNC_INFO, "Input dtkWidgetsMenu must be non void.");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

dtkWidgetsMenuInnerFacade::~dtkWidgetsMenuInnerFacade(void)
{
    this->menu = nullptr;
}

void dtkWidgetsMenuInnerFacade::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    dtkFontAwesome::instance()->setDefaultOption("color",
                                                 dtkThemesEngine::instance()->color("@base8"));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(dtkThemesEngine::instance()->color("@base8"));
    painter.drawPixmap(10, 10, 16, 16,
                       dtkFontAwesome::instance()->icon(this->menu->icon()).pixmap(16, 16));
    painter.drawText(::margin + 16 + ::margin, ::margin * 2 + 2, this->menu->title());
    painter.drawPixmap(this->width() - ::margin - 16, 10, 16, 16,
                       dtkFontAwesome::instance()->icon(fa::chevronright).pixmap(16, 16));
}

void dtkWidgetsMenuInnerFacade::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemFacade::dtkWidgetsMenuItemFacade(dtkWidgetsMenuItem *source, QWidget *parent)
    : QFrame(parent), menu_item(source)
{
    Q_ASSERT_X(this->menu_item, Q_FUNC_INFO, "Input dtkWidgetsMenuItem must be non void.");

    connect(this, SIGNAL(clicked()), source, SIGNAL(clicked()));

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

dtkWidgetsMenuItemFacade::~dtkWidgetsMenuItemFacade(void)
{
    this->menu_item = nullptr;
}

void dtkWidgetsMenuItemFacade::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (menu_item->isParameter())
        return;

    if (menu_item->isCustom())
        return;

    dtkFontAwesome::instance()->setDefaultOption("color",
                                                 dtkThemesEngine::instance()->color("@fg"));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(dtkThemesEngine::instance()->color("@fg"));
    painter.drawPixmap(10, 10, 16, 16,
                       dtkFontAwesome::instance()->icon(this->menu_item->icon()).pixmap(16, 16));
    painter.drawText(::margin + 16 + ::margin, ::margin * 2 + 2, this->menu_item->title());
}

void dtkWidgetsMenuItemFacade::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemParameterFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemParameterFacade::dtkWidgetsMenuItemParameterFacade(
        dtkWidgetsMenuItemParameter *source, QWidget *parent)
    : dtkWidgetsMenuItemFacade(source, parent)
{
    QFormLayout *layout = new QFormLayout(this);
    layout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    layout->addRow(source->title(), source->widget());

    /*
    if(source->parameter()->advanced()) {
        this->hide();
    }


    this->param_connect = connect(source->parameter(),
    &dtkCoreParameter::advancedChanged, [this] (bool advanced) { if(advanced)
                this->hide();
            else
                this->show();
        });
        */

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

dtkWidgetsMenuItemParameterFacade::~dtkWidgetsMenuItemParameterFacade(void)
{
    QObject::disconnect(this->param_connect);
    this->layout()->itemAt(1)->widget()->setParent(nullptr);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemCustomFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemCustomFacade::dtkWidgetsMenuItemCustomFacade(dtkWidgetsMenuItemCustom *source,
                                                               QWidget *parent)
    : dtkWidgetsMenuItemFacade(source, parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(source->widget());

    if (dtkWidgetsMenuItemDIY *item = dynamic_cast<dtkWidgetsMenuItemDIY *>(source))
        this->setSizePolicy(QSizePolicy::Expanding, item->policy());
}

dtkWidgetsMenuItemCustomFacade::~dtkWidgetsMenuItemCustomFacade(void)
{
    this->layout()->itemAt(0)->widget()->setParent(nullptr);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuFacade
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuFacade::dtkWidgetsMenuFacade(dtkWidgetsMenu *m, QWidget *parent)
    : QWidget(parent), menu(m)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);
    layout->setAlignment(Qt::AlignTop);

    for (QObject *object : m->objects()) {
        this->addWidget(dtkWidgetsMenuHelper::build(object));
    }
}

dtkWidgetsMenuFacade::~dtkWidgetsMenuFacade(void) {}

void dtkWidgetsMenuFacade::addItem(QSpacerItem *item)
{
    static_cast<QVBoxLayout *>(this->layout())->addItem(item);
}

void dtkWidgetsMenuFacade::addWidget(QWidget *widget)
{
    static_cast<QVBoxLayout *>(this->layout())->addWidget(widget);
}

QList<QWidget *> dtkWidgetsMenuFacade::items(void)
{
    QList<QWidget *> items;

    for (int i = 0; i < this->layout()->count(); ++i) {

        if (QWidget *widget = this->layout()->itemAt(i)->widget()) {

            if (dynamic_cast<dtkWidgetsMenuItemFacade *>(widget))
                items << widget;
            else if (dynamic_cast<dtkWidgetsMenuInnerFacade *>(widget))
                items << widget;
            else if (dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(widget))
                items << widget;
            else if (dynamic_cast<dtkWidgetsMenuHeaderFacade *>(widget))
                items << widget;
        }
    }

    return items;
}

//
// dtkWidgetsMenu+ux.cpp ends here
