// Version: $Id: 4fcd431e1ce3bade804d7f798894daa40a33d785 $
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

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequential::item implementation
// /////////////////////////////////////////////////////////////////

inline dtkMetaContainerSequential::item::item(HandlerIterator *iterator) : it(iterator)
{

}

inline dtkMetaContainerSequential::item::item(item&& o) : it(NULL)
{
    std::swap(it, o.it);
}

inline dtkMetaContainerSequential::item::~item()
{
    if (it)
        delete it;

    it = NULL;
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator = (const item& o)
{
    if (this != &o) {
        it->copy(*(o.it));
    }

    return *this;
}
inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator = (item&& o)
{
    std::swap(it, o.it);
    return *this;
}

inline bool dtkMetaContainerSequential::item::operator == (const item& o) const
{
    return  it->equal(*(o.it));
}

inline bool dtkMetaContainerSequential::item::operator != (const item& o) const
{
    return !it->equal(*(o.it));
}

inline const QVariant dtkMetaContainerSequential::item::value(void) const
{
    return it->variant();
}

template<typename T> inline const T& dtkMetaContainerSequential::item::value(void) const
{
    return *static_cast<const T *>(it->value());
}

template<typename T> inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator = (const T& t)
{
    it->assign(&t);
    return *this;
}

template<typename T> inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator += (const T& t)
{
    it->addAssign(&t);
    return *this;
}

template<typename T> inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator -= (const T& t)
{
    it->subAssign(&t);
    return *this;
}

template<typename T> inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator *= (const T& t)
{
    it->mulAssign(&t);
    return *this;
}

template<typename T> inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::item::operator /= (const T& t)
{
    it->divAssign(&t);
    return *this;
}

template<typename T> inline bool dtkMetaContainerSequential::item::operator == (const T& t) const
{
    return it->equalToValue(&t);
}

template<typename T> inline bool dtkMetaContainerSequential::item::operator != (const T& t) const
{
    return !it->equalToValue(&t);
}

// /////////////////////////////////////////////////////////////////

inline QDebug& operator << (QDebug debug, const dtkMetaContainerSequential::item& item)
{
    const bool oldSetting = debug.autoInsertSpaces();
    debug.nospace() << item.value();
    debug.setAutoInsertSpaces(oldSetting);
    return debug.maybeSpace();
}

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequential::iterator implementation
// /////////////////////////////////////////////////////////////////

inline dtkMetaContainerSequential::iterator::iterator(HandlerIterator *iterator) : proxy(iterator)
{
}

inline dtkMetaContainerSequential::iterator::iterator(const iterator& o) : proxy(o.proxy.it)
{
}

inline dtkMetaContainerSequential::iterator::iterator(iterator&& o) : proxy(o.proxy.it)
{
    o.proxy.it = NULL;
}

inline dtkMetaContainerSequential::iterator::~iterator(void)
{
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator = (const iterator& o)
{
    proxy = o.proxy;
    return *this;
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator = (iterator&& o)
{
    std::swap(proxy.it, o.proxy.it);
    return *this;
}

inline bool dtkMetaContainerSequential::iterator::operator == (const iterator& o) const
{
    return proxy == o.proxy;
}

inline bool dtkMetaContainerSequential::iterator::operator != (const iterator& o) const
{
    return proxy != o.proxy;
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::iterator::operator * (void)
{
    return proxy;
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::iterator::operator [] (qlonglong j)
{
    return *(*this + j);
}

template <typename T> inline T& dtkMetaContainerSequential::iterator::value(void)
{
    return *static_cast<T *>(proxy.it->value());
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator ++ (void)
{
    proxy.it->advance();
    return *this;
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::iterator::operator ++ (int)
{
    iterator o(*this);
    proxy.it->advance();
    return o;
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator -- (void)
{
    proxy.it->moveBackward(static_cast<qlonglong>(1));
    return *this;
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::iterator::operator -- (int)
{
    iterator o(*this);
    proxy.it->moveBackward(static_cast<qlonglong>(1));
    return o;
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator += (qlonglong j)
{
    proxy.it->moveForward(j);
    return *this;
}

inline dtkMetaContainerSequential::iterator& dtkMetaContainerSequential::iterator::operator -= (qlonglong j)
{
    proxy.it->moveBackward(j);
    return *this;
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::iterator::operator + (qlonglong j) const
{
    iterator o(*this);
    o += j;
    return o;
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::iterator::operator - (qlonglong j) const
{
    iterator o(*this);
    o -= j;
    return o;
}

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequential::const_iterator implementation
// /////////////////////////////////////////////////////////////////

inline dtkMetaContainerSequential::const_iterator::const_iterator(HandlerConstIterator *iterator) : it(iterator)
{
}

inline dtkMetaContainerSequential::const_iterator::const_iterator(const const_iterator& o) : it(o.it->clone())
{
}

inline dtkMetaContainerSequential::const_iterator::const_iterator(const_iterator&& o) : it(o.it)
{
    o.it = NULL;
}

inline dtkMetaContainerSequential::const_iterator::~const_iterator(void)
{
    if (it)
        delete it;

    it = NULL;
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator = (const const_iterator& o)
{
    if (this != &o)
        it->copy(*(o.it));

    return *this;
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator = (const_iterator&& o)
{
    std::swap(it, o.it);
    return *this;
}

inline bool dtkMetaContainerSequential::const_iterator::operator == (const const_iterator& o) const
{
    return it->equal(*(o.it));
}

inline bool dtkMetaContainerSequential::const_iterator::operator != (const const_iterator& o) const
{
    return !it->equal(*(o.it));
}

inline QVariant dtkMetaContainerSequential::const_iterator::operator * (void) const
{
    return it->variant();
}

inline QVariant dtkMetaContainerSequential::const_iterator::operator [] (qlonglong j) const
{
    return *(*this + j);
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator ++ (void)
{
    it->advance(); return *this;
    return *this;
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::const_iterator::operator ++ (int)
{
    const_iterator o(*this);
    it->advance();
    return o;
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator -- (void)
{
    it->moveBackward(static_cast<qlonglong>(1));
    return *this;
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::const_iterator::operator -- (int)
{
    const_iterator o(*this);
    it->moveBackward(static_cast<qlonglong>(1));
    return o;
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator += (qlonglong j)
{
    it->moveForward(j);
    return *this;
}

inline dtkMetaContainerSequential::const_iterator& dtkMetaContainerSequential::const_iterator::operator -= (qlonglong j)
{
    it->moveBackward(j);
    return *this;
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::const_iterator::operator + (qlonglong j) const
{
    const_iterator o(*this);
    o += j;
    return o;
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::const_iterator::operator - (qlonglong j) const
{
    const_iterator o(*this);
    o -= j;
    return o;
}

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequential implementation
// /////////////////////////////////////////////////////////////////

inline dtkMetaContainerSequential::dtkMetaContainerSequential(dtkMetaContainerSequentialHandler *handler) : h(handler), proxy(new item(NULL))
{
    if (handler) {
        proxy->it = handler->begin();
    }
}

inline dtkMetaContainerSequential::dtkMetaContainerSequential(dtkMetaContainerSequential&& o) : h(o.h), proxy(new item(NULL))
{
    if (h) {
        proxy->it = h->begin();
    }
    o.h = nullptr;
}

inline dtkMetaContainerSequential::~dtkMetaContainerSequential(void)
{
    if (h)
        delete h;

    h = NULL;
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::begin(void)
{
    return iterator(h->begin());
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::begin(void) const
{
    return const_iterator(h->cbegin());
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::cbegin(void) const
{
    return const_iterator(h->cbegin());
}

inline dtkMetaContainerSequential::iterator dtkMetaContainerSequential::end(void)
{
    return iterator(h->end());
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::end(void) const
{
    return const_iterator(h->cend());
}

inline dtkMetaContainerSequential::const_iterator dtkMetaContainerSequential::cend(void) const
{
    return const_iterator(h->cend());
}

inline bool dtkMetaContainerSequential::hasBiDirectionalIterator(void) const
{
    return h->hasBiDirectionalIterator();
}

inline bool dtkMetaContainerSequential::hasRandomAccessIterator(void) const
{
    return h->hasRandomAccessIterator();
}

inline bool dtkMetaContainerSequential::empty(void) const
{
    return h->empty();
}

inline qlonglong dtkMetaContainerSequential::size(void) const
{
    return h->size();
}

inline void dtkMetaContainerSequential::clear(void)
{
    h->clear();
}

inline void dtkMetaContainerSequential::reserve(qlonglong size)
{
    h->reserve(size);
}

inline void dtkMetaContainerSequential::resize(qlonglong size)
{
    h->resize(size);
}

template <typename T> inline void dtkMetaContainerSequential::append(const T& t)
{
    h->append(&t);
}

template <typename T> inline void dtkMetaContainerSequential::prepend(const T& t)
{
    h->prepend(&t);
}

inline void dtkMetaContainerSequential::append(const QVariant& v)
{
    this->insert(h->size(), v);
}

inline void dtkMetaContainerSequential::prepend(const QVariant& v)
{
    this->insert(0, v);
}

template <typename T> inline void dtkMetaContainerSequential::insert(qlonglong idx, const T& t)
{
    h->insert(idx, &t);
}

template <typename T> inline void dtkMetaContainerSequential::setAt(qlonglong idx, const T& t)
{
    h->setAt(idx, &t);
}

template <typename T> inline const T& dtkMetaContainerSequential::at(qlonglong idx) const
{
    return *static_cast<const T *>(h->at(idx));
}

inline const QVariant& dtkMetaContainerSequential::at(qlonglong idx) const
{
    return h->variantAt(idx, var);
}

inline void dtkMetaContainerSequential::removeAt(qlonglong idx)
{
    h->removeAt(idx);
}

inline const QVariant& dtkMetaContainerSequential::first(void) const
{
    return h->variantAt(0, var);
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::first(void)
{
    h->iteratorAt(0, proxy->it);
    return *proxy;
}

inline const QVariant& dtkMetaContainerSequential::last(void) const
{
    return h->variantAt(h->size() - 1, var);
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::last(void)
{
    h->iteratorAt(h->size() - 1, proxy->it);
    return *proxy;
}

inline const QVariant& dtkMetaContainerSequential::operator [] (qlonglong idx) const
{
    return h->variantAt(idx, var);
}

inline dtkMetaContainerSequential::item& dtkMetaContainerSequential::operator [] (qlonglong idx)
{
    h->iteratorAt(idx, proxy->it);
    return *proxy;
}

// ///////////////////////////////////////////////////////////////////
// Debug operator
// ///////////////////////////////////////////////////////////////////

inline QDebug& operator << (QDebug debug, const dtkMetaContainerSequential& container)
{
    debug << qPrintable(container.h->description());
    return debug.maybeSpace();
}

// /////////////////////////////////////////////////////////////////
// Specialization of Qt internal struct to build QVariant of dtkMetaContainerSequential
// /////////////////////////////////////////////////////////////////

namespace QtPrivate {
    template <> struct QVariantValueHelperInterface<dtkMetaContainerSequential> {
        static dtkMetaContainerSequential invoke(const QVariant& v) {
            return dtkMetaContainerSequential(v.value<dtkMetaContainerSequentialHandler *>());
        }
    };
}

// /////////////////////////////////////////////////////////////////
// Specialization of dtkMetaTypeHandler for dtkMetaContainerSequential
// /////////////////////////////////////////////////////////////////

template <> struct dtkMetaTypeHandler <dtkMetaContainerSequential> {
    static bool canConvert(const QList<int>& types) {
        int to = qMetaTypeId<dtkMetaContainerSequentialHandler *>();

        for (int from : types) {
            if (!QMetaType::hasRegisteredConverterFunction(from, to)) {
                return false;
            }
        }

        return true;
    }
};

// /////////////////////////////////////////////////////////////////
// Specialization of QVariant::canConvert<T>() method
// /////////////////////////////////////////////////////////////////

template <> inline bool QVariant::canConvert<dtkMetaContainerSequential>(void) const
{
    int from = d.type;
    int to = qMetaTypeId<dtkMetaContainerSequentialHandler *>();
    return QMetaType::hasRegisteredConverterFunction(from, to);
}

//
// dtkMetaContainerSequential.tpp ends here
