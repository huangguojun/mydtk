// Version: $Id: a37c6007da868436041b6be33f91fbb490db2fdd $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewLayout.h"
#include "dtkViewLayoutItem.h"

class dtkViewLayoutPrivate
{
public:
    dtkViewLayoutItem *root;
    dtkViewLayoutItem *current;
};

dtkViewLayout::dtkViewLayout(QWidget *parent) : QFrame(parent), d(new dtkViewLayoutPrivate)
{
    d->root = new dtkViewLayoutItem(0);
    d->root->setParent(this);
    d->root->setLayout(this);

    d->current = d->root;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->root);

    connect(d->root, SIGNAL(focused(dtkViewWidget *)), this, SIGNAL(focused(dtkViewWidget *)));
    connect(d->root, SIGNAL(unfocused(dtkViewWidget *)), this, SIGNAL(unfocused(dtkViewWidget *)));
}

dtkViewLayout::~dtkViewLayout(void)
{
    delete d;

    d = NULL;
}

dtkViewLayoutItem *dtkViewLayout::root(void)
{
    return d->root;
}

dtkViewLayoutItem *dtkViewLayout::current(void)
{
    return d->current;
}

void dtkViewLayout::setCurrent(dtkViewLayoutItem *item)
{
    d->current = item;
}

void dtkViewLayout::clear(void)
{
    d->root->clear();
}

//
// dtkViewLayout.cpp ends here
