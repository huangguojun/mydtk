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

#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenuBar.h"
#include "dtkWidgetsMenuBar_p.h"
#include "dtkWidgetsMenuSpy.h"

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuSpyPrivate
// /////////////////////////////////////////////////////////////////////////////

class dtkWidgetsMenuSpyPrivate : public QObject
{
    Q_OBJECT

public:
    dtkWidgetsMenuBar          *menu_bar           = nullptr;
    dtkWidgetsMenuBarContainer *menu_bar_container = nullptr;

public slots:
    void onIndexClicked(int);
    void onIndexVisible(int);
};

void dtkWidgetsMenuSpyPrivate::onIndexClicked(int index)
{
    if(!this->menu_bar)
        return;

    if(!this->menu_bar_container)
        return;

    std::function<void (void)> callback = [=] (void) -> void
    {
        this->blockSignals(false);
        this->menu_bar_container->blockSignals(false);
    };

    this->blockSignals(true);
    this->menu_bar_container->blockSignals(true);
    this->menu_bar_container->setCurrentIndex(index, callback);
}

void dtkWidgetsMenuSpyPrivate::onIndexVisible(int index)
{
    if(!this->menu_bar)
        return;

    if(!this->menu_bar_container)
        return;

    std::function<void (void)> callback = [=] (void) -> void
    {
        this->menu_bar->blockSignals(false);
    };

    this->menu_bar->blockSignals(true);
    this->menu_bar->setCurrentIndex(index, callback);
}

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuSpy
// /////////////////////////////////////////////////////////////////////////////

dtkWidgetsMenuSpy::dtkWidgetsMenuSpy(QObject *parent) : QObject(parent), d(new dtkWidgetsMenuSpyPrivate)
{

}

dtkWidgetsMenuSpy::~dtkWidgetsMenuSpy(void)
{
    delete d;
}

void dtkWidgetsMenuSpy::setMenuBar(dtkWidgetsMenuBar *menu_bar)
{
    d->menu_bar = menu_bar;

    connect(d->menu_bar, SIGNAL(clicked(int)), d, SLOT(onIndexClicked(int)));
}

void dtkWidgetsMenuSpy::setMenuBarContainer(dtkWidgetsMenuBarContainer *menu_bar_container)
{
    d->menu_bar_container = menu_bar_container;

    connect(d->menu_bar_container, SIGNAL(indexVisible(int)), d, SLOT(onIndexVisible(int)));
}

// /////////////////////////////////////////////////////////////////////////////

#include "dtkWidgetsMenuSpy.moc"

//
// dtkWidgetsMenuSpy.cpp ends here
