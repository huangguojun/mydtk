
#include "dtkWidgetsMainWindow.h"

#include "dtkApplication.h"
#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenuBar.h"
#include "dtkWidgetsMenuItem.h"
#include "dtkWidgetsMenuItem+custom.h"

#include <dtkThemes/dtkThemesEngine>
#include <dtkFonts/dtkFontAwesome>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMainWindowPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMainWindowPrivate
{
public:
    dtkWidgetsMenuBar *menubar = nullptr;

public:
    dtkWidgetsMenu *menu_files  = nullptr;
    dtkWidgetsMenu *menu_themes = nullptr;
    dtkWidgetsMenu *menu_about  = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMainWindow
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMainWindow::dtkWidgetsMainWindow(QWidget *parent) : QMainWindow(parent), d(new dtkWidgetsMainWindowPrivate)
{
    //dtkApp->setWindow(this);
}

dtkWidgetsMainWindow::~dtkWidgetsMainWindow(void)
{
    delete d;
}

dtkWidgetsMenuBar *dtkWidgetsMainWindow::menubar(void)
{
    if(!d->menubar)
        d->menubar = new dtkWidgetsMenuBar(this);

    return d->menubar;
}

void dtkWidgetsMainWindow::populate(void)
{
//  Q_UNUSED(this->populateFileMenu());
    Q_UNUSED(this->populateThemesMenu());
//  Q_UNUSED(this->populateAboutMenu());
}

dtkWidgetsMenu *dtkWidgetsMainWindow::populateFileMenu(void)
{
    Q_UNUSED(this->menubar());

    if(!d->menu_files) {
        d->menu_files = d->menubar->insertMenu(0, fa::file, "Files");
        d->menu_files->addItem(0, "Open");
        d->menu_files->addItem(0, "Open Recent");
        d->menu_files->addItem(0, "Save");
        d->menu_files->addItem(0, "Save As");
    }

    return d->menu_files;
}

dtkWidgetsMenu *dtkWidgetsMainWindow::populateThemesMenu(void)
{
    Q_UNUSED(this->menubar());

    if(!d->menu_themes) {

        d->menu_themes = d->menubar->addMenu(fa::paintbrush, "Themes");
        auto *inner_menu_theme = d->menu_themes->addMenu(fa::paintbrush, "Choose Theme");
        for (const QString theme : dtkThemesEngine::instance()->themes()) {
            connect(static_cast<dtkWidgetsMenuItem *>(inner_menu_theme->addItem(new dtkWidgetsMenuItemTheme(theme))), &dtkWidgetsMenuItem::clicked, [=] (void) -> void {
                dtkThemesEngine::instance()->apply(theme);
            });
        }
    }

    return d->menu_themes;
}

dtkWidgetsMenu *dtkWidgetsMainWindow::populateAboutMenu(void)
{
    Q_UNUSED(this->menubar());

    if(!d->menu_about) {
        d->menu_about = d->menubar->addMenu(fa::question, "About");
        d->menu_about->addMenu(fa::question, "dtk")->addItem(0, "prout");
        d->menu_about->addMenu(fa::question, "dtk environment (conda)")->addItem(0, "Pour Come");
    }

    return d->menu_about;
}

void dtkWidgetsMainWindow::resizeEvent(QResizeEvent *event)
{
    //dtkThemesWidgetsMainWindow::resizeEvent(event);
    QMainWindow::resizeEvent(event);

    if (d->menubar)
        d->menubar->setFixedHeight(event->size().height());
}

//
// dtkWidgetsMainWindow.cpp ends here
