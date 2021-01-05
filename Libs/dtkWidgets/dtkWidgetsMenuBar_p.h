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

#pragma once

#include <dtkWidgetsExport>

#include "dtkWidgetsOverlayPane.h"
#include "dtkWidgetsOverlayPaneSlider.h"

#include <QtCore>
#include <QtWidgets>

class dtkWidgetsMenu;
class dtkWidgetsMenuBar;
class dtkWidgetsMenuBarButton;
class dtkWidgetsMenuBarButtonCollapse;
class dtkWidgetsMenuBarContainer;
class dtkWidgetsMenuBarContainerSlide;
class dtkWidgetsMenuNavigationFacade;
class dtkWidgetsMenuSpy;

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarContainer
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuBarContainer : public dtkWidgetsOverlayPane
{
    Q_OBJECT

public:
    dtkWidgetsMenuBarContainer(QWidget *parent = nullptr);
    ~dtkWidgetsMenuBarContainer(void);

signals:
    void indexVisible(int);

public slots:
    void setCurrentIndex(int, std::function<void()> &);

public slots:
    void switchToRoot(std::function<void()> &);
    void switchToPrevSlide(dtkWidgetsMenu *);
    void switchToNextSlide(dtkWidgetsMenu *);

public:
    void decr(void); // NOTE: Use with caution

public:
    void build(const QVector<dtkWidgetsMenu *> &);
    void buildChildSlide(dtkWidgetsMenu *);

public:
    void touch(void);
    void touch(dtkWidgetsMenuBar *);

public:
    dtkWidgetsMenuBar *q = nullptr;

public:
    dtkWidgetsOverlayPaneSlider *slider = nullptr;

public:
    QVector<QWidget *> frames;
    QHash<dtkWidgetsMenu *, dtkWidgetsMenuBarContainerSlide *> slides;
    dtkWidgetsMenuBarContainerSlide *master_slide = nullptr;
    dtkWidgetsMenuNavigationFacade *navigator = nullptr;

    // /////////////////////////////////////////////////////////////////////////////
    // TODO: Special case of a container used standalone
    // /////////////////////////////////////////////////////////////////////////////

public:
    QVector<dtkWidgetsMenu *> master_menus;

    // /////////////////////////////////////////////////////////////////////////////

private:
    friend class dtkWidgetsMenuBarButton;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarContainerSlide
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuBarContainerSlide : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *, dtkWidgetsMenu *,
                                    dtkWidgetsMenuBarContainer * = nullptr);
    dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *, dtkWidgetsMenuBar *,
                                    dtkWidgetsMenuBarContainer * = nullptr);
    ~dtkWidgetsMenuBarContainerSlide(void);

protected:
    dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *, dtkWidgetsMenuBarContainer *);

public:
    bool isMaster(void) const { return (menu_bar != nullptr); }

public:
    void touch(dtkWidgetsMenuBar *, dtkWidgetsMenuBarContainer *);
    void touch(dtkWidgetsMenu *, dtkWidgetsMenuBarContainer *);
    void touch(const QVector<dtkWidgetsMenu *> &, dtkWidgetsMenuBarContainer *);

private:
    dtkWidgetsMenu *menu = nullptr;
    dtkWidgetsMenuBar *menu_bar = nullptr;
    dtkWidgetsMenuBar *q = nullptr;

private:
    friend class dtkWidgetsMenuBarContainer;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuBarPrivate
{
public:
    dtkWidgetsMenuBar *q = nullptr;
    dtkWidgetsMenuBarContainer *c = nullptr;
    dtkWidgetsMenuSpy *s = nullptr;

public:
    QHash<QString, dtkWidgetsMenu *> menu_hash;
    QVector<dtkWidgetsMenu *> menu_list;

public:
    QVBoxLayout *layout = nullptr;

public:
    QScrollArea *scroll = nullptr;

public:
    dtkWidgetsMenuBarButtonCollapse *toggle = nullptr;

public:
    void touch(void);

public:
    QPoint o_pos;
    QPoint c_pos;
    QPoint d_pos;

public:
    int size = 50;
    int margin = 18;
    int offset = 0;

public:
    bool interactive = true;
};

//
// dtkWidgetsMenuBar_p.h ends here
