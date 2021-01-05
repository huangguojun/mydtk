// Version: $Id: bfd62f8a98673b5b2ed2c111fee6e32dd8028c8b $
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

#include <QtCore>

template<typename T>
class dtkAbstractIterator;

// ///////////////////////////////////////////////////////////////////
// dtkIterator
// ///////////////////////////////////////////////////////////////////

template<typename T>
class dtkIterator
{
public:
    dtkAbstractIterator<T> *it;

public:
    typedef qlonglong difference_type;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

public:
    explicit dtkIterator(dtkAbstractIterator<T> *iterator = 0);

public:
    dtkIterator(const dtkIterator &o);
    dtkIterator(dtkIterator &&o);
    ~dtkIterator(void);

public:
    dtkIterator &operator=(const dtkIterator &o);
    dtkIterator &operator=(dtkIterator &&o);

public:
    bool operator==(const dtkIterator &o) const;
    bool operator!=(const dtkIterator &o) const;
    bool operator<(const dtkIterator &o) const;
    bool operator<=(const dtkIterator &o) const;
    bool operator>(const dtkIterator &o) const;
    bool operator>=(const dtkIterator &o) const;

public:
    T &operator*(void)const;
    T *operator->(void)const;
    T &operator[](qlonglong j) const;

public:
    dtkIterator &operator++(void);
    dtkIterator operator++(int);
    dtkIterator &operator--(void);
    dtkIterator operator--(int);
    dtkIterator &operator+=(qlonglong j);
    dtkIterator &operator-=(qlonglong j);
    dtkIterator operator+(qlonglong j) const;
    dtkIterator operator-(qlonglong j) const;

public:
    operator T *() const;
};

// ///////////////////////////////////////////////////////////////////
// dtkAbstractIterator
// ///////////////////////////////////////////////////////////////////

template<typename T>
class dtkAbstractIterator
{
public:
    virtual ~dtkAbstractIterator(void) { ; }

public:
    virtual dtkAbstractIterator *clone(void) const = 0;
    virtual void copy(const dtkAbstractIterator &o) = 0;

public:
    virtual bool equal(const dtkAbstractIterator &o) const = 0;
    virtual bool lowerThan(const dtkAbstractIterator &o) const = 0;
    virtual bool greaterThan(const dtkAbstractIterator &o) const = 0;

public:
    virtual T &ref(void) const = 0;

public:
    virtual void advance(void) = 0;
    virtual void rewind(void) = 0;
    virtual void advance(qlonglong j) = 0;
    virtual void rewind(qlonglong j) = 0;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template<typename T>
inline dtkIterator<T>::dtkIterator(dtkAbstractIterator<T> *iterator) : it(iterator)
{
}

template<typename T>
inline dtkIterator<T>::dtkIterator(const dtkIterator &o) : it(o.it->clone())
{
}

template<typename T>
inline dtkIterator<T>::dtkIterator(dtkIterator &&o) : it(o.it)
{
    o.it = 0;
}

template<typename T>
inline dtkIterator<T>::~dtkIterator(void)
{
    if (it)
        delete it;

    it = 0;
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator=(const dtkIterator &o)
{
    it->copy(*(o.it));
    return *this;
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator=(dtkIterator &&o)
{
    std::swap(it, o.it);
    return *this;
}

template<typename T>
inline bool dtkIterator<T>::operator==(const dtkIterator &o) const
{
    return it->equal(*(o.it));
}

template<typename T>
inline bool dtkIterator<T>::operator!=(const dtkIterator &o) const
{
    return !(it->equal(*(o.it)));
}

template<typename T>
inline bool dtkIterator<T>::operator<(const dtkIterator &o) const
{
    return it->lowerThan(*(o.it));
}

template<typename T>
inline bool dtkIterator<T>::operator<=(const dtkIterator &o) const
{
    return (it->lowerThan(*(o.it)) || it->equal(*(o.it)));
}

template<typename T>
inline bool dtkIterator<T>::operator>(const dtkIterator &o) const
{
    return it->greaterThan(*(o.it));
}

template<typename T>
inline bool dtkIterator<T>::operator>=(const dtkIterator &o) const
{
    return (it->greaterThan(*(o.it)) || it->equal(*(o.it)));
}

template<typename T>
inline T &dtkIterator<T>::operator*(void)const
{
    return it->ref();
}

template<typename T>
inline T *dtkIterator<T>::operator->(void)const
{
    return &(it->ref());
}

template<typename T>
inline T &dtkIterator<T>::operator[](qlonglong j) const
{
    return *(*this + j);
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator++(void)
{
    it->advance();
    return *this;
}

template<typename T>
inline dtkIterator<T> dtkIterator<T>::operator++(int)
{
    dtkIterator o(*this);
    it->advance();
    return o;
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator--(void)
{
    it->rewind();
    return *this;
}

template<typename T>
inline dtkIterator<T> dtkIterator<T>::operator--(int)
{
    dtkIterator o(*this);
    it->rewind();
    return o;
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator+=(qlonglong j)
{
    it->advance(j);
    return *this;
}

template<typename T>
inline dtkIterator<T> &dtkIterator<T>::operator-=(qlonglong j)
{
    it->rewind(j);
    return *this;
}

template<typename T>
inline dtkIterator<T> dtkIterator<T>::operator+(qlonglong j) const
{
    dtkIterator o(*this);
    o += j;
    return o;
}

template<typename T>
inline dtkIterator<T> dtkIterator<T>::operator-(qlonglong j) const
{
    dtkIterator o(*this);
    o -= j;
    return o;
}

template<typename T>
inline dtkIterator<T>::operator T *() const
{
    return &(it->ref());
}

// ///////////////////////////////////////////////////////////////////
// dtkIteratorBase CRTP
// ///////////////////////////////////////////////////////////////////

template<typename T, typename Iterator>
class dtkIteratorBase : public dtkAbstractIterator<T>
{
public:
    dtkIteratorBase *clone(void) const;
    void copy(const dtkAbstractIterator<T> &o);

public:
    bool equal(const dtkAbstractIterator<T> &o) const;
    bool lowerThan(const dtkAbstractIterator<T> &o) const;
    bool greaterThan(const dtkAbstractIterator<T> &o) const;

public:
    T &ref(void) const;

public:
    void advance(void);
    void rewind(void);
    void advance(qlonglong j);
    void rewind(qlonglong j);
};

// ///////////////////////////////////////////////////////////////////

template<typename T, typename Iterator>
inline dtkIteratorBase<T, Iterator> *dtkIteratorBase<T, Iterator>::clone(void) const
{
    return new Iterator(static_cast<const Iterator &>(*this));
}

template<typename T, typename Iterator>
inline void dtkIteratorBase<T, Iterator>::copy(const dtkAbstractIterator<T> &o)
{
    static_cast<Iterator &>(*this) = static_cast<const Iterator &>(o);
}

template<typename T, typename Iterator>
inline bool dtkIteratorBase<T, Iterator>::equal(const dtkAbstractIterator<T> &o) const
{
    return (static_cast<const Iterator &>(*this) == static_cast<const Iterator &>(o));
}

template<typename T, typename Iterator>
inline bool dtkIteratorBase<T, Iterator>::lowerThan(const dtkAbstractIterator<T> &o) const
{
    return (static_cast<const Iterator &>(*this) < static_cast<const Iterator &>(o));
}

template<typename T, typename Iterator>
inline bool dtkIteratorBase<T, Iterator>::greaterThan(const dtkAbstractIterator<T> &o) const
{
    return (static_cast<const Iterator &>(*this) > static_cast<const Iterator &>(o));
}

template<typename T, typename Iterator>
inline T &dtkIteratorBase<T, Iterator>::ref(void) const
{
    return *(static_cast<const Iterator &>(*this));
}

template<typename T, typename Iterator>
inline void dtkIteratorBase<T, Iterator>::advance(void)
{
    ++(static_cast<Iterator &>(*this));
}

template<typename T, typename Iterator>
inline void dtkIteratorBase<T, Iterator>::advance(qlonglong j)
{
    static_cast<Iterator &>(*this) += j;
}

template<typename T, typename Iterator>
inline void dtkIteratorBase<T, Iterator>::rewind(void)
{
    --(static_cast<Iterator &>(*this));
}

template<typename T, typename Iterator>
inline void dtkIteratorBase<T, Iterator>::rewind(qlonglong j)
{
    static_cast<Iterator &>(*this) -= j;
}

// ///////////////////////////////////////////////////////////////////
// dtkIteratorGeneric
// ///////////////////////////////////////////////////////////////////

template<typename Iterator>
class dtkIteratorGeneric
    : public dtkIteratorBase<typename Iterator::value_type, dtkIteratorGeneric<Iterator>>
{
    Iterator it;

public:
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;

public:
    dtkIteratorGeneric(const Iterator ite) : it(ite) {}
    dtkIteratorGeneric(const dtkIteratorGeneric &o) : it(o.it) {}

public:
    dtkIteratorGeneric &operator=(const dtkIteratorGeneric &o)
    {
        it = o.it;
        return *this;
    }

public:
    bool operator==(const dtkIteratorGeneric &o) const { return it == o.it; }
    bool operator!=(const dtkIteratorGeneric &o) const { return it != o.it; }
    bool operator<(const dtkIteratorGeneric &o) const { return it < o.it; }
    bool operator<=(const dtkIteratorGeneric &o) const { return it <= o.it; }
    bool operator>(const dtkIteratorGeneric &o) const { return it > o.it; }
    bool operator>=(const dtkIteratorGeneric &o) const { return it >= o.it; }

public:
    reference operator*(void)const { return *it; }
    pointer operator->(void)const { return &(*it); }
    reference operator[](qlonglong j) const { return it[j]; }

public:
    dtkIteratorGeneric &operator++(void)
    {
        ++it;
        return *this;
    }
    dtkIteratorGeneric operator++(int)
    {
        dtkIteratorGeneric o(it);
        ++it;
        return o;
    }
    dtkIteratorGeneric &operator--(void)
    {
        --it;
        return *this;
    }
    dtkIteratorGeneric operator--(int)
    {
        dtkIteratorGeneric o(it);
        --it;
        return o;
    }
    dtkIteratorGeneric &operator+=(qlonglong j)
    {
        it += j;
        return *this;
    }
    dtkIteratorGeneric &operator-=(qlonglong j)
    {
        it -= j;
        return *this;
    }
    dtkIteratorGeneric operator+(qlonglong j) const { return dtkIteratorGeneric(it + j); }
    dtkIteratorGeneric operator-(qlonglong j) const { return dtkIteratorGeneric(it - j); }

public:
    operator pointer() const { return &(*it); }
};
//
// dtkIterator.h ends here
