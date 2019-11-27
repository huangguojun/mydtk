// Version: $Id: 006f7d582ab9fcd9c6d369d295f0fff6451b2c90 $
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
#include <dtkMeta>
#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// Private practices
// ///////////////////////////////////////////////////////////////////

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::copyData(Data *other_d)
{
    if (other_d->ref.ref()) {
        d = other_d;

    } else {
        if (other_d->capacityReserved) {
            if (other_d->alloc > PreallocSize) {
                d = Data::allocate(other_d->alloc);
                d->capacityReserved = true;
            } else {
                d = Data::fromRawData(this->preallocData(), other_d->size, dtkArrayData::Unsharable);
                d->capacityReserved = bool(PreallocSize);
                d->alloc = PreallocSize;
            }
        } else {
            d = Data::allocate(other_d->size);
        }

        if (d->alloc) {
            copyConstruct(other_d->begin(), other_d->end(), d->begin());
            d->size = other_d->size;
        }
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::reallocData(const qlonglong asize, const qlonglong aalloc, dtkArrayData::AllocationOptions options)
{
    Q_ASSERT(asize >= 0 && asize <= aalloc);
    Data *x = d;

    const bool isShared = d->ref.isShared();

    if (aalloc != 0) {
        if (aalloc != qlonglong(d->alloc) || isShared) {
            QT_TRY {
                if (aalloc > PreallocSize)
                {
                    // allocate memory
                    x = Data::allocate(aalloc, options);
                    x->size = asize;
                    x->capacityReserved = d->capacityReserved;

                } else {
                    x = Data::fromRawData(this->preallocData(), asize, dtkArrayData::Unsharable);
                    x->capacityReserved = bool(PreallocSize);
                    x->alloc = PreallocSize;
                }

                Q_CHECK_PTR(x);
                // aalloc is bigger then 0 so it is not [un]sharedEmpty
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)
                Q_ASSERT(x->ref.isSharable() || options.testFlag(dtkArrayData::Unsharable) || x->alloc == PreallocSize);
#endif
                Q_ASSERT(!x->ref.isStatic());

                T *srcBegin = d->begin();
                T *srcEnd = asize > d->size ? d->end() : d->begin() + asize;
                T *dst = x->begin();

                if (QTypeInfo<T>::isStatic || (isShared && QTypeInfo<T>::isComplex))
                {
                    // we can not move the data, we need to copy construct it
                    while (srcBegin != srcEnd) {
                        new (dst++) T(*srcBegin++);
                    }
                } else {
                    ::memcpy(static_cast<void *>(dst), static_cast<void *>(srcBegin), (srcEnd - srcBegin) * sizeof(T));
                    dst += srcEnd - srcBegin;

                    // destruct unused / not moved data
                    if (asize < d->size)
                        destruct(d->begin() + asize, d->end());
                }

                if (asize > d->size)
                {
                    // construct all new objects when growing
                    QT_TRY {
                        defaultConstruct(dst, x->end());
                    } QT_CATCH (...) {
                        // destruct already copied objects
                        destruct(x->begin(), dst);
                        QT_RETHROW;
                    }
                }
            } QT_CATCH (...) {
                Data::deallocate(x);
                QT_RETHROW;
            }
        } else {
            Q_ASSERT(qlonglong(d->alloc) == aalloc); // resize, without changing allocation size
            Q_ASSERT(isDetached());       // can be done only on detached d
            Q_ASSERT(x == d);             // in this case we do not need to allocate anything

            if (asize <= d->size) {
                destruct(x->begin() + asize, x->end()); // from future end to current end
            } else {
                defaultConstruct(x->end(), x->begin() + asize); // from current end to future end
            }

            x->size = asize;
        }
    } else {
        x = Data::fromRawData(this->preallocData(), 0, dtkArrayData::Unsharable);
        x->capacityReserved = bool(PreallocSize);
        x->alloc = PreallocSize;
    }

    if (d != x) {
        if (!d->ref.deref()) {
            if (QTypeInfo<T>::isStatic || !aalloc || (isShared && QTypeInfo<T>::isComplex)) {
                // data was copy constructed, we need to call destructors
                // or if !alloc we did nothing to the old 'd'.
                freeData(d);
            } else {
                Data::deallocate(d);
            }
        }

        d = x;
    }

    Q_ASSERT(d->data());
    Q_ASSERT(quintptr(d->size) <= d->alloc);
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)
    Q_ASSERT(d != Data::unsharableEmpty());
#endif
    //Q_ASSERT(aalloc ? d != Data::sharedNull() : d == Data::sharedNull());
    Q_ASSERT(d->alloc >= quintptr(aalloc));
    Q_ASSERT(d->size == asize);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::reallocData(const qlonglong size)
{
    return this->reallocData(size, d->alloc);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::freeData(Data *x)
{
    if (!isRawData)
        destruct(x->begin(), x->end());

    Data::deallocate(x);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::defaultConstruct(T *from, T *to)
{
    if (QTypeInfo<T>::isComplex) {
        while (from != to) {
            new (from++) T();
        }
    } else {
        ::memset(static_cast<void *>(from), 0, (to - from) * sizeof(T));
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::copyConstruct(const T *srcFrom, const T *srcTo, T *dstFrom)
{
    if (QTypeInfo<T>::isComplex) {
        while (srcFrom != srcTo)
            new (dstFrom++) T(*srcFrom++);
    } else {
        ::memcpy(static_cast<void *>(dstFrom), static_cast<const void *>(srcFrom), (srcTo - srcFrom) * sizeof(T));
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::destruct(T *from, T *to)
{
    if (QTypeInfo<T>::isComplex) {
        while (from != to) {
            from++->~T();
        }
    }
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::isValidIterator(const iterator& i) const
{
    return (i <= d->end()) && (d->begin() <= i);
}

// ///////////////////////////////////////////////////////////////////
// Public API
// ///////////////////////////////////////////////////////////////////

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(void) : isRawData(false)
{
    Q_STATIC_ASSERT_X(PreallocSize >= 0, "dtkArray PreallocSize must be greater than 0.");

    if (PreallocSize) {
        d = Data::fromRawData(this->preallocData(), 0, dtkArrayData::Unsharable);
        d->capacityReserved = bool(PreallocSize);
        d->alloc = PreallocSize;

    } else {
        d = Data::fromRawData(this->preallocData(), 0);
    }
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(qlonglong size) : isRawData(false)
{
    Q_STATIC_ASSERT_X(PreallocSize >= 0, "dtkArray PreallocSize must be greater or equal than 0.");
    Q_ASSERT_X(size >= 0, "dtkArray::dtkArray", "Size must be greater than or equal to 0.");

    if (size > PreallocSize) {
        d = Data::allocate(size);
        d->size = size;
    } else {
        d = Data::fromRawData(this->preallocData(), size, dtkArrayData::Unsharable);
        d->capacityReserved = bool(PreallocSize);
        d->alloc = PreallocSize;
    }

    defaultConstruct(d->begin(), d->end());
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(qlonglong size, const T& value) : isRawData(false)
{
    Q_STATIC_ASSERT_X(PreallocSize >= 0, "dtkArray PreallocSize must be greater than 0.");
    Q_ASSERT_X(size >= 0, "dtkArray::dtkArray", "Size must be greater than or equal to 0.");

    if (size > PreallocSize) {
        d = Data::allocate(size);
        d->size = size;
    } else {
        d = Data::fromRawData(this->preallocData(), size, dtkArrayData::Unsharable);
        d->capacityReserved = bool(PreallocSize);
        d->alloc = PreallocSize;
    }

    T *i = d->end();

    while (i != d->begin())
        new (--i) T(value);
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(const T *values, qlonglong size) : isRawData(false)
{
    Q_STATIC_ASSERT_X(PreallocSize >= 0, "dtkArray PreallocSize must be greater than 0.");
    Q_ASSERT_X(size >= 0, "dtkArray::dtkArray", "Size must be greater than or equal to 0.");

    if (size > PreallocSize) {
        d = Data::allocate(size);
        d->size = size;
    } else {
        d = Data::fromRawData(this->preallocData(), size, dtkArrayData::Unsharable);
        d->capacityReserved = bool(PreallocSize);
        d->alloc = PreallocSize;
    }

    T *i = d->end();
    const T *value = values + size;

    while (i != d->begin())
        new (--i) T(*(--value));
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(std::initializer_list<T> args) : isRawData(false)
{
    Q_STATIC_ASSERT_X(PreallocSize >= 0, "dtkArray PreallocSize must be greater than 0.");
    Q_ASSERT_X(args.size() >= 0, "dtkArray::dtkArray", "Size must be greater than or equal to 0.");

    if (args.size() > PreallocSize) {
        d = Data::allocate(args.size());
        d->size = int(args.size());
    } else {
        d = Data::fromRawData(this->preallocData(), args.size(), dtkArrayData::Unsharable);
        d->capacityReserved = bool(PreallocSize);
        d->alloc = PreallocSize;
    }

    copyConstruct(args.begin(), args.end(), d->begin());
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(const dtkArray& other) : isRawData(false)
{
    this->copyData(other.d);
}

#ifdef Q_COMPILER_RVALUE_REFS
template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::dtkArray(dtkArray&& other) : isRawData(false)
{
    this->copyData(other.d);
    other.d = Data::sharedNull();
}
#endif

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>::~dtkArray(void)
{
    if (!d->ref.deref())
        freeData(d);
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator = (const dtkArray& other)
{
    if (other.d != d) {

        if (!d->ref.deref())
            freeData(d);

        this->copyData(other.d);
    }

    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator = (dtkArray&& other)
{
    this->swap(other);
    return *this;
}
#endif

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::operator == (const dtkArray<T, PreallocSize>& other) const
{
    if (d->size != other.d->size)
        return false;

    if (d == other.d)
        return true;

    T *b = d->begin();
    T *i = b + d->size;
    T *j = other.d->end();

    while (i != b)
        if (!(*--i == *--j))
            return false;

    return true;
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::operator != (const dtkArray<T, PreallocSize>& other) const
{
    return !(*this == other);
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline dtkArray<T, PreallocSize>::dtkArray(const dtkArray<T, PreallocSizeOther>& other)
{
    this->copyData(other.d);
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator = (const dtkArray<T, PreallocSizeOther>& other)
{
    if (other.d != d) {

        if (!d->ref.deref())
            freeData(d);

        this->copyData(other.d);
    }

    return *this;
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline bool dtkArray<T, PreallocSize>::operator == (const dtkArray<T, PreallocSizeOther>& other) const
{
    if (d->size != other.d->size)
        return false;

    if (d == other.d)
        return true;

    T *b = d->begin();
    T *i = b + d->size;
    T *j = other.d->end();

    while (i != b)
        if (!(*--i == *--j))
            return false;

    return true;
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline bool dtkArray<T, PreallocSize>::operator != (const dtkArray<T, PreallocSizeOther>& other) const
{
    return !(*this == other);
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline bool dtkArray<T, PreallocSize>::isSharedWith(const dtkArray<T, PreallocSizeOther>& other) const
{
    return d == other.d;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::swap(dtkArray& other)
{
    if (other.d->ref.isSharable() && d->ref.isSharable()) {
        qSwap(d, other.d);

    } else {
        dtkArray<T, PreallocSize> tmp(other);
        other = *this;
        *this = tmp;
    }
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::isDetached(void) const
{
    return !d->ref.isShared();
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::detach(void)
{
    if (!isDetached()) {
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)

        if (!d->alloc)
            d = Data::unsharableEmpty();
        else
#endif
            dtkDebug() << Q_FUNC_INFO << "detaching array of size " << d->size;

        reallocData(d->size, qlonglong(d->alloc));
    }

    Q_ASSERT(isDetached());
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::isSharedWith(const dtkArray& other) const
{
    return d == other.d;
}

#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)
template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::setSharable(bool sharable)
{
    if (sharable == d->ref.isSharable())
        return;

    if (!sharable)
        detach();

    if (d == Data::unsharableEmpty()) {
        if (sharable)
            d = Data::sharedNull();
    } else {
        d->ref.setSharable(sharable);
    }

    Q_ASSERT(d->ref.isSharable() == sharable);
}
#endif

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::empty(void) const
{
    return (d->size == 0);
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::isEmpty(void) const
{
    return (d->size == 0);
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::usePreallocation(void) const
{
    return this->dtkArrayPrealloc<T, PreallocSize>::isPreallocated(d->data());
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::size(void) const
{
    return d->size;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::count(void) const
{
    return d->size;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::length(void) const
{
    return d->size;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::capacity(void) const
{
    return qlonglong(d->alloc);
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::preallocatedCapacity(void) const
{
    return PreallocSize;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::clear(void)
{
    *this = dtkArray<T, PreallocSize>();
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::resize(qlonglong asize)
{
    if (asize < 0)
        asize = 0;

    qlonglong newAlloc;
    const qlonglong oldAlloc = qlonglong(d->alloc);
    dtkArrayData::AllocationOptions opt;

    if (asize > oldAlloc) { // there is not enough space
        newAlloc = asize;
        opt = dtkArrayData::Grow;
    } else if (!d->capacityReserved && asize < d->size && asize < (oldAlloc >> 1)) { // we want to shrink
        newAlloc = asize;
        opt = dtkArrayData::Grow;
    } else {
        newAlloc = oldAlloc;
    }

    reallocData(asize, newAlloc, opt);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::reserve(qlonglong asize)
{
    if (asize > qlonglong(d->alloc))
        reallocData(d->size, asize);

    if (isDetached())
        d->capacityReserved = 1;

    Q_ASSERT(capacity() >= asize);

}
template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::squeeze(void)
{
    reallocData(d->size, d->size);

    if (d->capacityReserved && d->size > PreallocSize) {
        // capacity reserved in a read only memory would be useless
        // this checks avoid writing to such memory.
        d->capacityReserved = 0;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const T& value)
{
    const bool isTooSmall = quintptr(d->size + 1) > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? d->size + 1 : d->alloc, opt);
    }

    if (QTypeInfo<T>::isComplex) {
        new (d->end()) T(value);
    } else {
        *d->end() = T(value);
    }

    ++d->size;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const T& v1, const T& v2)
{
    quintptr newSize = d->size + 2;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;

        if (QTypeInfo<T>::isComplex) {
            new (--w) T(v2);
            new (--w) T(v1);
        } else {
            *--w = T(v2);
            *--w = T(v1);
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const T& v1, const T& v2, const T& v3)
{
    quintptr newSize = d->size + 3;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;

        if (QTypeInfo<T>::isComplex) {
            new (--w) T(v3);
            new (--w) T(v2);
            new (--w) T(v1);
        } else {
            *--w = T(v3);
            *--w = T(v2);
            *--w = T(v1);
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const T& v1, const T& v2, const T& v3, const T& v4)
{
    quintptr newSize = d->size + 4;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;

        if (QTypeInfo<T>::isComplex) {
            new (--w) T(v4);
            new (--w) T(v3);
            new (--w) T(v2);
            new (--w) T(v1);
        } else {
            *--w = T(v4);
            *--w = T(v3);
            *--w = T(v2);
            *--w = T(v1);
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const T *values, qlonglong size)
{
    quintptr newSize = d->size + size;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;
        const T *i = values + size;
        const T *b = values;

        while (i != b) {
            if (QTypeInfo<T>::isComplex)
                new (--w) T(*--i);
            else
                *--w = *--i;
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(std::initializer_list<T> args)
{
    qlonglong size = args.size();

    if (size != 0) {
        quintptr newSize = d->size + size;
        const bool isTooSmall = newSize > d->alloc;

        if (!isDetached() || isTooSmall) {
            dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
            reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
        }

        if (d->alloc) {
            copyConstruct(args.begin(), args.end(), d->begin() + d->size);
            d->size = newSize;
        }
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::append(const dtkArray& other)
{
    quintptr newSize = d->size + other.d->size;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;
        T *i = other.d->end();
        T *b = other.d->begin();

        while (i != b) {
            if (QTypeInfo<T>::isComplex)
                new (--w) T(*--i);
            else
                *--w = *--i;
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::prepend(const T& value)
{
    insert(begin(), 1, value);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::insert(qlonglong index, const T& value)
{
    Q_ASSERT_X(index >= 0 && index <= d->size, "dtkArray<T, PreallocSize>::insert", "index out of range");

    insert(begin() + index, 1, value);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::insert(qlonglong index, qlonglong length, const T& value)
{
    Q_ASSERT_X(index >= 0 && index <= d->size, "dtkArray<T, PreallocSize>::insert", "index out of range");

    insert(begin() + index, length, value);
}

template <typename T, qlonglong PreallocSize> inline typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::insert(iterator before, const T& value)
{
    return insert(before, 1, value);
}

template <typename T, qlonglong PreallocSize> inline typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::insert(iterator before, qlonglong n, const T& t)
{
    Q_ASSERT_X(isValidIterator(before), "dtkArray<T, PreallocSize>::insert", "The specified iterator argument 'before' is invalid");

    qlonglong offset = std::distance(d->begin(), before);

    if (n > 0) {
        const T copy(t);

        if (!isDetached() || d->size + n > qlonglong(d->alloc))
            reallocData(d->size, d->size + n, dtkArrayData::Grow);

        if (QTypeInfo<T>::isStatic) {
            T *b = d->end();
            T *i = d->end() + n;

            while (i != b)
                new (--i) T;

            i = d->end();
            T *j = i + n;
            b = d->begin() + offset;

            while (i != b)
                *--j = *--i;

            i = b + n;

            while (i != b)
                *--i = copy;
        } else {
            T *b = d->begin() + offset;
            T *i = b + n;
            ::memmove(i, b, (d->size - offset) * sizeof(T));

            while (i != b)
                new (--i) T(copy);
        }

        d->size += n;
    }

    return d->begin() + offset;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::setAt(qlonglong index, const T& value)
{
    Q_ASSERT_X(index >= 0 && index < d->size, "dtkArray<T, PreallocSize>::setAt", "index out of range");
    data()[index] = value;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::setAt(qlonglong index, const T *values, qlonglong length)
{
    Q_ASSERT_X(index >= 0 && length >= 0, "dtkArray<T, PreallocSize>::setAt", "index out of range or negative length");

    if (length != 0) {
        quintptr newSize = qMax((quintptr)d->size, (quintptr)(index + length));
        const bool isTooSmall = newSize > d->alloc;

        if (!isDetached() || isTooSmall) {
            dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
            reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
        }

        if (QTypeInfo<T>::isStatic) {
            if (index >= d->size) {
                T *b = d->end();
                T *i = d->begin() + index;

                while (i != b)
                    new (--i) T;
            }

            T *w = d->begin() + index + length;
            const T *b = values;
            const T *i = b + length;

            while (i != b)
                new (--w) T(*--i);

        } else {
            if (index >= d->size) {
                T *b = d->end();
                T *i = d->begin() + index;

                while (i != b) {
                    new (--i) T();
                }
            }

            T *src = const_cast<T *>(values);
            T *dst = d->begin() + index;
            ::memmove(dst, src, length * sizeof(T));
        }

        d->size = newSize;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::replace(qlonglong index, const T& value)
{
    this->setAt(index, value);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::replace(qlonglong index, const T *values, qlonglong length)
{
    this->setAt(index, values, length);
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::fill(const T& from, qlonglong asize)
{
    resize(asize < 0 ? d->size : asize);

    if (d->size) {
        T *i = d->end();
        T *b = d->begin();

        while (i != b)
            *--i = from;
    }

    return *this;
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::setRawData(const T *raw_data, qlonglong size)
{
    if (!d->ref.deref())
        freeData(d);

    if (!raw_data || ! size) {
        d = Data::fromRawData(this->preallocData(), 0);
    } else {
        d = Data::fromRawData(raw_data, size);
        d->ref.atomic.store(2);
        d->alloc = size;
        isRawData = true;
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::setWritableRawData(T *raw_data, qlonglong size)
{
    if (!d->ref.deref())
        freeData(d);

    if (!raw_data || ! size) {
        d = Data::fromRawData(this->preallocData(), 0);
    } else {
        d = Data::fromRawData(raw_data, size);
        d->alloc = size;
        isRawData = true;
    }
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::fromRawData(const T *raw_data, qlonglong size)
{
    dtkArray result;

    result.d = Data::fromRawData(raw_data, size);
    result.d->ref.atomic.store(2);
    result.d->alloc = size;
    result.isRawData = true;

    return result;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::fromWritableRawData(T *raw_data, qlonglong size)
{
    dtkArray result;

    result.d = Data::fromRawData(raw_data, size);
    result.d->alloc = size;
    result.isRawData = true;

    return result;
}

template <typename T, qlonglong PreallocSize> inline typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::erase(iterator abegin, iterator aend)
{
    Q_ASSERT_X(isValidIterator(abegin), "dtkArray<T, PreallocSize>::erase", "The specified iterator argument 'begin' is invalid");
    Q_ASSERT_X(isValidIterator(aend), "dtkArray<T, PreallocSize>::erase", "The specified iterator argument 'end' is invalid");

    const int itemsToErase = aend - abegin;

    if (!itemsToErase)
        return abegin;

    Q_ASSERT(abegin >= d->begin());
    Q_ASSERT(aend <= d->end());
    Q_ASSERT(abegin <= aend);

    const int itemsUntouched = abegin - d->begin();

    // FIXME we could do a proper realloc, which copy constructs only needed data.
    // FIXME we ara about to delete data maybe it is good time to shrink?
    // FIXME the shrink is also an issue in removeLast, that is just a copy + reduce of this.
    if (d->alloc) {
        detach();
        abegin = d->begin() + itemsUntouched;
        aend = abegin + itemsToErase;

        if (QTypeInfo<T>::isStatic) {
            iterator moveBegin = abegin + itemsToErase;
            iterator moveEnd = d->end();

            while (moveBegin != moveEnd) {
                if (QTypeInfo<T>::isComplex)
                    static_cast<T *>(abegin)->~T();

                new (abegin++) T(*moveBegin++);
            }

            if (abegin < d->end()) {
                // destroy rest of instances
                destruct(abegin, d->end());
            }
        } else {
            destruct(abegin, aend);
            memmove(abegin, aend, (d->size - itemsToErase - itemsUntouched) * sizeof(T));
        }

        d->size -= itemsToErase;
    }

    return d->begin() + itemsUntouched;
}

template <typename T, qlonglong PreallocSize> inline typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::erase(iterator pos)
{
    return erase(pos, pos + 1);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::remove(qlonglong index)
{
    Q_ASSERT_X(index >= 0 && index <= d->size, "dtkArray<T, PreallocSize>::remove", "index out of range");

    erase(d->begin() + index, d->begin() + index + 1);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::remove(qlonglong index, qlonglong length)
{
    Q_ASSERT_X(index >= 0 && length >= 0 && index + length <= d->size, "dtkArray<T, PreallocSize>::remove", "index out of range");

    erase(d->begin() + index, d->begin() + index + length);
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::removeFirst(void)
{
    Q_ASSERT(!isEmpty());

    erase(d->begin());
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::removeLast(void)
{
    Q_ASSERT(!isEmpty());
    Q_ASSERT(d->alloc);

    if (!d->ref.isShared()) {
        --d->size;

        if (QTypeInfo<T>::isComplex)
            (d->data() + d->size)->~T();
    } else {
        reallocData(d->size - 1);
    }
}

template <typename T, qlonglong PreallocSize> inline void dtkArray<T, PreallocSize>::removeAt(qlonglong index)
{
    Q_ASSERT_X(index >= 0 && index <= d->size, "dtkArray<T, PreallocSize>::removeAt", "index out of range");

    erase(d->begin() + index, d->begin() + index + 1);
}

template <typename T, qlonglong PreallocSize> inline T dtkArray<T, PreallocSize>::takeFirst(void)
{
    Q_ASSERT(!isEmpty()); T r = first(); removeFirst(); return r;
}

template <typename T, qlonglong PreallocSize> inline T dtkArray<T, PreallocSize>::takeLast(void)
{
    Q_ASSERT(!isEmpty()); T r = last(); removeLast(); return r;
}

template <typename T, qlonglong PreallocSize> inline T dtkArray<T, PreallocSize>::takeAt(qlonglong index)
{
    T t = at(index); remove(index); return t;
}

template <typename T, qlonglong PreallocSize> Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::reverse(void)
{
    detach();
    T *src = begin();
    T *dst = end() - 1;

    while (src < dst)
        qSwap(*(dst--), *(src++));
}

template <typename T, qlonglong PreallocSize> Q_OUTOFLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::reversed(void) const
{
    dtkArray<T, PreallocSize> result(size());
    const T *src = begin();
    T *dst = result.end() - 1;

    if (!QTypeInfo<T>::isComplex) {
        while (src != end())
            *(dst--) = *(src++);
    } else {
        while (src != end())
            new (dst--) T(*src++);
    }

    return result;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator += (const T& value)
{
    this->append(value);
    return *this;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator += (const dtkArray& other)
{
    this->append(other);
    return *this;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator << (const T& value)
{
    this->append(value);
    return *this;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator << (const dtkArray& other)
{
    *this += other;
    return *this;
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator += (const dtkArray<T, PreallocSizeOther>& other)
{
    quintptr newSize = d->size + other.d->size;
    const bool isTooSmall = newSize > d->alloc;

    if (!isDetached() || isTooSmall) {
        dtkArrayData::AllocationOptions opt(isTooSmall ? dtkArrayData::Grow : dtkArrayData::Default);
        reallocData(d->size, isTooSmall ? newSize : d->alloc, opt);
    }

    if (d->alloc) {
        T *w = d->begin() + newSize;
        T *i = other.d->end();
        T *b = other.d->begin();

        while (i != b) {
            if (QTypeInfo<T>::isComplex)
                new (--w) T(*--i);
            else
                *--w = *--i;
        }

        d->size = newSize;
    }

    return *this;
}

template <typename T, qlonglong PreallocSize> template <qlonglong PreallocSizeOther> inline dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator << (const dtkArray<T, PreallocSizeOther>& other)
{
    *this += other;
    return *this;
}

template <typename T, qlonglong PreallocSize> inline const T& dtkArray<T, PreallocSize>::at(qlonglong i) const
{
    Q_ASSERT_X(i >= 0 && i < d->size, "dtkArray<T, PreallocSize>::at", "index out of range");
    return d->begin()[i];
}

template <typename T, qlonglong PreallocSize> inline const T& dtkArray<T, PreallocSize>::operator[](qlonglong i) const
{
    Q_ASSERT_X(i >= 0 && i < d->size, "dtkArray<T, PreallocSize>::operator[]", "index out of range");
    return d->begin()[i];
}

template <typename T, qlonglong PreallocSize> inline T& dtkArray<T, PreallocSize>::operator[](qlonglong i)
{
    Q_ASSERT_X(i >= 0 && i < d->size, "dtkArray<T, PreallocSize>::operator[]", "index out of range");
    return data()[i];
}

template <typename T, qlonglong PreallocSize> inline T& dtkArray<T, PreallocSize>::first(void)
{
    Q_ASSERT(!isEmpty()); return *begin();
}

template <typename T, qlonglong PreallocSize> inline const T& dtkArray<T, PreallocSize>::first(void) const
{
    Q_ASSERT(!isEmpty()); return *begin();
}

template <typename T, qlonglong PreallocSize> inline T& dtkArray<T, PreallocSize>::last(void)
{
    Q_ASSERT(!isEmpty()); return *(end() - 1);
}

template <typename T, qlonglong PreallocSize> inline const T& dtkArray<T, PreallocSize>::last(void) const
{
    Q_ASSERT(!isEmpty()); return *(end() - 1);
}

template <typename T, qlonglong PreallocSize> Q_OUTOFLINE_TEMPLATE T dtkArray<T, PreallocSize>::value(qlonglong index) const
{
    return quintptr(index) >= quintptr(d->size) ? T() : d->begin()[index];
}

template <typename T, qlonglong PreallocSize> Q_OUTOFLINE_TEMPLATE T dtkArray<T, PreallocSize>::value(qlonglong index, const T& defaultValue) const
{
    return quintptr(index) >= quintptr(d->size) ? defaultValue : d->begin()[index];
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::startsWith(const T& t) const
{
    return !isEmpty() && first() == t;
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::endsWith(const T& t) const
{
    return !isEmpty() && last() == t;
}

template <typename T, qlonglong PreallocSize> inline bool dtkArray<T, PreallocSize>::contains(const T& t) const
{
    T *b = d->begin();
    T *i = d->end();

    while (i != b)
        if (*--i == t)
            return true;

    return false;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::indexOf(const T& t, qlonglong from) const
{
    if (from < 0)
        from = qMax(from + d->size, qlonglong(0));

    if (from < d->size) {
        T *n = d->begin() + from - 1;
        T *e = d->end();

        while (++n != e)
            if (*n == t)
                return n - d->begin();
    }

    return -1;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::lastIndexOf(const T& t, qlonglong from) const
{
    if (from < 0)
        from += d->size;
    else if (from >= d->size)
        from = d->size - 1;

    if (from >= 0) {
        T *b = d->begin();
        T *n = d->begin() + from + 1;

        while (n != b) {
            if (*--n == t)
                return n - b;
        }
    }

    return -1;
}

template <typename T, qlonglong PreallocSize> inline qlonglong dtkArray<T, PreallocSize>::count(const T& t) const
{
    qlonglong c = 0;
    T *b = d->begin();
    T *i = d->end();

    while (i != b)
        if (*--i == t)
            ++c;

    return c;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::mid(qlonglong pos, qlonglong len) const
{
    if (len < 0)
        len = size() - pos;

    if (pos == 0 && len == size())
        return *this;

    if (pos + len > size())
        len = size() - pos;

    dtkArray<T, PreallocSize> copy;
    copy.append(constData() + pos, len);
    return copy;
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::left(qlonglong length) const
{
    return mid(0, length);
}

template <typename T, qlonglong PreallocSize> inline dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::right(qlonglong length) const
{
    if (length < 0 || length >= size())
        length = size();

    return mid(size() - length, length);
}

// ///////////////////////////////////////////////////////////////////

template <typename T, qlonglong PreallocSize> inline QDebug& operator << (QDebug debug, const dtkArray<T, PreallocSize>& array)
{
    const bool oldSetting = debug.autoInsertSpaces();
    debug.nospace() << "dtkArray";
    debug.nospace() << '(';

    for (typename dtkArray<T, PreallocSize>::size_type i = 0; i < array.size(); ++i) {
        if (i)
            debug << ", ";

        debug << array.at(i);
    }

    debug << ')';
    debug.setAutoInsertSpaces(oldSetting);
    return debug.maybeSpace();
}

// ///////////////////////////////////////////////////////////////////


template<typename T, qlonglong PreallocSize> inline QDataStream& operator << (QDataStream& s, const dtkArray<T, PreallocSize>& array)
{
    s << quint64(array.size());

    for (const T& t : array)
        s << t;

    return s;
}

template<typename T, qlonglong PreallocSize> inline QDataStream& operator >> (QDataStream& s, dtkArray<T, PreallocSize>& array)
{
    array.clear();
    quint64 size; s >> size;
    array.resize(size);

    for (T& t : array) {
        s >> t;
    }

    return s;
}

template<typename T, qlonglong PreallocSize> inline QDataStream& operator << (QDataStream& s, const dtkArray<T *, PreallocSize>& array)
{
    s << quint64(array.size());

    for (T *t : array) {
        s << dtkMetaType::variantFromValue(t);
    }

    return s;
}

template<typename T, qlonglong PreallocSize> inline QDataStream& operator >> (QDataStream& s, dtkArray<T *, PreallocSize>& array)
{
    array.clear();
    quint64 size; s >> size;
    array.resize(size);

    for (T *&t : array) {
        QVariant var;
        s >> var;
        t = var.value<T *>();
    }

    return s;
}

// ///////////////////////////////////////////////////////////////////
// Registering dtkArray to QMetaType system
// ///////////////////////////////////////////////////////////////////

template <typename T, qlonglong PreallocSize> struct QMetaTypeId< dtkArray<T, PreallocSize> > {
    enum {
        Defined = QMetaTypeId2<T>::Defined
    };

    static int qt_metatype_id() {
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);

        if (const int id = metatype_id.load())
            return id;

        const char *array_type = QMetaType::typeName(qMetaTypeId<T>());
        Q_ASSERT(array_type);
        int type_length = int(qstrlen(array_type));

        QByteArray array_name("dtkArray", 8);
        QByteArray array_size = QByteArray::number(PreallocSize);

        QByteArray typeName;
        typeName.reserve(array_name.size() + 1 + type_length + 1 + array_size.size() + 1);
        typeName.append(array_name).append('<').append(array_type).append(',').append(array_size).append('>');

        const int newId = qRegisterNormalizedMetaType< dtkArray<T, PreallocSize> >(typeName, reinterpret_cast<dtkArray<T, PreallocSize>*>(quintptr(-1)));
        metatype_id.storeRelease(newId);

        qRegisterMetaTypeStreamOperators<T>(array_type);
        qRegisterMetaTypeStreamOperators< dtkArray<T, PreallocSize> >(typeName.constData());

        return newId;
    }
};
namespace QtPrivate {
    template<typename T, qlonglong PreallocSize> struct IsSequentialContainer< dtkArray<T, PreallocSize> > {
        enum { Value = true };
    };

// Pointer version

}
template <typename T, qlonglong PreallocSize> struct QMetaTypeId< dtkArray<T, PreallocSize> *> {
    enum {
        Defined = QMetaTypeId2<T>::Defined
    };

    static int qt_metatype_id() {
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);

        if (const int id = metatype_id.load())
            return id;

        const char *array_type = QMetaType::typeName(qMetaTypeId<T>());
        Q_ASSERT(array_type);
        int type_length = int(qstrlen(array_type));

        QByteArray array_name("dtkArray", 8);
        QByteArray array_size = QByteArray::number(PreallocSize);

        QByteArray typeName;
        typeName.reserve(array_name.size() + 1 + type_length + 1 + array_size.size() + 1 + 1);
        typeName.append(array_name).append('<').append(array_type).append(',').append(array_size).append('>').append('*');

        const int newId = qRegisterNormalizedMetaType<dtkArray<T, PreallocSize> *>(typeName, reinterpret_cast<dtkArray<T, PreallocSize>* *>(quintptr(-1)));
        metatype_id.storeRelease(newId);

        if (newId > 0) {
            QMetaTypeId< dtkArray<T, PreallocSize> >::qt_metatype_id();
            dtkMetaType::registerContainerPointerConverter<dtkArray<T, PreallocSize> *>(newId);

            qRegisterMetaTypeStreamOperators<T>(array_type);
            qRegisterMetaTypeStreamOperators< dtkArray<T, PreallocSize> *>(typeName.constData());
        }

        return newId;
    }
};

template<typename T, qlonglong PreallocSize> struct dtkMetaTypeIsSequentialContainerPointer< dtkArray<T, PreallocSize> *> : std::true_type {};

// ///////////////////////////////////////////////////////////////////
// Specialization of typetraits
// ///////////////////////////////////////////////////////////////////

#include <dtkMetaTypeTraits.h>

template< typename T, qlonglong PreallocSize > struct dtkMetaContainerIsReservable< dtkArray<T, PreallocSize> > : std::true_type {};
template< typename T, qlonglong PreallocSize > struct  dtkMetaContainerIsResizable< dtkArray<T, PreallocSize> > : std::true_type {};

//
// dtkArray.tpp ends here
