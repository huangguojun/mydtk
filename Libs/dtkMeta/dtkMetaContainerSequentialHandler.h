// Version: $Id: 5e5aa4d47fea6e9627fba9b3eb2360d5dab7e3d7 $
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
#include <dtkMetaExport.h>

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandler
// ///////////////////////////////////////////////////////////////////

class DTKMETA_EXPORT dtkMetaContainerSequentialHandler
{
public:
    struct DTKMETA_EXPORT iterator
    {
    public:
        virtual ~iterator(void) {}

    public:
        virtual iterator *clone(void) const = 0;
        virtual void copy(const iterator &o) = 0;
        virtual bool equal(const iterator &o) const = 0;

    public:
        virtual void advance(void) = 0;
        virtual void moveForward(qlonglong step) = 0;
        virtual void moveBackward(qlonglong step) = 0;

    public:
        virtual void *value(void) const = 0;
        virtual QVariant variant(void) const = 0;

    public:
        virtual void assign(const void *t) = 0;
        virtual void addAssign(const void *t) = 0;
        virtual void subAssign(const void *t) = 0;
        virtual void mulAssign(const void *t) = 0;
        virtual void divAssign(const void *t) = 0;

    public:
        virtual bool equalToValue(const void *t) const = 0;
    };

public:
    struct DTKMETA_EXPORT const_iterator
    {
    public:
        virtual ~const_iterator(void) {}

    public:
        virtual const_iterator *clone(void) const = 0;
        virtual void copy(const const_iterator &o) = 0;
        virtual bool equal(const const_iterator &o) const = 0;

    public:
        virtual void advance(void) = 0;
        virtual void moveForward(qlonglong step) = 0;
        virtual void moveBackward(qlonglong step) = 0;

    public:
        virtual const void *value(void) const = 0;
        virtual QVariant variant(void) const = 0;
    };

public:
    virtual ~dtkMetaContainerSequentialHandler(void) {}

public:
    virtual QString description(void) const = 0;

public:
    virtual bool hasBiDirectionalIterator(void) const = 0;
    virtual bool hasRandomAccessIterator(void) const = 0;

public:
    virtual iterator *begin(void) = 0;
    virtual const_iterator *cbegin(void) const = 0;
    virtual iterator *end(void) = 0;
    virtual const_iterator *cend(void) const = 0;

public:
    virtual bool empty(void) const = 0;
    virtual qlonglong size(void) const = 0;

public:
    virtual void clear(void) = 0;
    virtual void reserve(qlonglong size) = 0;
    virtual void resize(qlonglong size) = 0;

public:
    virtual void append(const void *t) = 0;
    virtual void prepend(const void *t) = 0;
    virtual void insert(qlonglong idx, const void *t) = 0;

public:
    virtual void setAt(qlonglong idx, const void *t) = 0;
    virtual void removeAt(qlonglong idx) = 0;

public:
    virtual const void *at(qlonglong idx) const = 0;
    virtual void *at(qlonglong idx) = 0;

public:
    virtual iterator *iteratorAt(qlonglong idx, iterator *pit) = 0;
    virtual QVariant &variantAt(qlonglong idx, QVariant &var) = 0;

public:
    int value_type_id;
};

Q_DECLARE_METATYPE(dtkMetaContainerSequentialHandler *)

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerTemplate
// ///////////////////////////////////////////////////////////////////

template<typename T>
class dtkMetaContainerSequentialHandlerTemplate : public dtkMetaContainerSequentialHandler
{
public:
    typedef typename T::value_type ValueType;
    typedef typename T::iterator Iterator;
    typedef typename T::const_iterator ConstIterator;

public:
    typedef dtkMetaContainerSequentialHandler::iterator HandlerIterator;
    typedef dtkMetaContainerSequentialHandler::const_iterator HandlerConstIterator;

public:
    struct iterator : public HandlerIterator
    {
    public:
        Iterator it;

    public:
        iterator(const Iterator &iterator);

    public:
        HandlerIterator *clone(void) const;
        void copy(const HandlerIterator &o);
        bool equal(const HandlerIterator &o) const;

    public:
        void advance(void);
        void moveForward(qlonglong step);
        void moveBackward(qlonglong step);

    public:
        void *value(void) const;
        QVariant variant(void) const;

    public:
        void assign(const void *t);
        void addAssign(const void *t);
        void subAssign(const void *t);
        void mulAssign(const void *t);
        void divAssign(const void *t);

    public:
        bool equalToValue(const void *t) const;
    };

public:
    struct const_iterator : public HandlerConstIterator
    {
    public:
        ConstIterator it;

    public:
        const_iterator(const ConstIterator &iterator);

    public:
        HandlerConstIterator *clone(void) const;
        void copy(const HandlerConstIterator &o);
        bool equal(const HandlerConstIterator &o) const;

    public:
        void advance(void);
        void moveForward(qlonglong step);
        void moveBackward(qlonglong step);

    public:
        const void *value(void) const;
        QVariant variant(void) const;
    };

public:
    HandlerIterator *begin(void);
    HandlerConstIterator *cbegin(void) const;
    HandlerIterator *end(void);
    HandlerConstIterator *cend(void) const;

public:
    explicit dtkMetaContainerSequentialHandlerTemplate(T *c);
    ~dtkMetaContainerSequentialHandlerTemplate(void);

public:
    QString description(void) const;

public:
    bool hasBiDirectionalIterator(void) const;
    bool hasRandomAccessIterator(void) const;

public:
    bool empty(void) const;
    qlonglong size(void) const;

public:
    void clear(void);
    void reserve(qlonglong size);
    void resize(qlonglong size);

public:
    void append(const void *t);
    void prepend(const void *t);
    void insert(qlonglong idx, const void *t);

public:
    void setAt(qlonglong idx, const void *t);
    void removeAt(qlonglong idx);

public:
    const void *at(qlonglong idx) const;
    void *at(qlonglong idx);

public:
    HandlerIterator *iteratorAt(qlonglong idx, HandlerIterator *pit);
    QVariant &variantAt(qlonglong idx, QVariant &var);

private:
    T *m_container;
};

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialHandlerHelper
// ///////////////////////////////////////////////////////////////////

template<typename T>
struct dtkMetaContainerSequentialHandlerHelper
{
    typedef typename T::value_type ValueType;
    typedef typename T::iterator Iterator;

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaIteratorIsBidirectional<typename U::iterator>::value,
                                   bool>::type
    hasBidirectionalIterator(void)
    {
        return true;
    }
    template<typename U = T>
    static typename std::enable_if<!dtkMetaIteratorIsBidirectional<typename U::iterator>::value,
                                   bool>::type
    hasBidirectionalIterator(void)
    {
        return false;
    }

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaIteratorIsRandomAccess<typename U::iterator>::value,
                                   bool>::type
    hasRandomAccesIterator(void)
    {
        return true;
    }
    template<typename U = T>
    static typename std::enable_if<!dtkMetaIteratorIsRandomAccess<typename U::iterator>::value,
                                   bool>::type
    hasRandomAccesIterator(void)
    {
        return false;
    }

public:
    static bool empty(const T *c);
    static qlonglong size(const T *c);

public:
    static void clear(T *c);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaContainerIsReservable<U>::value>::type
    reserve(T *c, qlonglong size);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaContainerIsReservable<U>::value>::type
    reserve(T *c, qlonglong size);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaContainerIsResizable<U>::value>::type
    resize(T *c, qlonglong size);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaContainerIsResizable<U>::value>::type
    resize(T *c, qlonglong size);

public:
    static void insert(T *c, qlonglong idx, const ValueType &t);
    static void setAt(T *c, qlonglong idx, const ValueType &t);
    static void removeAt(T *c, qlonglong idx);

public:
    static const ValueType *at(const T *c, qlonglong idx);
    static ValueType *at(T *c, qlonglong idx);

public:
    static void iteratorAt(T *c, qlonglong idx, Iterator &it);

public:
    static QVariant &variantAt(const T *c, qlonglong idx, QVariant &var);
};

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialItemHelper
// ///////////////////////////////////////////////////////////////////

template<typename T>
struct dtkMetaContainerSequentialIteratorHelper
{
public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaTypeIsAssignable<U>::value>::type assign(T &lhs,
                                                                                   const T &rhs);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaTypeIsAssignable<U>::value>::type assign(T &, const T &);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaTypeIsAddAssignable<U>::value>::type
    addAssign(T &lhs, const T &rhs);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaTypeIsAddAssignable<U>::value>::type
    addAssign(T &, const T &);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaTypeIsSubAssignable<U>::value>::type
    subAssign(T &lhs, const T &rhs);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaTypeIsSubAssignable<U>::value>::type
    subAssign(T &, const T &);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaTypeIsMulAssignable<U>::value>::type
    mulAssign(T &lhs, const T &rhs);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaTypeIsMulAssignable<U>::value>::type
    mulAssign(T &, const T &);

public:
    template<typename U = T>
    static typename std::enable_if<dtkMetaTypeIsDivAssignable<U>::value>::type
    divAssign(T &lhs, const T &rhs);
    template<typename U = T>
    static typename std::enable_if<!dtkMetaTypeIsDivAssignable<U>::value>::type
    divAssign(T &, const T &);
};

// /////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialPrivate::convertFunctor for QMetatype system
// /////////////////////////////////////////////////////////////////

template<typename From>
struct dtkMetaContainerSequentialConvertFunctor
{
};

template<typename From>
struct dtkMetaContainerSequentialConvertFunctor<From *>
{
    dtkMetaContainerSequentialHandler *operator()(From *f) const;
};

// ///////////////////////////////////////////////////////////////////
// dtkMetaContainerSequentialRegisterConverter
// ///////////////////////////////////////////////////////////////////

template<typename T,
         bool = QMetaTypeId2<typename std::remove_pointer<T>::type::value_type>::Defined>
struct dtkContainerSequentialValueTypeIsMetaType
{
    static bool record(int) { return false; }
};

template<typename T>
struct dtkContainerSequentialValueTypeIsMetaType<T, true>
{
    static bool record(int id);
};

template<typename T, bool = dtkMetaTypeIsSequentialContainerPointer<T>::value>
struct dtkMetaContainerSequentialRegisterConverter
{
    static bool record(int) { return false; }
};

template<typename T>
struct dtkMetaContainerSequentialRegisterConverter<T, true>
    : public dtkContainerSequentialValueTypeIsMetaType<T>
{
};

// /////////////////////////////////////////////////////////////////

#include "dtkMetaContainerSequentialHandler.tpp"

//
// dtkMetaContainerSequentialHandler.h ends here
