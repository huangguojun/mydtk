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

class dtkWidgetsLayout;
class dtkWidgetsLayoutItem;
class dtkWidgetsLayoutItemButton;
class dtkWidgetsLayoutItemProxy;
class dtkWidgetsWidget;

class QComboBox;
class QFormLayout;
class QFrame;
class QSplitter;

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemProxyPrivate
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLayoutItemProxyPrivate
{
public:
    dtkWidgetsWidget *view = nullptr;

public:
    QFormLayout *form = nullptr;

public:
    bool dirty = true;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemPrivate
// /////////////////////////////////////////////////////////////////

class dtkWidgetsLayoutItemPrivate
{
public:
    static dtkWidgetsLayoutItemProxy *firstViewChild(dtkWidgetsLayoutItem *item);

public:
    dtkWidgetsLayoutItem *root = nullptr;
    dtkWidgetsLayoutItem *parent = nullptr;

public:
    dtkWidgetsLayoutItem *a = nullptr;
    dtkWidgetsLayoutItem *b = nullptr;

public:
    dtkWidgetsLayout *layout = nullptr;

public:
    dtkWidgetsLayoutItemProxy *proxy = nullptr;

public:
    QSplitter *splitter = nullptr;

public:
    dtkWidgetsLayoutItemButton *close = nullptr;
    dtkWidgetsLayoutItemButton *horzt = nullptr;
    dtkWidgetsLayoutItemButton *vertc = nullptr;
    dtkWidgetsLayoutItemButton *maxmz = nullptr;

public:
    QComboBox *label = nullptr;

public:
    QFrame *footer = nullptr;

public:
    dtkWidgetsLayoutItem *q = nullptr;
};

//
// dtkWidgetsLayoutItem_p.h ends here
