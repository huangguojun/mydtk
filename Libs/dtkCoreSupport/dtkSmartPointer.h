/* dtkSmartPointer.h ---
 *
 * Author: John Stark
 * Copyright (C) 2011 - John Stark, Inria.
 * Created: Wed May 25 14:00:00 2011 (+0100)
 * Version: $Id: bdc34d1a33b18cca88606cb2e9e34476fadc7588 $
 * Last-Updated: Wed Aug  3 09:00:13 2011 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 55
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKSMARTPOINTER_H
#define DTKSMARTPOINTER_H

#include <QtCore/QHash>

template<class T>
class dtkSmartPointer
{
public:
    typedef T ObjectType;

    dtkSmartPointer(void) : d(0) {}

    dtkSmartPointer(const dtkSmartPointer<T> &p) : d(p.d) { this->retain(); }

    template<typename TR>
    dtkSmartPointer(const dtkSmartPointer<TR> &p) : d(dynamic_cast<T *>(p.operator->()))
    {
        this->retain();
    }

    dtkSmartPointer(T *p) : d(p) { this->retain(); }

    ~dtkSmartPointer(void) { this->release(); }

    T *operator->(void)const { return d; }

    operator T *(void)const { return d; }

    bool isNull(void) const { return d == 0; }

    int refCount(void) const
    {
        if (d)
            return d->count();
        else
            return 0;
    }

    template<typename TR>
    bool operator==(TR r) const
    {
        return (d == static_cast<const T *>(r));
    }

    template<typename TR>
    bool operator==(const dtkSmartPointer<TR> &r) const
    {
        return (d == static_cast<const T *>(r.constData()));
    }

    template<typename TR>
    bool operator!=(TR r) const
    {
        return (d != static_cast<const T *>(r));
    }

    template<typename TR>
    bool operator!=(const dtkSmartPointer<TR> &r) const
    {
        return (d != static_cast<const T *>(r.constData()));
    }

    bool operator==(const dtkSmartPointer &r) const { return d == r.d; }

    bool operator!=(const dtkSmartPointer &r) const { return d != r.d; }

    bool operator<(const dtkSmartPointer &r) const { return d < r.d; }

    bool operator<=(const dtkSmartPointer &r) const { return d <= r.d; }

    bool operator>(const dtkSmartPointer &r) const { return d > r.d; }

    bool operator>=(const dtkSmartPointer &r) const { return d >= r.d; }

    T *data(void) { return d; }

    const T *constData(void) const { return d; }

    T &operator*(void) { return *d; }

    const T &operator*(void)const { return *d; }

    dtkSmartPointer &operator=(const dtkSmartPointer &r) { return this->operator=(r.d); }

    template<typename TR>
    dtkSmartPointer &operator=(const dtkSmartPointer<TR> &r)
    {
        return this->operator=(dynamic_cast<T *>(r.operator->()));
    }

    dtkSmartPointer &operator=(T *r)
    {
        if (d != r) {
            T *tmp = d;
            d = r;

            this->retain();

            if (tmp)
                tmp->release();
        }

        return *this;
    }

    void swap(dtkSmartPointer &other)
    {
        T *tmp = d;
        d = other.d;
        other.d = tmp;
    }

    dtkSmartPointer &takePointer(T *r)
    {
        if (d != r) {
            T *tmp = d;
            d = r;

            if (tmp)
                tmp->release();
        }

        return *this;
    }

    T *releasePointer(void)
    {
        T *tmp = d;
        d = 0;
        return tmp;
    }

private:
    void retain(void)
    {
        if (d)
            d->retain();
    }

    void release(void)
    {
        if (d)
            d->release();
    }

private:
    T *d;
};

template<class T>
inline uint qHash(const dtkSmartPointer<T> &key)
{
    return qHash(key.constData());
}

#endif
