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

#include "dtkWidgetsMenuItem+custom.h"

#include "dtkWidgetsWorkspaceStackBar.h"

#include <dtkThemesWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemDIYPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemDIYPrivate
{
public:
    QWidget *source = nullptr;
    QWidget *widget = nullptr;

    QSizePolicy::Policy policy = QSizePolicy::Fixed;

    bool show_title = true;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemDIY implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemDIY::dtkWidgetsMenuItemDIY(const QString& title) : dtkWidgetsMenuItemCustom(title), d(new dtkWidgetsMenuItemDIYPrivate)
{
    d->source = new QWidget;
    d->source->setLayout(new QVBoxLayout);
}

dtkWidgetsMenuItemDIY::dtkWidgetsMenuItemDIY(const QString& title, QWidget *widget) : dtkWidgetsMenuItemCustom(title), d(new dtkWidgetsMenuItemDIYPrivate)
{
    d->source = widget;
}

dtkWidgetsMenuItemDIY::~dtkWidgetsMenuItemDIY(void)
{
    delete d;
}

void dtkWidgetsMenuItemDIY::addLayout(QLayout *layout)
{
    if(QBoxLayout *l = dynamic_cast<QBoxLayout *>(d->source->layout()))
        l->addLayout(layout);
}

void dtkWidgetsMenuItemDIY::addWidget(QWidget *widget)
{
    if(QBoxLayout *l = dynamic_cast<QBoxLayout *>(d->source->layout()))
        l->addWidget(widget);
}

QWidget *dtkWidgetsMenuItemDIY::widget(void)
{
    if (!d->widget) {
        if (!this->showTitle() || this->title().isEmpty()) {
            d->widget = d->source;
        } else {
            d->widget = new QWidget;

            QVBoxLayout *layout = new QVBoxLayout;
            layout->addWidget(new QLabel(this->title()));
            layout->addWidget(d->source);

            d->widget->setLayout(layout);
        }
    }
    return d->widget;
}

void dtkWidgetsMenuItemDIY::clear(void)
{
    QLayoutItem *child;

    if (d->source) {
        while ((child = d->source->layout()->takeAt(0)) != 0)
            delete child;
    }

    if (d->widget) {
        while ((child = d->widget->layout()->takeAt(0)) != 0)
            delete child;
    }
}

void dtkWidgetsMenuItemDIY::setSizePolicy(QSizePolicy::Policy policy)
{
    d->policy = policy;
}

QSizePolicy::Policy dtkWidgetsMenuItemDIY::policy(void)
{
    return d->policy;
}

void dtkWidgetsMenuItemDIY::setShowTitle(bool show_title)
{
    d->show_title = show_title;
}

bool dtkWidgetsMenuItemDIY::showTitle(void)
{
    return d->show_title;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemWorkspacePrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemWorkspacePrivate
{
public:
    dtkWidgetsWorkspaceStackBar *widget = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemWorkspace implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemWorkspace::dtkWidgetsMenuItemWorkspace(const QString& title, dtkWidgetsWorkspaceStackBar *bar) : dtkWidgetsMenuItemCustom(title), d(new dtkWidgetsMenuItemWorkspacePrivate)
{
    d->widget = bar;
}

dtkWidgetsMenuItemWorkspace::~dtkWidgetsMenuItemWorkspace(void)
{
    delete d;
}

QWidget *dtkWidgetsMenuItemWorkspace::widget(void)
{
    return d->widget;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemThemePrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemThemePrivate
{
public:
    QString theme;

public:
    QWidget *widget = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemTheme implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemTheme::dtkWidgetsMenuItemTheme(const QString& theme) : dtkWidgetsMenuItemCustom(theme), d(new dtkWidgetsMenuItemThemePrivate)
{
    d->theme = theme;
}

dtkWidgetsMenuItemTheme::~dtkWidgetsMenuItemTheme(void)
{
    delete d;
}

QWidget *dtkWidgetsMenuItemTheme::widget(void)
{
    if (!d->widget) {
        d->widget = new QWidget;

        QVBoxLayout *layout = new QVBoxLayout(d->widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(new QLabel(d->theme));
        layout->addWidget(new dtkThemesWidget(d->theme));

        d->widget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }

    return d->widget;
}

//
// dtkWidgetsMenuItem+custom.cpp ends here
