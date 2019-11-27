// Version: $Id: 54876802f986e6246b1ab4ff9c5a7e3777237a4d $
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

#include "dtkMeta.h"
#include "dtkMetaContainerSequentialHandler.h"
#include <dtkMetaExport.h>

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequential definition
// /////////////////////////////////////////////////////////////////

class DTKMETA_EXPORT dtkMetaContainerSequential
{
    typedef dtkMetaContainerSequentialHandler::iterator       HandlerIterator;
    typedef dtkMetaContainerSequentialHandler::const_iterator HandlerConstIterator;

public:
    struct iterator;
    struct DTKMETA_EXPORT item {
        HandlerIterator *it;
    private:
        friend class dtkMetaContainerSequential;
        friend struct iterator;

    private:
        explicit item(HandlerIterator *iterator);

    public:
        item(const item& o) = delete;
        item(item&& o);
        ~item(void);

    public:
        item& operator = (const item& o);
        item& operator = (item&& o);

    public:
        bool operator == (const item& o) const;
        bool operator != (const item& o) const;

    public:
        const QVariant value(void) const;

    public:
        template <typename T> const T& value(void) const;

    public:
        template <typename T> item& operator  = (const T& t);
        template <typename T> item& operator += (const T& t);
        template <typename T> item& operator -= (const T& t);
        template <typename T> item& operator *= (const T& t);
        template <typename T> item& operator /= (const T& t);

    public:
        template <typename T> bool operator == (const T& t) const;
        template <typename T> bool operator != (const T& t) const;
    };

public:
    struct DTKMETA_EXPORT iterator {
    private:
        friend class dtkMetaContainerSequential;
        item proxy;

    private:
        explicit iterator(HandlerIterator *iterator);

    public:
        iterator(const iterator& o);
        iterator(iterator&& o);
        ~iterator(void);

    public:
        iterator& operator = (const iterator& o);
        iterator& operator = (iterator&& o);

    public:
        bool operator == (const iterator& o) const;
        bool operator != (const iterator& o) const;

    public:
        item& operator *  (void);
        item& operator [] (qlonglong j);

    public:
        template <typename T> T& value(void);

    public:
        iterator& operator ++ (void);
        iterator  operator ++ (int);
        iterator& operator -- (void);
        iterator  operator -- (int);
        iterator& operator += (qlonglong j);
        iterator& operator -= (qlonglong j);
        iterator  operator +  (qlonglong j) const;
        iterator  operator -  (qlonglong j) const;
    };

public:
    struct DTKMETA_EXPORT const_iterator {
    private:
        friend class dtkMetaContainerSequential;
        HandlerConstIterator *it;

    private:
        explicit const_iterator(HandlerConstIterator *iterator);

    public:
        const_iterator(const const_iterator& o);
        const_iterator(const_iterator&& o);
        ~const_iterator(void);

    public:
        const_iterator& operator = (const const_iterator& o);
        const_iterator& operator = (const_iterator&& o);

    public:
        bool operator == (const const_iterator& o) const;
        bool operator != (const const_iterator& o) const;

    public:
        QVariant operator *  (void)        const;
        QVariant operator [] (qlonglong j) const;

    public:
        const_iterator& operator ++ (void);
        const_iterator  operator ++ (int);
        const_iterator& operator -- (void);
        const_iterator  operator -- (int);
        const_iterator& operator += (qlonglong j);
        const_iterator& operator -= (qlonglong j);
        const_iterator  operator +  (qlonglong j) const;
        const_iterator  operator -  (qlonglong j) const;
    };

public:
    dtkMetaContainerSequential(dtkMetaContainerSequentialHandler *handler);
    dtkMetaContainerSequential(dtkMetaContainerSequential&& o);

public:
    ~dtkMetaContainerSequential(void);

public:
    iterator  begin(void);
    const_iterator  begin(void) const;
    const_iterator cbegin(void) const;
    iterator    end(void);
    const_iterator    end(void) const;
    const_iterator   cend(void) const;

public:
    bool hasBiDirectionalIterator(void) const;
    bool  hasRandomAccessIterator(void) const;

public:
    bool      empty(void) const;
    qlonglong  size(void) const;

public:
    void   clear(void);
    void reserve(qlonglong size);
    void  resize(qlonglong size);

public:
    template <typename T> void  append(const T& t);
    template <typename T> void prepend(const T& t);

public:
    void  append(const QVariant& v);
    void prepend(const QVariant& v);

public:
    template <typename T> void insert(qlonglong idx, const T& t);
    template <typename T> void  setAt(qlonglong idx, const T& t);

public:
    void insert(qlonglong idx, const QVariant& v);
    void  setAt(qlonglong idx, const QVariant& v);

public:
    const QVariant& at(qlonglong idx) const;

public:
    template <typename T> const T& at(qlonglong idx) const;

public:
    void removeAt(qlonglong idx);

public:
    const QVariant& first(void) const;
    item& first(void);

    const QVariant& last(void) const;
    item& last(void);

    const QVariant& operator [] (qlonglong idx) const;
    item& operator [] (qlonglong idx);

protected:
    friend QDebug& operator << (QDebug debug, const dtkMetaContainerSequential& container);

protected:
    dtkMetaContainerSequentialHandler *h;
    item *proxy;
    mutable QVariant var;
};

// ///////////////////////////////////////////////////////////////////
// Debug operator
// ///////////////////////////////////////////////////////////////////

QDebug& operator << (QDebug debug, const dtkMetaContainerSequential& container);

// /////////////////////////////////////////////////////////////////
// Register standard containers as a sequential container pointer
// /////////////////////////////////////////////////////////////////

DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(QList);
DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(QVector);
DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(QVarLengthArray);
DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(std::list);
DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(std::vector);

// /////////////////////////////////////////////////////////////////
// Register QStringList* as a sequential container pointer
// /////////////////////////////////////////////////////////////////

template <> struct QMetaTypeId<QStringList *> {
    enum {
        Defined = QMetaTypeId2<QString>::Defined
    };

    static int qt_metatype_id() {
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);

        if (const int id = metatype_id.load())
            return id;

        QByteArray typeName("QStringList*", 12);

        const int newId = qRegisterNormalizedMetaType<QStringList *>(typeName, reinterpret_cast<QStringList **>(quintptr(-1)));
        metatype_id.storeRelease(newId);

        if (newId > 0) {
            dtkMetaType::registerContainerPointerConverter<QStringList *>(newId);
        }

        return newId;
    }
};

template<> struct dtkMetaTypeIsSequentialContainerPointer<QStringList *> : std::true_type {};


// /////////////////////////////////////////////////////////////////

#include "dtkMetaContainerSequential.tpp"

//
// dtkMetaContainerSequential.h ends here
