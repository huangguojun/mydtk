/* dtkAbstractDataComposite.cpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Wed Sep 21 14:28:37 2011 (+0200)
 * Version: $Id: 90c3369417d06008c5c4f088d791438dfa044d62 $
 * Last-Updated: jeu. oct.  9 09:12:05 2014 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 530
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataComposite.h"
#include "dtkGlobal.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataCompositePrivate
// /////////////////////////////////////////////////////////////////

class dtkAbstractDataCompositePrivate
{
public:
    dtkAbstractDataCompositePrivate(void);

    dtkAbstractDataCompositePrivate(QVector<dtkAbstractData *> *ivector);
    dtkAbstractDataCompositePrivate(QVector<dtkAbstractData *> &ivector);
    dtkAbstractDataCompositePrivate(const QVector<dtkAbstractData *> &ivector);

    dtkAbstractDataCompositePrivate(QList<dtkAbstractData *> *ilist);
    dtkAbstractDataCompositePrivate(QList<dtkAbstractData *> &ilist);
    dtkAbstractDataCompositePrivate(const QList<dtkAbstractData *> &ilist);

public:
    dtkAbstractDataComposite::Type type;

    const QVector<dtkAbstractData *> &const_vector;
    QVector<dtkAbstractData *> &vector;

    const QList<dtkAbstractData *> &const_list;
    QList<dtkAbstractData *> &list;

    const QVector<dtkAbstractData *> *cv;
    QVector<dtkAbstractData *> *vv;

    const QList<dtkAbstractData *> *cl;
    QList<dtkAbstractData *> *vl;
};

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(
        QVector<dtkAbstractData *> *ivector)
    : const_vector(*new QVector<dtkAbstractData *>()),
      vector(*ivector),
      const_list(*new QList<dtkAbstractData *>()),
      list(*new QList<dtkAbstractData *>()),
      cv(0),
      vv(ivector),
      cl(0),
      vl(0)
{
    cv = &const_vector;
    cl = &const_list;
    vl = &list;
}

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(
        const QVector<dtkAbstractData *> &ivector)
    : const_vector(ivector),
      vector(*new QVector<dtkAbstractData *>()),
      const_list(*new QList<dtkAbstractData *>()),
      list(*new QList<dtkAbstractData *>()),
      cv(&ivector)
{
    vv = &vector;
    cl = &const_list;
    vl = &list;
}

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(
        QVector<dtkAbstractData *> &ivector)
    : const_vector(*new QVector<dtkAbstractData *>()),
      vector(ivector),
      const_list(*new QList<dtkAbstractData *>()),
      list(*new QList<dtkAbstractData *>()),
      vv(&ivector)
{
    cv = &const_vector;
    cl = &const_list;
    vl = &list;
}

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(QList<dtkAbstractData *> *ilist)
    : const_vector(*new QVector<dtkAbstractData *>()),
      vector(*new QVector<dtkAbstractData *>()),
      const_list(*new QList<dtkAbstractData *>()),
      list(*ilist),
      vl(ilist)
{
    cv = &const_vector;
    vv = &vector;
    cl = &const_list;
}

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(
        const QList<dtkAbstractData *> &ilist)
    : const_vector(*new QVector<dtkAbstractData *>()),
      vector(*new QVector<dtkAbstractData *>()),
      const_list(ilist),
      list(*new QList<dtkAbstractData *>()),
      cl(&ilist)
{
    cv = &const_vector;
    vv = &vector;
    vl = &list;
}

dtkAbstractDataCompositePrivate::dtkAbstractDataCompositePrivate(QList<dtkAbstractData *> &ilist)
    : const_vector(*new QVector<dtkAbstractData *>()),
      vector(*new QVector<dtkAbstractData *>()),
      const_list(*new QList<dtkAbstractData *>()),
      list(ilist),
      vl(&ilist)
{
    cv = &const_vector;
    vv = &vector;
    cl = &const_list;
}

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataComposite definition
// /////////////////////////////////////////////////////////////////

//! Constructs a composite data from a const QVector.
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(const QVector<dtkAbstractData *> &vector,
                                                   dtkAbstractData *parent)
    : dtkAbstractData(parent), d(new dtkAbstractDataCompositePrivate(vector))
{
    d->type = dtkAbstractDataComposite::ConstVector;
}

//! Constructs a composite data from a QVector.
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(QVector<dtkAbstractData *> &vector,
                                                   dtkAbstractData *parent)
    : dtkAbstractData(parent), d(new dtkAbstractDataCompositePrivate(vector))
{
    d->type = dtkAbstractDataComposite::Vector;
}
//! Constructs a composite data from a const QList.
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(const QList<dtkAbstractData *> &list,
                                                   dtkAbstractData *parent)
    : dtkAbstractData(parent), d(new dtkAbstractDataCompositePrivate(list))
{
    d->type = dtkAbstractDataComposite::ConstList;
}

//! Constructs a composite data from a QList.
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(QList<dtkAbstractData *> &list,
                                                   dtkAbstractData *parent)
    : dtkAbstractData(parent), d(new dtkAbstractDataCompositePrivate(list))
{
    d->type = dtkAbstractDataComposite::List;
}

//! Destructs the composite data.
/*!
 *
 */
dtkAbstractDataComposite::~dtkAbstractDataComposite(void)
{
    if (d->cv) {
        delete d->cv;
        d->cv = NULL;
    }

    if (d->vv) {
        delete d->vv;
        d->vv = NULL;
    }

    if (d->cl) {
        delete d->cl;
        d->cl = NULL;
    }

    if (d->vl) {
        delete d->vl;
        d->vl = NULL;
    }

    delete d;
    d = NULL;
}

//! Insert \a data at the end of the composite.
/*!
 *
 */
void dtkAbstractDataComposite::append(dtkAbstractData *data)
{
    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        d->vector.append(data);
        break;

    case dtkAbstractDataComposite::List:
        d->list.append(data);
        break;

    case dtkAbstractDataComposite::ConstVector:
        qDebug() << DTK_PRETTY_FUNCTION
                 << "Composite data is of type const vector. Adding data failed";
        break;

    case dtkAbstractDataComposite::ConstList:
        qDebug() << DTK_PRETTY_FUNCTION
                 << "Composite data is of type const list. Adding data failed";
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data has no type. Adding data failed";
        break;
    }
}

//! Remove \a data from the composite.
/*!
 *
 */
void dtkAbstractDataComposite::remove(dtkAbstractData *data)
{
    qlonglong index;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        index = d->vector.indexOf(data);

        while (index >= 0) {
            d->vector.remove(index);
            index = d->vector.indexOf(data);
        }

        break;

    case dtkAbstractDataComposite::List:
        d->list.removeAll(data);
        break;

    case dtkAbstractDataComposite::ConstVector:
        qDebug() << DTK_PRETTY_FUNCTION
                 << "Composite data is of type const vector. Removing data failed";
        break;

    case dtkAbstractDataComposite::ConstList:
        qDebug() << DTK_PRETTY_FUNCTION
                 << "Composite data is of type const list. Removing data failed";
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Removing data failed";
        break;
    }
}

//! Returns true if the composite contains an occurrence of \a data;
//! otherwise returns false.
/*!
 *
 */
bool dtkAbstractDataComposite::has(dtkAbstractData *data) const
{
    bool has = false;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        has = d->vector.contains(data);
        break;

    case dtkAbstractDataComposite::List:
        has = d->list.contains(data);
        break;

    case dtkAbstractDataComposite::ConstVector:
        has = d->const_vector.contains(data);
        break;

    case dtkAbstractDataComposite::ConstList:
        has = d->const_list.contains(data);
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Returns false.";
        break;
    }

    return has;
}

//! Returns the number of items in the composite.
/*!
 *
 */
qlonglong dtkAbstractDataComposite::count(void)
{
    qlonglong count = 0;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        count = d->vector.count();
        break;

    case dtkAbstractDataComposite::List:
        count = d->list.count();
        break;

    case dtkAbstractDataComposite::ConstVector:
        count = d->const_vector.count();
        break;

    case dtkAbstractDataComposite::ConstList:
        count = d->const_list.count();
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Returns 0.";
        break;
    }

    return count;
}

//! Returns the index position of the first occurrence of \a data in
//! the composite, searching forward from index position \a
//! from. Returns -1 if no item matched.
/*!
 *
 */
qlonglong dtkAbstractDataComposite::indexOf(dtkAbstractData *data, qlonglong from)
{
    qlonglong index = -1;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        index = d->vector.indexOf(data, from);
        break;

    case dtkAbstractDataComposite::List:
        index = d->list.indexOf(data, from);
        break;

    case dtkAbstractDataComposite::ConstVector:
        index = d->const_vector.indexOf(data, from);
        break;

    case dtkAbstractDataComposite::ConstList:
        index = d->const_list.indexOf(data, from);
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Returns -1.";
        break;
    }

    return index;
}

//! Returns type of the composite (eg vector or list).
/*!
 *
 */
dtkAbstractDataComposite::Type dtkAbstractDataComposite::type(void)
{
    return d->type;
}

//! Returns the item at index position \a index in the composite.
/*!
 *   \a index must be a valid index position in the composite (i.e., 0 <= i <
 * size).
 */
const dtkAbstractData *dtkAbstractDataComposite::at(qlonglong index) const
{
    dtkAbstractData *data = NULL;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        data = d->vector.at(index);
        break;

    case dtkAbstractDataComposite::List:
        data = d->list.at(index);
        break;

    case dtkAbstractDataComposite::ConstVector:
        data = d->const_vector.at(index);
        break;

    case dtkAbstractDataComposite::ConstList:
        data = d->const_list.at(index);
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Returns NULL.";
        break;
    }

    return data;
}

//! Returns the item at index position \a index in the composite.
/*!
 *   \a index must be a valid index position in the composite (i.e., 0 <= i <
 * size).
 */
dtkAbstractData *dtkAbstractDataComposite::at(qlonglong index)
{
    dtkAbstractData *data = NULL;

    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        data = d->vector[index];
        break;

    case dtkAbstractDataComposite::List:
        data = d->list[index];
        break;

    case dtkAbstractDataComposite::ConstVector:
        data = d->const_vector[index];
        break;

    case dtkAbstractDataComposite::ConstList:
        data = d->const_list[index];
        break;

    default:
        qDebug() << DTK_PRETTY_FUNCTION << "Composite data is empty. Returns NULL.";
        break;
    }

    return data;
}

//! Returns the data of the composite as a Qt vector.
/*!
 *  When composite has been created using a Qt vector, a reference to
 *  this original vector is returned.
 *
 *  When composite has been created using a Qt list, a reference to a
 *  vector containing a copy of the original list is returned.
 */
const QVector<dtkAbstractData *> &dtkAbstractDataComposite::vector(void) const
{
    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        return d->vector;
        break;

    case dtkAbstractDataComposite::ConstVector:
        return d->const_vector;
        break;

    case dtkAbstractDataComposite::List:
        d->vector = d->list.toVector();
        return d->vector;
        break;

    case dtkAbstractDataComposite::ConstList:
        d->vector = d->const_list.toVector();
        return d->vector;
        break;

    default:
        return d->vector;
        break;
    }
}

//! Returns the data of the composite as a Qt vector.
/*!
 *  When composite has been created using a non const Qt vector, a
 *  reference to this original vector is returned.
 *
 *  When composite has been created using a const Qt vector, a
 *  reference to a copy of this original vector is returned.
 *
 *  When composite has been created using a Qt list, a reference to a
 *  vector containing a copy of the original list is returned.
 */
QVector<dtkAbstractData *> &dtkAbstractDataComposite::vector(void)
{
    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        return d->vector;
        break;

    case dtkAbstractDataComposite::ConstVector:
        d->vector = d->const_vector;
        return d->vector;
        break;

    case dtkAbstractDataComposite::List:
        d->vector = d->list.toVector();
        return d->vector;
        break;

    case dtkAbstractDataComposite::ConstList:
        d->vector = d->const_list.toVector();
        return d->vector;
        break;

    default:
        return d->vector;
        break;
    }
}

//! Returns the data of the composite as a Qt list.
/*!
 *  When composite has been created using a Qt list, a reference to
 *  this original list is returned.
 *
 *  When composite has been created using a Qt vector, a reference to a
 *  list containing a copy of the original vector is returned.
 */
const QList<dtkAbstractData *> &dtkAbstractDataComposite::list(void) const
{
    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        d->list = d->vector.toList();
        return d->list;
        break;

    case dtkAbstractDataComposite::ConstVector:
        d->list = d->const_vector.toList();
        return d->list;
        break;

    case dtkAbstractDataComposite::List:
        return d->list;
        break;

    case dtkAbstractDataComposite::ConstList:
        return d->const_list;
        break;

    default:
        return d->list;
        break;
    }
}

//! Returns the data of the composite as a Qt list.
/*!
 *  When composite has been created using a non const Qt list, a
 *  reference to this original list is returned.
 *
 *  When composite has been created using a const Qt list, a
 *  reference to a copy of this original list is returned.
 *
 *  When composite has been created using a Qt vector, a reference to a
 *  list containing a copy of the original vector is returned.
 */
QList<dtkAbstractData *> &dtkAbstractDataComposite::list(void)
{
    switch (d->type) {
    case dtkAbstractDataComposite::Vector:
        d->list = d->vector.toList();
        return d->list;
        break;

    case dtkAbstractDataComposite::ConstVector:
        d->list = d->const_vector.toList();
        return d->list;
        break;

    case dtkAbstractDataComposite::List:
        return d->list;
        break;

    case dtkAbstractDataComposite::ConstList:
        d->list = d->const_list;
        return d->list;
        break;

    default:
        return d->list;
        break;
    }
}

// /////////////////////////////////////////////////////////////////
// Private constructors
// /////////////////////////////////////////////////////////////////

//! Constructs a composite data from a pointer to a QVector.
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(QVector<dtkAbstractData *> *vector)
    : dtkAbstractData(), d(new dtkAbstractDataCompositePrivate(vector))
{
    d->type = dtkAbstractDataComposite::Vector;
}

//! Constructs a composite data from a pointer to a QList
/*!
 *
 */
dtkAbstractDataComposite::dtkAbstractDataComposite(QList<dtkAbstractData *> *list)
    : dtkAbstractData(), d(new dtkAbstractDataCompositePrivate(list))
{
    d->type = dtkAbstractDataComposite::List;
}

// /////////////////////////////////////////////////////////////////
// Static methods for creating composite by copying vector or list
// /////////////////////////////////////////////////////////////////

//! Creates a composite from a copy of \a vector.
/*!
 *
 */
dtkAbstractDataComposite *
dtkAbstractDataComposite::createFromVectorCopy(const QVector<dtkAbstractData *> &vector)
{
    return new dtkAbstractDataComposite(new QVector<dtkAbstractData *>(vector));
}

//! Creates a composite from a copy of \a vector.
/*!
 *
 */
dtkAbstractDataComposite *
dtkAbstractDataComposite::createFromVectorCopy(QVector<dtkAbstractData *> &vector)
{
    return new dtkAbstractDataComposite(new QVector<dtkAbstractData *>(vector));
}

//! Creates a composite from a copy of \a vector.
/*!
 *
 */
dtkAbstractDataComposite *
dtkAbstractDataComposite::createFromListCopy(const QList<dtkAbstractData *> &list)
{
    return new dtkAbstractDataComposite(new QList<dtkAbstractData *>(list));
}

//! Creates a composite from a copy of \a vector.
/*!
 *
 */
dtkAbstractDataComposite *
dtkAbstractDataComposite::createFromListCopy(QList<dtkAbstractData *> &list)
{
    return new dtkAbstractDataComposite(new QList<dtkAbstractData *>(list));
}

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataComposite documentation
// /////////////////////////////////////////////////////////////////

/*! \class dtkAbstractDataComposite
 *
 *  \brief Class enabling dealing with collection of dtkAbstractData*.
 *
 *
 *
 *  Example:
 *  \code
 *
 *
 *
 *  \endcode
 *
 *
 */
