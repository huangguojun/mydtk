/* dtkAbstractDataComposite.tpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 * Created: Thu Sep 22 14:34:08 2011 (+0200)
 * Version: $Id: 0a373e61417d5dbf8182ab69f82f3bf0c7a16418 $
 * Last-Updated: mar. févr.  4 11:05:04 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 192
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATACOMPOSITE_TPP
#define DTKABSTRACTDATACOMPOSITE_TPP

// /////////////////////////////////////////////////////////////////
// Related non-member templated functions
// /////////////////////////////////////////////////////////////////

//! Casts const input \a vector of T into a const vector of
//! dtkAbstractData*, provided that dynamic casting from
//! T to dtkAbstractData* is valid.
/*!
 *  When input vector is empty or when dynamic cast from T to
 *  dtkAbstractData* fails, an empty vector is returned.
 */
template <typename T> const QVector<dtkAbstractData *>& dtkUpCastIntoDataVector(const QVector<T>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Casting failed. Returns empty vector.";
        return *(new QVector<dtkAbstractData *>());
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(vector.at(0));

    if (!data) {
        dtkDebug() << "Input vector is not made of dtkAbstractData*. Casting failed. Returns empty vector.";
        return *(new QVector<dtkAbstractData *>());
    }

    return *(static_cast<const QVector<dtkAbstractData *> *>((void *)(&vector)));
}

//! Casts input \a vector of T into a vector of dtkAbstractData*,
//! provided that dynamic casting from T to dtkAbstractData* is valid.
/*!
 *  When input vector is empty or when dynamic cast from T to
 *  dtkAbstractData* fails, an empty vector is returned.
 */
template <typename T> QVector<dtkAbstractData *>& dtkUpCastIntoDataVector(QVector<T>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Casting failed. Returns empty vector.";
        return *(new QVector<dtkAbstractData *>());
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(vector.at(0));

    if (!data) {
        dtkDebug() << "Input vector is not made of dtkAbstractData*. Casting failed. Returns empty vector.";
        return *(new QVector<dtkAbstractData *>());
    }

    return *(static_cast<QVector<dtkAbstractData *> *>((void *)(&vector)));
}

//! Casts const input \a list of T into a const list of
//! dtkAbstractData*, provided that dynamic casting from
//! T to dtkAbstractData* is valid.
/*!
 *  When input vector is empty or when dynamic cast from T to
 *  dtkAbstractData* fails, an empty list is returned.
 */
template <typename T> const QList<dtkAbstractData *>& dtkUpCastIntoDataList(const QList<T>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Casting failed. Returns empty list.";
        return *(new QList<dtkAbstractData *>());
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(list.at(0));

    if (!data) {
        dtkDebug() << "Input list is not made of dtkAbstractData*. Casting failed. Returns empty list.";
        return *(new QList<dtkAbstractData *>());
    }

    return *(static_cast<const QList<dtkAbstractData *> *>((void *)(&list)));
}

//! Casts input \a list of T into a list of dtkAbstractData*, provided
//! that dynamic casting from T to dtkAbstractData* is valid.
/*!
 *  When input vector is empty or when dynamic cast from T to
 *  dtkAbstractData* fails, an empty list is returned.
 */
template <typename T> QList<dtkAbstractData *>& dtkUpCastIntoDataList(QList<T>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Casting failed. Returns empty list.";
        return *(new QList<dtkAbstractData *>());
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(list.at(0));

    if (!data) {
        dtkDebug() << "Input list is not made of dtkAbstractData*. Casting failed. Returns empty list.";
        return *(new QList<dtkAbstractData *>());
    }

    return *(static_cast<QList<dtkAbstractData *> *>((void *)(&list)));
}

//! Casts const input \a vector of dtkAbstractData* into a const
//! vector of type T, provided that dynamic casting from
//! dtkAbstractData* to T is valid.
/*!
 *  When input vector is empty or when dynamic cast from
 *  dtkAbstractData* to T fails, an empty vector is returned.
 */
template <typename T> const QVector<T>& dtkDownCastFromDataVector(const QVector<dtkAbstractData *>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Casting failed. Returns empty vector.";
        return *(new QVector<T>());
    }

    T data = dynamic_cast<T>(vector.at(0));

    if (!data) {
        dtkDebug() << "Output vector is not made of dtkAbstractData*. Casting failed. Returns empty vector.";
        return *(new QVector<T>());
    }

    return *(static_cast<const QVector<T> *>((void *)(&vector)));
}

//! Casts input \a vector of dtkAbstractData* into a vector of type T,
//! provided that dynamic casting from dtkAbstractData* to T is valid.
/*!
 *  When input vector is empty or when dynamic cast from
 *  dtkAbstractData* to T fails, an empty vector is returned.
 */
template <typename T> QVector<T>& dtkDownCastFromDataVector(QVector<dtkAbstractData *>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Casting failed. Returns empty vector.";
        return *(new QVector<T>());
    }

    T data = qobject_cast<T>(vector.at(0));

    if (!data) {
        dtkDebug() << "Output vector is not made of dtkAbstractData*. Casting failed. Returns empty vector.";
        return *(new QVector<T>());
    }

    return *(static_cast<QVector<T> *>((void *)(&vector)));
}

//! Casts const input \a list of dtkAbstractData* into a const
//! list of type T, provided that dynamic casting from
//! dtkAbstractData* to T is valid.
/*!
 *  When input list is empty or when dynamic cast from
 *  dtkAbstractData* to T fails, an empty list is returned.
 */
template <typename T> const QList<T>& dtkDownCastFromDataList(const QList<dtkAbstractData *>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Casting failed. Returns empty list.";
        return *(new QList<T>());
    }

    T data = qobject_cast<T>(list.at(0));

    if (!data) {
        dtkDebug() << "Output list is not made of dtkAbstractData*. Casting failed. Returns empty list.";
        return *(new QList<T>());
    }

    return *(static_cast<const QList<T> *>((void *)(&list)));
}

//! Casts input \a list of dtkAbstractData* into a list of type T,
//! provided that dynamic casting from dtkAbstractData* to T is valid.
/*!
 *  When input list is empty or when dynamic cast from
 *  dtkAbstractData* to T fails, an empty list is returned.
 */
template <typename T> QList<T>& dtkDownCastFromDataList(QList<dtkAbstractData *>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Casting failed. Returns empty list.";
        return *(new QList<T>());
    }

    T data = qobject_cast<T>(list.at(0));

    if (!data) {
        dtkDebug() << "Output list is not made of dtkAbstractData*. Casting failed. Returns empty list.";
        return *(new QList<T>());
    }

    return *(static_cast<QList<T> *>((void *)(&list)));
}

//! Creates a pointer to a composite data from a copy of \a vector.
/*!
 *  Returns NULL when input type T cannot be casted into
 *  dtkAbstractData* or when input vector is empty.
 */
template <typename T> dtkAbstractDataComposite *dtkAbstractDataCompositeFromVectorCopy(const QVector<T>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Creation failed. Returns empty composite.";
        return NULL;
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(vector.at(0));

    if (!data) {
        dtkDebug() << "Input vector is not made of dtkAbstractData*. Creation failed. Returns empty composite.";
        return NULL;
    }

    return dtkAbstractDataComposite::createFromVectorCopy(dtkUpCastIntoDataVector(vector));
}

//! Creates a pointer to a composite data from a copy of \a vector.
/*!
 *  Returns NULL when input type T cannot be casted into
 *  dtkAbstractData* or when input vector is empty.
 */
template <typename T> dtkAbstractDataComposite *dtkAbstractDataCompositeFromVectorCopy(QVector<T>& vector)
{
    if (vector.isEmpty()) {
        dtkDebug() << "Input vector is empty. Creation failed. Returns empty composite.";
        return NULL;
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(vector.at(0));

    if (!data) {
        dtkDebug() << "Input vector is not made of dtkAbstractData*. Creation failed. Returns empty composite.";
        return NULL;
    }

    return dtkAbstractDataComposite::createFromVectorCopy(dtkUpCastIntoDataVector(vector));
}

//! Creates a pointer to a composite data from a copy of \a list.
/*!
 *  Returns NULL when input type T cannot be casted into
 *  dtkAbstractData* or when input list is empty.
 */
template <typename T> dtkAbstractDataComposite *dtkAbstractDataCompositeFromListCopy(const QList<T>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Creation failed. Returns empty composite.";
        return NULL;
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(list.at(0));

    if (!data) {
        dtkDebug() << "Input list is not made of dtkAbstractData*. Creation failed. Returns empty composite.";
        return NULL;
    }

    return dtkAbstractDataComposite::createFromListCopy(dtkUpCastIntoDataVector(list));
}

//! Creates a pointer to a composite data from a copy of \a list.
/*!
 *  Returns NULL when input type T cannot be casted into
 *  dtkAbstractData* or when input list is empty.
 */
template <typename T> dtkAbstractDataComposite *dtkAbstractDataCompositeFromListCopy(QList<T>& list)
{
    if (list.isEmpty()) {
        dtkDebug() << "Input list is empty. Creation failed. Returns empty composite.";
        return NULL;
    }

    dtkAbstractData *data = qobject_cast<dtkAbstractData *>(list.at(0));

    if (!data) {
        dtkDebug() << "Input list is not made of dtkAbstractData*. Creation failed. Returns empty composite.";
        return NULL;
    }

    return dtkAbstractDataComposite::createFromListCopy(dtkUpCastIntoDataVector(list));
}

#endif
