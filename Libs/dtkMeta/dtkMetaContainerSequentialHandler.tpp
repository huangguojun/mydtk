// Version: $Id: 50b03aac8985c04920b0c92c7eba5500acd88477 $
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

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerTemplate::iterator
// ///////////////////////////////////////////////////////////////////

template < typename T > inline dtkMetaContainerSequentialHandlerTemplate<T>::iterator::iterator(const Iterator& iterator) : it(iterator)
{

}

template < typename T > inline dtkMetaContainerSequentialHandler::iterator *dtkMetaContainerSequentialHandlerTemplate<T>::iterator::clone(void) const
{
    return new iterator(it);
}

template < typename T > inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::copy(const dtkMetaContainerSequentialHandler::iterator& o)
{
    it = static_cast<const iterator&>(o).it;
}

template < typename T > inline bool dtkMetaContainerSequentialHandlerTemplate<T>::iterator::equal(const dtkMetaContainerSequentialHandler::iterator& o) const
{
    return it == static_cast<const iterator&>(o).it;
}

template < typename T > inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::advance(void)
{
    ++it;
}

template < typename T > inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::moveForward(qlonglong step)
{
    std::advance(it, step);
}

template < typename T > inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::moveBackward(qlonglong step)
{
    std::advance(it, -step);
}

template < typename T > inline void *dtkMetaContainerSequentialHandlerTemplate<T>::iterator::value(void) const
{
    return &(*it);
}

template < typename T > inline QVariant dtkMetaContainerSequentialHandlerTemplate<T>::iterator::variant(void) const
{
    return QVariant(qMetaTypeId<ValueType>(), &(*it), QTypeInfo<ValueType>::isPointer);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::assign(const void *t)
{
    dtkMetaContainerSequentialIteratorHelper<ValueType>::assign(*it, *static_cast<const ValueType *>(t));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::addAssign(const void *t)
{
    dtkMetaContainerSequentialIteratorHelper<ValueType>::addAssign(*it, *static_cast<const ValueType *>(t));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::subAssign(const void *t)
{
    dtkMetaContainerSequentialIteratorHelper<ValueType>::subAssign(*it, *static_cast<const ValueType *>(t));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::mulAssign(const void *t)
{
    dtkMetaContainerSequentialIteratorHelper<ValueType>::mulAssign(*it, *static_cast<const ValueType *>(t));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::iterator::divAssign(const void *t)
{
    dtkMetaContainerSequentialIteratorHelper<ValueType>::divAssign(*it, *static_cast<const ValueType *>(t));
}

template <typename T> inline bool dtkMetaContainerSequentialHandlerTemplate<T>::iterator::equalToValue(const void *t) const
{
    return (*it == *static_cast<const ValueType *>(t));
}

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerTemplate::const_iterator
// ///////////////////////////////////////////////////////////////////

template <typename T> inline dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::const_iterator(const ConstIterator& iterator) : it(iterator)
{

}

template <typename T> inline dtkMetaContainerSequentialHandler::const_iterator *dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::clone(void) const
{
    return new const_iterator(it);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::copy(const dtkMetaContainerSequentialHandler::const_iterator& o)
{
    it = static_cast<const const_iterator&>(o).it;
}

template <typename T> inline bool dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::equal(const dtkMetaContainerSequentialHandler::const_iterator& o) const
{
    return it == static_cast<const const_iterator&>(o).it;
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::advance(void)
{
    ++it;
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::moveForward(qlonglong step)
{
    std::advance(it, step);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::moveBackward(qlonglong step)
{
    std::advance(it, -step);
}

template < typename T > inline const void *dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::value(void) const
{
    return &(*it);
}

template <typename T> inline QVariant dtkMetaContainerSequentialHandlerTemplate<T>::const_iterator::variant(void) const
{
    return QVariant(qMetaTypeId<ValueType>(), &(*it), QTypeInfo<ValueType>::isPointer);
}

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerTemplate
// ///////////////////////////////////////////////////////////////////

template <typename T> inline dtkMetaContainerSequentialHandler::iterator *dtkMetaContainerSequentialHandlerTemplate<T>::begin(void)
{
    return new iterator(m_container->begin());
}

template <typename T> inline dtkMetaContainerSequentialHandler::const_iterator *dtkMetaContainerSequentialHandlerTemplate<T>::cbegin(void) const
{
    return new const_iterator(m_container->cbegin());
}

template <typename T> inline dtkMetaContainerSequentialHandler::iterator *dtkMetaContainerSequentialHandlerTemplate<T>::end(void)
{
    return new iterator(m_container->end());
}

template <typename T> inline dtkMetaContainerSequentialHandler::const_iterator *dtkMetaContainerSequentialHandlerTemplate<T>::cend(void) const
{
    return new const_iterator(m_container->cend());
}

template <typename T> inline dtkMetaContainerSequentialHandlerTemplate<T>::dtkMetaContainerSequentialHandlerTemplate(T *c) : m_container(c)
{
}

template <typename T> inline dtkMetaContainerSequentialHandlerTemplate<T>::~dtkMetaContainerSequentialHandlerTemplate(void)
{
    m_container = NULL;
}

template <typename T> inline QString dtkMetaContainerSequentialHandlerTemplate<T>::description(void) const
{
    QString desc;
    QDebug dbg(&desc);
    dbg.nospace() << QMetaType::typeName(qMetaTypeId<T *>());
    dbg.nospace() << ", size = ";
    dbg.nospace() << this->size();
    dbg.nospace() << ", (";

    for (qlonglong i = 0; i < this->size(); ++i) {
        if (i)
            dbg << ", ";

        dbg << *static_cast<const ValueType *>(this->at(i));
    }

    dbg << ')';
    return desc;
}

template <typename T> inline bool dtkMetaContainerSequentialHandlerTemplate<T>::hasBiDirectionalIterator(void) const
{
    return dtkMetaContainerSequentialHandlerHelper<T>::hasBidirectionalIterator();
}

template <typename T> inline bool dtkMetaContainerSequentialHandlerTemplate<T>::hasRandomAccessIterator(void) const
{
    return dtkMetaContainerSequentialHandlerHelper<T>::hasRandomAccesIterator();
}

template <typename T> inline bool dtkMetaContainerSequentialHandlerTemplate<T>::empty(void) const
{
    return dtkMetaContainerSequentialHandlerHelper<T>::empty(m_container);
}

template <typename T> inline qlonglong dtkMetaContainerSequentialHandlerTemplate<T>::size(void) const
{
    return dtkMetaContainerSequentialHandlerHelper<T>::size(m_container);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::clear(void)
{
    dtkMetaContainerSequentialHandlerHelper<T>::clear(m_container);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::reserve(qlonglong size)
{
    dtkMetaContainerSequentialHandlerHelper<T>::reserve(m_container, size);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::resize(qlonglong size)
{
    dtkMetaContainerSequentialHandlerHelper<T>::resize(m_container, size);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::append(const void *t)
{
    dtkMetaContainerSequentialHandlerHelper<T>::insert(m_container, this->size(), *(static_cast<const ValueType *>(t)));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::prepend(const void *t)
{
    dtkMetaContainerSequentialHandlerHelper<T>::insert(m_container, 0, *(static_cast<const ValueType *>(t)));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::insert(qlonglong idx, const void *t)
{
    dtkMetaContainerSequentialHandlerHelper<T>::insert(m_container, idx, *(static_cast<const ValueType *>(t)));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::setAt(qlonglong idx, const void *t)
{
    dtkMetaContainerSequentialHandlerHelper<T>::setAt(m_container, idx, *(static_cast<const ValueType *>(t)));
}

template <typename T> inline void dtkMetaContainerSequentialHandlerTemplate<T>::removeAt(qlonglong idx)
{
    dtkMetaContainerSequentialHandlerHelper<T>::removeAt(m_container, idx);
}

template <typename T> inline const void *dtkMetaContainerSequentialHandlerTemplate<T>::at(qlonglong idx) const
{
    return dtkMetaContainerSequentialHandlerHelper<T>::at(m_container, idx);
}

template <typename T> inline void *dtkMetaContainerSequentialHandlerTemplate<T>::at(qlonglong idx)
{
    return dtkMetaContainerSequentialHandlerHelper<T>::at(m_container, idx);
}

template <typename T> inline dtkMetaContainerSequentialHandler::iterator *dtkMetaContainerSequentialHandlerTemplate<T>::iteratorAt(qlonglong idx, dtkMetaContainerSequentialHandler::iterator *it)
{
    dtkMetaContainerSequentialHandlerHelper<T>::iteratorAt(m_container, idx, static_cast<dtkMetaContainerSequentialHandlerTemplate<T>::iterator *>(it)->it);
    return it;
}

template <typename T> inline QVariant& dtkMetaContainerSequentialHandlerTemplate<T>::variantAt(qlonglong idx, QVariant& var)
{
    return dtkMetaContainerSequentialHandlerHelper<T>::variantAt(m_container, idx, var);
}


// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerHelper
// ///////////////////////////////////////////////////////////////////

template <typename T> inline bool dtkMetaContainerSequentialHandlerHelper<T>::empty(const T *c)
{
    return c->empty();
}

template <typename T> inline qlonglong dtkMetaContainerSequentialHandlerHelper<T>::size(const T *c)
{
    return c->size();
}

template <typename T> inline void dtkMetaContainerSequentialHandlerHelper<T>::clear(T *c)
{
    c->clear();
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaContainerIsReservable<U>::value>::type dtkMetaContainerSequentialHandlerHelper<T>::reserve(T *c, qlonglong size)
{
    c->reserve(size);
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaContainerIsReservable<U>::value >::type dtkMetaContainerSequentialHandlerHelper<T>::reserve(T *, qlonglong)
{
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaContainerIsResizable<U>::value>::type dtkMetaContainerSequentialHandlerHelper<T>::resize(T *c, qlonglong size)
{
    c->resize(size);
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaContainerIsResizable<U>::value >::type dtkMetaContainerSequentialHandlerHelper<T>::resize(T *, qlonglong)
{
}

template <typename T> inline void dtkMetaContainerSequentialHandlerHelper<T>::insert(T *c, qlonglong idx, const ValueType& t)
{
    typename T::iterator it(c->begin());
    std::advance(it, idx);
    c->insert(it, t);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerHelper<T>::setAt(T *c, qlonglong idx, const ValueType& t)
{
    typename T::iterator it(c->begin());
    std::advance(it, idx);
    *it = t;
}

template <typename T> inline void dtkMetaContainerSequentialHandlerHelper<T>::removeAt(T *c, qlonglong idx)
{
    typename T::iterator it(c->begin());
    std::advance(it, idx);
    c->erase(it);
}

template <typename T> inline const typename dtkMetaContainerSequentialHandlerHelper<T>::ValueType *dtkMetaContainerSequentialHandlerHelper<T>::at(const T *c, qlonglong idx)
{
    typename T::const_iterator it(c->begin());
    std::advance(it, idx);
    return &(*it);
}

template <typename T> inline typename dtkMetaContainerSequentialHandlerHelper<T>::ValueType *dtkMetaContainerSequentialHandlerHelper<T>::at(T *c, qlonglong idx)
{
    typename T::iterator it(c->begin());
    std::advance(it, idx);
    return &(*it);
}

template <typename T> inline void dtkMetaContainerSequentialHandlerHelper<T>::iteratorAt(T *c, qlonglong idx, Iterator& it)
{
    it = c->begin();
    std::advance(it, idx);
}

template <typename T> inline QVariant& dtkMetaContainerSequentialHandlerHelper<T>::variantAt(const T *c, qlonglong idx, QVariant& var)
{
    typename T::const_iterator it(c->cbegin());
    std::advance(it, idx);
    var.setValue(*it);
    return var;
}

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialIteratorHelper
// ///////////////////////////////////////////////////////////////////

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaTypeIsAssignable<U>::value>::type dtkMetaContainerSequentialIteratorHelper<T>::assign(T& lhs, const T& rhs)
{
    lhs = rhs;
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaTypeIsAssignable<U>::value >::type dtkMetaContainerSequentialIteratorHelper<T>::assign(T&, const T&)
{
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaTypeIsAddAssignable<U>::value>::type dtkMetaContainerSequentialIteratorHelper<T>::addAssign(T& lhs, const T& rhs)
{
    lhs += rhs;
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaTypeIsAddAssignable<U>::value >::type dtkMetaContainerSequentialIteratorHelper<T>::addAssign(T&, const T&)
{
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaTypeIsSubAssignable<U>::value>::type dtkMetaContainerSequentialIteratorHelper<T>::subAssign(T& lhs, const T& rhs)
{
    lhs -= rhs;
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaTypeIsSubAssignable<U>::value >::type dtkMetaContainerSequentialIteratorHelper<T>::subAssign(T&, const T&)
{
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaTypeIsMulAssignable<U>::value>::type dtkMetaContainerSequentialIteratorHelper<T>::mulAssign(T& lhs, const T& rhs)
{
    lhs *= rhs;
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaTypeIsMulAssignable<U>::value >::type dtkMetaContainerSequentialIteratorHelper<T>::mulAssign(T&, const T&)
{
}

template <typename T> template <typename U> inline typename std::enable_if<dtkMetaTypeIsDivAssignable<U>::value>::type dtkMetaContainerSequentialIteratorHelper<T>::divAssign(T& lhs, const T& rhs)
{
    lhs /= rhs;
}

template <typename T> template <typename U> inline typename std::enable_if < !dtkMetaTypeIsDivAssignable<U>::value >::type dtkMetaContainerSequentialIteratorHelper<T>::divAssign(T&, const T&)
{
}

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialConvertFunctor
// /////////////////////////////////////////////////////////////////

template <typename From> inline dtkMetaContainerSequentialHandler *dtkMetaContainerSequentialConvertFunctor<From *>::operator () (From *f) const
{
    return new dtkMetaContainerSequentialHandlerTemplate<From>(f);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename T> inline bool dtkMetaType::registerContainerPointerConverter(int id)
{
    return dtkMetaContainerSequentialRegisterConverter<T>::record(id);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename T> inline bool dtkContainerSequentialValueTypeIsMetaType<T, true>::record(int id)
{
    const int toId = qMetaTypeId<dtkMetaContainerSequentialHandler *>();

    if (!QMetaType::hasRegisteredConverterFunction(id, toId)) {
        dtkMetaContainerSequentialConvertFunctor<T> o;
        return QMetaType::registerConverter<T,
               dtkMetaContainerSequentialHandler *,
               dtkMetaContainerSequentialConvertFunctor<T> >(o);
    }

    return true;
}

//
// dtkMetaContainerSequentialHandler.tpp ends here

