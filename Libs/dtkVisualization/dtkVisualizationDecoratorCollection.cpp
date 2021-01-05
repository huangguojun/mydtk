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

#include "dtkVisualizationDecoratorCollection.h"

#include "dtkVisualizationCanvas.h"
#include "dtkVisualizationDecorator.h"

#include <QtCore>

#include <algorithm>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorCollectionPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorCollectionPrivate
{
public:
    std::list<dtkVisualizationDecorator *> collection;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorCollection
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(void)
    : d(new dtkVisualizationDecoratorCollectionPrivate)
{
}

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(
        const QList<dtkVisualizationDecorator *> &l)
    : d(new dtkVisualizationDecoratorCollectionPrivate)
{
    for (auto dec : l) {
        d->collection.emplace_back(dec);
    }
}

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(
        dtkVisualizationDecorator *dec)
    : d(new dtkVisualizationDecoratorCollectionPrivate)
{
    d->collection.emplace_back(dec);
}

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(
        const std::list<dtkVisualizationDecorator *> &l)
    : d(new dtkVisualizationDecoratorCollectionPrivate)
{
    d->collection = l;
}

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(
        const dtkVisualizationDecoratorCollection &o)
    : d(new dtkVisualizationDecoratorCollectionPrivate)
{
    d->collection = o.d->collection;
}

dtkVisualizationDecoratorCollection::dtkVisualizationDecoratorCollection(
        dtkVisualizationDecoratorCollection &&o)
    : d(o.d)
{
    o.d = nullptr;
}

dtkVisualizationDecoratorCollection::~dtkVisualizationDecoratorCollection(void)
{
    if (d) {
        this->clear();
        delete d;
    }
    d = nullptr;
}

bool dtkVisualizationDecoratorCollection::add(dtkVisualizationDecorator *dec)
{
    auto it = std::find(std::begin(d->collection), std::end(d->collection), dec);
    if (it == std::end(d->collection)) {
        d->collection.emplace_back(dec);
        return true;
    } else {
        return false;
    }
}

bool dtkVisualizationDecoratorCollection::remove(dtkVisualizationDecorator *dec)
{
    auto it = std::find(std::begin(d->collection), std::end(d->collection), dec);
    if (it != std::end(d->collection)) {
        d->collection.erase(it);
        dec->unsetCanvas();
        return true;
    } else {
        return false;
    }
}

void dtkVisualizationDecoratorCollection::clear(void)
{
    for (auto dec : d->collection) {
        dec->unsetCanvas();
        delete dec;
    }
    d->collection.clear();
}

bool dtkVisualizationDecoratorCollection::empty(void) const
{
    return d->collection.empty();
}

std::size_t dtkVisualizationDecoratorCollection::size(void) const
{
    return d->collection.size();
}

dtkVisualizationDecoratorCollection &dtkVisualizationDecoratorCollection::
operator<<(dtkVisualizationDecorator *dec)
{
    this->add(dec);
    return *this;
}

dtkVisualizationDecoratorCollection &dtkVisualizationDecoratorCollection::
operator<<(const QList<dtkVisualizationDecorator *> &l)
{
    for (auto dec : l) {
        this->add(dec);
    }
    return *this;
}

dtkVisualizationDecoratorCollection &dtkVisualizationDecoratorCollection::
operator<<(const std::list<dtkVisualizationDecorator *> &l)
{
    for (auto dec : l) {
        this->add(dec);
    }
    return *this;
}

dtkVisualizationDecoratorCollection &dtkVisualizationDecoratorCollection::
operator<<(const dtkVisualizationDecoratorCollection &l)
{
    for (auto dec : l) {
        this->add(dec);
    }
    return *this;
}

auto dtkVisualizationDecoratorCollection::begin(void) -> iterator
{
    return d->collection.begin();
}

auto dtkVisualizationDecoratorCollection::begin(void) const -> const_iterator
{
    return d->collection.begin();
}

auto dtkVisualizationDecoratorCollection::cbegin(void) const -> const_iterator
{
    return d->collection.cbegin();
}

auto dtkVisualizationDecoratorCollection::end(void) -> iterator
{
    return d->collection.end();
}

auto dtkVisualizationDecoratorCollection::end(void) const -> const_iterator
{
    return d->collection.end();
}

auto dtkVisualizationDecoratorCollection::cend(void) const -> const_iterator
{
    return d->collection.cend();
}

auto dtkVisualizationDecoratorCollection::rbegin(void) -> reverse_iterator
{
    return d->collection.rbegin();
}

auto dtkVisualizationDecoratorCollection::rbegin(void) const -> const_reverse_iterator
{
    return d->collection.rbegin();
}

auto dtkVisualizationDecoratorCollection::crbegin(void) const -> const_reverse_iterator
{
    return d->collection.crbegin();
}

auto dtkVisualizationDecoratorCollection::rend(void) -> reverse_iterator
{
    return d->collection.rend();
}

auto dtkVisualizationDecoratorCollection::rend(void) const -> const_reverse_iterator
{
    return d->collection.rend();
}

auto dtkVisualizationDecoratorCollection::crend(void) const -> const_reverse_iterator
{
    return d->collection.crend();
}

void dtkVisualizationDecoratorCollection::setCanvas(dtkVisualizationCanvas *c)
{
    for (auto dec : d->collection) {
        dec->setCanvas(c);
    }
    c->reset();
}

void dtkVisualizationDecoratorCollection::unsetCanvas(void)
{
    for (auto dec : d->collection) {
        dec->unsetCanvas();
    }
}

QVariant dtkVisualizationDecoratorCollection::data(void) const
{
    if (!d->collection.empty()) {
        return (*(d->collection.begin()))->data();
    }
    return QVariant();
}

dtkVisualizationCanvas *dtkVisualizationDecoratorCollection::canvas(void) const
{
    if (!d->collection.empty()) {
        return (*(d->collection.begin()))->canvas();
    }
    return nullptr;
}

void dtkVisualizationDecoratorCollection::draw(void)
{
    for (auto dec : d->collection) {
        dec->touch();
    }
    auto canvas = this->canvas();
    if (canvas) {
        canvas->draw();
    }
}

// dtkVisualizationDecoratorCollection.cpp ends here
