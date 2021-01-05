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

#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenuItem.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

static int item_height = 32;

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuNavigationFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuNavigationFacade : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuNavigationFacade(QWidget *parent = nullptr);
    dtkWidgetsMenuNavigationFacade(dtkWidgetsMenu *source, QWidget *parent = nullptr);
    ~dtkWidgetsMenuNavigationFacade(void);

public:
    void setMenu(dtkWidgetsMenu *menu);

signals:
    void clicked(void);

public:
    QSize sizeHint(void) const { return QSize(200, ::item_height + 20); }

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event)
    {
        if (event->pos().x() < 50)
            emit clicked();
    }

public:
    dtkWidgetsMenu *menu = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuHeaderFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuHeaderFacade : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuHeaderFacade(dtkWidgetsMenu *source, QWidget *parent = nullptr);
    ~dtkWidgetsMenuHeaderFacade(void);

public:
    QSize sizeHint(void) const { return QSize(200, ::item_height); }

protected:
    void paintEvent(QPaintEvent *event);

public:
    dtkWidgetsMenu *menu = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuMenuSpacer
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemSpacerFacade : public QWidget
{
public:
    dtkWidgetsMenuItemSpacerFacade(int width, int height, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        m_width = width;
        m_height = height;
        this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

public:
    QSize sizeHint(void) const { return QSize(m_width, m_height); }

private:
    int m_width = 0;
    int m_height = 0;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuInnerFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuInnerFacade : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuInnerFacade(dtkWidgetsMenu *source, QWidget *parent = nullptr);
    ~dtkWidgetsMenuInnerFacade(void);

public:
    QSize sizeHint(void) const { return QSize(400, ::item_height); }

signals:
    void clicked(void);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    dtkWidgetsMenu *menu = nullptr;

public:
    int parent_index = -1;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemFacade : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuItemFacade(dtkWidgetsMenuItem *source, QWidget *parent = nullptr);
    ~dtkWidgetsMenuItemFacade(void);

public:
    QSize sizeHint(void) const { return QSize(400, ::item_height); }

signals:
    void clicked(void);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    dtkWidgetsMenuItem *menu_item = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemParameterFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemParameterFacade : public dtkWidgetsMenuItemFacade
{
    Q_OBJECT

public:
    dtkWidgetsMenuItemParameterFacade(dtkWidgetsMenuItemParameter *source,
                                      QWidget *parent = nullptr);
    ~dtkWidgetsMenuItemParameterFacade(void);

private:
    QMetaObject::Connection param_connect;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemWorkspaceFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemCustomFacade : public dtkWidgetsMenuItemFacade
{
    Q_OBJECT

public:
    dtkWidgetsMenuItemCustomFacade(dtkWidgetsMenuItemCustom *source, QWidget *parent = nullptr);
    ~dtkWidgetsMenuItemCustomFacade(void);
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuFacade
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuFacade : public QWidget
{
    Q_OBJECT

public:
    dtkWidgetsMenuFacade(dtkWidgetsMenu *, QWidget *parent = nullptr);
    ~dtkWidgetsMenuFacade(void);

signals:
    void back(void);
    void clicked(dtkWidgetsMenu *);

public:
    void addItem(QSpacerItem *);
    void addWidget(QWidget *);

public:
    QList<QWidget *> items(void);

public:
    dtkWidgetsMenu *menu = nullptr;
};

//
// dtkWidgetsMenu+ux.h ends here
