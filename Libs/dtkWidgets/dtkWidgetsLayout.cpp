// Version: $Id: c38ed7cb8f50e3cfa5bb6c151e0d9ce3c97e3ed4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsLayout.h"
#include "dtkWidgetsController.h"
#include "dtkWidgetsLayoutItem.h"

#include <QtCore>
#include <QtWidgets>

/*!

  \class dtkWidgetsLayout

  \brief The dtkWidgetsLayout class is s composite class that acts as a
  visual proxy to the layout item interface. It actually composes a root
  node.

  \since 2.0.0

  \ingroup dtkWidgets

  dtkWidgetsLayout is a top level frame that instanciates a root node,
  which actally is a binary tree. It is responsible for satisfying the
  node's interface, when it comes to its ui.

  \sa dtkWidgetsLayoutItem, dtkWidgetsItemProxy

*/

class dtkWidgetsLayoutPrivate
{
public:
    dtkWidgetsLayoutItem *root = nullptr;
    dtkWidgetsLayoutItem *current = nullptr;

public:
    dtkWidgetsLayout::creator creator = nullptr;
};

/*!

  \fn dtkWidgetsLayout::dtkWidgetsLayout(QWidget *parent)

  \brief Construcs a dtkWidgetsLayout

  \ingroup dtkWidgets

  Inherits QFrame to be styled.

  \sa QFrame

*/

dtkWidgetsLayout::dtkWidgetsLayout(QWidget *parent) : QFrame(parent), d(new dtkWidgetsLayoutPrivate)
{
    d->root = new dtkWidgetsLayoutItem(0);
    d->root->setParent(this);
    d->root->setLayout(this);

    d->current = d->root;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->root);

    connect(d->root, SIGNAL(focused(dtkWidgetsWidget *)), this,
            SIGNAL(focused(dtkWidgetsWidget *)));
    connect(d->root, SIGNAL(unfocused(dtkWidgetsWidget *)), this,
            SIGNAL(unfocused(dtkWidgetsWidget *)));
    // TODO: // connect(d->root, SIGNAL(create()), this, SIGNAL(create()));
    connect(d->root, SIGNAL(create(const QString &)), this, SLOT(create(const QString &)));
}

dtkWidgetsLayout::~dtkWidgetsLayout(void)
{
    delete d->root;

    delete d;

    d = NULL;
}

dtkWidgetsLayoutItem *dtkWidgetsLayout::root(void)
{
    return d->root;
}

dtkWidgetsLayoutItem *dtkWidgetsLayout::current(void)
{
    return d->current;
}

void dtkWidgetsLayout::setCreator(dtkWidgetsLayout::creator creator)
{
    d->creator = creator;
}

void dtkWidgetsLayout::setCurrent(dtkWidgetsLayoutItem *item)
{
    disconnect(d->current, 0, 0, 0);

    d->current = item;

    connect(d->current, SIGNAL(create()), this, SLOT(create()));
    connect(d->current, SIGNAL(create(const QString &)), this, SLOT(create(const QString &)));
}

void dtkWidgetsLayout::clear(void)
{
    d->root->clear();
}

void dtkWidgetsLayout::create(void)
{
    this->create("");
}

void dtkWidgetsLayout::create(const QString &name)
{
    if (!d->creator)
        return;

    dtkWidgetsWidget *widget = d->creator(name);

    if (widget) {
        d->current->setView(widget);
        dtkWidgetsController::instance()->insert(widget);
    }
}

void dtkWidgetsLayout::keyPressEvent(QKeyEvent *event)
{
    event->ignore();

    QFrame::keyPressEvent(event);
}

//
// dtkWidgetsLayout.cpp ends here
