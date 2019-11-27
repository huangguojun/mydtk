// Version: $Id: d5a6eb3591efcd579851864e63ad380bfb770d25 $
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

#include <dtkCoreExport.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

template <typename T> struct dtkArrayDataTemplate;

template <> struct dtkArrayDataTemplate<qint32> {
public:
    dtkArrayDataTemplate(void) {}
    dtkArrayDataTemplate(int r, qint32 s, quint32 a, quint32 c, qptrdiff o) : size(s), alloc(a), capacityReserved(c), offset(o) {
        ref.atomic.store(r);
    }

public:
    QtPrivate::RefCount ref;
    qint32   size;
    quint32  alloc : 31;
    quint32  capacityReserved : 1;
    qptrdiff offset;
};

template <> struct dtkArrayDataTemplate<qint64> {
public:
    dtkArrayDataTemplate(void) {}
    dtkArrayDataTemplate(int r, qint64 s, quint64 a, quint64 c, qptrdiff o) : size(s), alloc(a), capacityReserved(c), offset(o) {
        ref.atomic.store(r);
    }

public:
    QtPrivate::RefCount ref;
    qint64   size;
    quint64  alloc : 63;
    quint64  capacityReserved : 1;
    qptrdiff offset;
};

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

typedef dtkArrayDataTemplate<qintptr> dtkArrayDataBase;

struct DTKCORE_EXPORT dtkArrayData : public dtkArrayDataBase {
public:
    using dtkArrayDataBase::ref;
    using dtkArrayDataBase::size;
    using dtkArrayDataBase::alloc;
    using dtkArrayDataBase::capacityReserved;
    using dtkArrayDataBase::offset;

public:
    dtkArrayData(void) : dtkArrayDataBase() {}
    dtkArrayData(int r, qintptr s, quintptr a, quintptr c, qptrdiff o) : dtkArrayDataBase(r, s, a, c, o) {}

public:
    void *data(void)       {
        return reinterpret_cast<      char *>(this) + offset;
    }
    const void *data(void) const {
        return reinterpret_cast<const char *>(this) + offset;
    }

public:
    bool isMutable(void) const {
        return alloc != 0;
    }

public:
    enum AllocationOption {
        CapacityReserved    = 0x1,
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)
        Unsharable          = 0x2,
#endif
        RawData             = 0x4,
        Grow                = 0x8,
        Default = 0
    };

    Q_DECLARE_FLAGS(AllocationOptions, AllocationOption);

public:
    size_t detachCapacity(size_t newSize) const;

    AllocationOptions detachFlags(void) const;
    AllocationOptions  cloneFlags(void) const;

public:
    static dtkArrayData *allocate(size_t objectSize, size_t alignment, size_t capacity, AllocationOptions options = Default);
    static void deallocate(dtkArrayData *data, size_t objectSize, size_t alignment);

public:
    static const dtkArrayData shared_null[2];
    static dtkArrayData *sharedNull(void) {
        return const_cast<dtkArrayData *>(shared_null);
    }
};

Q_DECLARE_OPERATORS_FOR_FLAGS(dtkArrayData::AllocationOptions)

// /////////////////////////////////////////////////////////////////

inline size_t dtkArrayData::detachCapacity(size_t newSize) const
{
    if (capacityReserved && newSize < alloc)
        return alloc;

    return newSize;
}

inline dtkArrayData::AllocationOptions dtkArrayData::detachFlags(void) const
{
    AllocationOptions result;
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)

    if (!ref.isSharable())
        result |= Unsharable;

#endif

    if (capacityReserved)
        result |= CapacityReserved;

    return result;
}

inline dtkArrayData::AllocationOptions dtkArrayData::cloneFlags(void) const
{
    AllocationOptions result;

    if (capacityReserved)
        result |= CapacityReserved;

    return result;
}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

template <typename T> struct dtkTypedArrayData : dtkArrayData {
public:
    T *data(void)       {
        return static_cast<      T *>(dtkArrayData::data());
    }
    const T *data(void) const {
        return static_cast<const T *>(dtkArrayData::data());
    }

public:
    class iterator
    {
    public:
        T *i;
        typedef std::random_access_iterator_tag iterator_category;
        typedef qintptr difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T& reference;

    public:
        iterator(void) : i(0) {}
        iterator(T *n) : i(n) {}

    public:
        T& operator *  (void) const {
            return *i;
        }
        T *operator -> (void) const {
            return  i;
        }
        T& operator [] (qintptr j) const {
            return *(i + j);
        }
        bool operator == (const iterator& o) const {
            return i == o.i;
        }
        bool operator != (const iterator& o) const {
            return i != o.i;
        }
        bool operator <  (const iterator& other) const {
            return i <  other.i;
        }
        bool operator <= (const iterator& other) const {
            return i <= other.i;
        }
        bool operator >  (const iterator& other) const {
            return i >  other.i;
        }
        bool operator >= (const iterator& other) const {
            return i >= other.i;
        }
        iterator& operator ++ (void) {
            ++i;
            return *this;
        }
        iterator  operator ++ (int) {
            T *n = i;
            ++i;
            return n;
        }
        iterator& operator -- (void) {
            i--;
            return *this;
        }
        iterator  operator -- (int) {
            T *n = i;
            i--;
            return n;
        }
        iterator& operator += (qintptr j) {
            i += j;
            return *this;
        }
        iterator& operator -= (qintptr j) {
            i -= j;
            return *this;
        }
        iterator  operator +  (qintptr j) const {
            return iterator(i + j);
        }
        iterator  operator -  (qintptr j) const {
            return iterator(i - j);
        }
        qintptr operator -  (const iterator& j) const {
            return i - j.i;
        }
        operator T *(void) const {
            return i;
        }
    };
    friend class iterator;

public:
    class const_iterator
    {
    public:
        const T *i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qintptr difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T& reference;

    public:
        const_iterator(void) : i(0) {}
        const_iterator(const T *n) : i(n) {}
        explicit const_iterator(const iterator& o): i(o.i) {}

    public:
        const T& operator *  (void) const {
            return *i;
        }
        const T *operator -> (void) const {
            return  i;
        }
        const T& operator [] (qintptr j) const {
            return *(i + j);
        }
        bool operator == (const const_iterator& o) const {
            return i == o.i;
        }
        bool operator != (const const_iterator& o) const {
            return i != o.i;
        }
        bool operator <  (const const_iterator& other) const {
            return i <  other.i;
        }
        bool operator <= (const const_iterator& other) const {
            return i <= other.i;
        }
        bool operator >  (const const_iterator& other) const {
            return i >  other.i;
        }
        bool operator >= (const const_iterator& other) const {
            return i >= other.i;
        }
        const_iterator& operator ++ (void) {
            ++i;
            return *this;
        }
        const_iterator operator  ++ (int) {
            const T *n = i;
            ++i;
            return n;
        }
        const_iterator& operator -- (void) {
            i--;
            return *this;
        }
        const_iterator operator  -- (int) {
            const T *n = i;
            i--;
            return n;
        }
        const_iterator& operator += (qintptr j) {
            i += j;
            return *this;
        }
        const_iterator& operator -= (qintptr j) {
            i -= j;
            return *this;
        }
        const_iterator operator  +  (qintptr j) const {
            return const_iterator(i + j);
        }
        const_iterator operator  -  (qintptr j) const {
            return const_iterator(i - j);
        }
        qintptr      operator  -  (const const_iterator& j) const {
            return i - j.i;
        }
        operator const T *(void) const {
            return i;
        }
    };
    friend class const_iterator;

public:
    iterator      begin(iterator = iterator())                   {
        return data();
    }
    iterator        end(iterator = iterator())                   {
        return data() + size;
    }
    const_iterator      begin(const_iterator = const_iterator()) const {
        return data();
    }
    const_iterator        end(const_iterator = const_iterator()) const {
        return data() + size;
    }
    const_iterator constBegin(const_iterator = const_iterator()) const {
        return data();
    }
    const_iterator   constEnd(const_iterator = const_iterator()) const {
        return data() + size;
    }

public:
    class AlignmentDummy
    {
        dtkArrayData header;
        T data;
    };

public:
    dtkTypedArrayData(const dtkTypedArrayData& other);

public:
    static dtkTypedArrayData *allocate(qintptr capacity, AllocationOptions options = Default);

    static void deallocate(dtkArrayData *data);

    static dtkTypedArrayData *fromRawData(const T *data, qintptr n, AllocationOptions options = Default);

public:
    static dtkTypedArrayData *sharedNull(void);
    static dtkTypedArrayData *sharedEmpty(void);
    static dtkTypedArrayData *unsharableEmpty(void);
};

// /////////////////////////////////////////////////////////////////

template <typename T> inline dtkTypedArrayData<T>::dtkTypedArrayData(const dtkTypedArrayData& other) : dtkArrayData(other.ref.atomic.load(), other.size, other.alloc, other.capacityReserved, other.offset)
{}

template <typename T> inline dtkTypedArrayData<T> *dtkTypedArrayData<T>::allocate(qintptr capacity, AllocationOptions options)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    return static_cast<dtkTypedArrayData *>(dtkArrayData::allocate(sizeof(T), Q_ALIGNOF(AlignmentDummy), capacity, options));
}

template <typename T> inline void dtkTypedArrayData<T>::deallocate(dtkArrayData *data)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    dtkArrayData::deallocate(data, sizeof(T), Q_ALIGNOF(AlignmentDummy));
}

template <typename T> inline dtkTypedArrayData<T> *dtkTypedArrayData<T>::fromRawData(const T *data, qintptr n, AllocationOptions options)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    dtkTypedArrayData *result = allocate(0, options | RawData);

    if (result) {
        Q_ASSERT(!result->ref.isShared());

        result->offset = reinterpret_cast<const char *>(data) - reinterpret_cast<const char *>(result);
        result->size = n;
    }

    return result;
}

template <typename T> inline dtkTypedArrayData<T> *dtkTypedArrayData<T>::sharedNull(void)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    return static_cast<dtkTypedArrayData *>(dtkArrayData::sharedNull());
}

template <typename T> inline dtkTypedArrayData<T> *dtkTypedArrayData<T>::sharedEmpty(void)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    return allocate(/* capacity */ 0);
}

template <typename T> inline dtkTypedArrayData<T> *dtkTypedArrayData<T>::unsharableEmpty(void)
{
    Q_STATIC_ASSERT(sizeof(dtkTypedArrayData) == sizeof(dtkArrayData));

    return allocate(/* capacity */ 0, Unsharable);
}

//
// dtkArrayData.h ends here
