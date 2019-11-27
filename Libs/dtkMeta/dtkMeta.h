// Version: $Id: e7e953acf03afce48eb7703c9c7af7c3ff3976c9 $
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

#include <dtkMetaExport.h>

#include "dtkMetaTypeTraits.h"

#include <type_traits>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeHandler definition
// /////////////////////////////////////////////////////////////////

template< typename T, bool = QtPrivate::IsPointerToTypeDerivedFromQObject<T>::Value >
struct dtkMetaTypeHandler {
    static bool canConvert(const QList<int>& types);
};

template<typename T> struct dtkMetaTypeHandler<T *, false> {
    static     bool  canConvert(const QList<int>& types);
    static QVariant  variantFromValue(T *t);
    static        T *clone(T *t);
    static     void  copy(T *source, T *target);
};

template<typename T> struct dtkMetaTypeHandler<T *, true> {
    static     bool  canConvert(const QList<int>& types);
    static QVariant  variantFromValue(T *t);
    static        T *clone(T *t);
    static     void  copy(T *source, T *target);
};

template< typename T, bool B> struct dtkMetaTypeHandlerHelper;

template< typename T> struct dtkMetaTypeHandlerHelper<T *, false> {
    static bool  canConvert(const QList<int>& types);
    static    T *clone(T *t);
    static void  copy(T *source, T *target);
};

template< typename T> struct dtkMetaTypeHandlerHelper<T *, true> {
    static bool  canConvert(const QList<int>& types);
    static    T *clone(T *);
    static void  copy(T *source, T *target);
};

// /////////////////////////////////////////////////////////////////
// dtkMetaType definition
// /////////////////////////////////////////////////////////////////

class dtkMetaType
{
public:
    template <typename T> static     bool canConvert(int type);
    template <typename T> static     bool canConvert(const QList<int>& types);
    template <typename T> static QVariant variantFromValue(const T& t);
    template <typename T> static QVariant variantFromValue(      T *t);
    template <typename T> static        T *clone(T *t);
    template <typename T> static     void copy(T *source, T *target);
    template <typename T> static     bool registerContainerPointerConverter(int id);

public:
    static DTKMETA_EXPORT  QString description(const QVariant& v);
    static DTKMETA_EXPORT QVariant cloneContent(const QVariant& v);
    static DTKMETA_EXPORT QVariant createEmptyContainer(const QVariant& v);
    static DTKMETA_EXPORT     bool destroyPointer(QVariant& v);
};

// /////////////////////////////////////////////////////////////////
// Stream operators redefinition
// /////////////////////////////////////////////////////////////////

template <typename T> QDataStream& operator << (QDataStream& s, T   *t);
template <typename T> QDataStream& operator >> (QDataStream& s, T *&t);

template<typename T> QDataStream& operator << (QDataStream& s, const QList<T *>& l);
template<typename T> QDataStream& operator >> (QDataStream& s,       QList<T *>& l);

template<typename T> QDataStream& operator << (QDataStream& s, const QVector<T *>& v);
template<typename T> QDataStream& operator >> (QDataStream& s,       QVector<T *>& v);

template<typename T> QDataStream& operator << (QDataStream& s, const std::list<T *>& l);
template<typename T> QDataStream& operator >> (QDataStream& s,       std::list<T *>& l);

template<typename T> QDataStream& operator << (QDataStream& s, const std::vector<T *>& v);
template<typename T> QDataStream& operator >> (QDataStream& s,       std::vector<T *>& v);

// /////////////////////////////////////////////////////////////////
// MACRO REGISTERING POINTERS OF CONTAINERS INTO METATYPE SYSTEM
// /////////////////////////////////////////////////////////////////

#define DTK_DECLARE_SEQUENTIAL_CONTAINER_POINTER(CONTAINER_ARG)                   \
template <typename T> struct QMetaTypeId< CONTAINER_ARG<T> *>                     \
{                                                                                 \
    enum {                                                                        \
        Defined = QMetaTypeId2<T>::Defined                                        \
    };                                                                            \
    static int qt_metatype_id()                                                   \
    {                                                                             \
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);       \
        if (const int id = metatype_id.load())                                    \
            return id;                                                            \
        const char *tName = QMetaType::typeName(qMetaTypeId<T>());                \
        Q_ASSERT(tName);                                                          \
        const int tNameLen = int(qstrlen(tName));                                 \
        QByteArray typeName;                                                      \
        typeName.reserve(int(sizeof(#CONTAINER_ARG)) + 1 + tNameLen + 1 + 1 + 1); \
        typeName.append(#CONTAINER_ARG, int(sizeof(#CONTAINER_ARG)) - 1)          \
            .append('<').append(tName, tNameLen).append('>');                     \
        typeName.append('*');                                                     \
        const int newId = qRegisterNormalizedMetaType< CONTAINER_ARG<T> *>(       \
                        typeName,                                                 \
                        reinterpret_cast< CONTAINER_ARG<T> **>(quintptr(-1)));    \
        metatype_id.storeRelease(newId);                                          \
        if (newId > 0) {                                                          \
            dtkMetaType::registerContainerPointerConverter< CONTAINER_ARG<T> * >(newId); \
        }                                                                         \
        return newId;                                                             \
    }                                                                             \
};                                                                                \
template<typename T> struct dtkMetaTypeIsSequentialContainerPointer< CONTAINER_ARG<T> *> : std::true_type {};

// /////////////////////////////////////////////////////////////////

#include "dtkMeta.tpp"

//
// dtkMeta.h ends here
