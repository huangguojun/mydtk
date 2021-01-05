// Version: $Id: 3c7547087fc8da498cfc83367ab3d3f7cb95c0b0 $
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

#include "dtkArrayData.h"
#include <iterator>
#include <string.h>
#include <vector>

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
// Preallocated data area for quickly building small arrays on the stack without
// malloc overhead
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize, size_t AlignT>
union dtkArrayAlignedPrealloc {
    char data[sizeof(T) * PreallocSize];
    qint64 q_for_alignment_1;
    double q_for_alignment_2;
};

// ///////////////////////////////////////////////////////////////////
// dtkArrayPrealloc base class
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize>
class dtkArrayPrealloc
{
public:
    dtkArrayAlignedPrealloc<T, PreallocSize, sizeof(T)> m_prealloc;

    T *preallocData(void) { return reinterpret_cast<T *>(m_prealloc.data); }

    bool isPreallocated(const T *start) const
    {
        return start == reinterpret_cast<const T *>(m_prealloc.data);
    }
};

template<typename T>
class dtkArrayPrealloc<T, 0>
{
public:
    T *preallocData(void) { return 0; }

    bool isPreallocated(const T *) const { return false; }
};

// ///////////////////////////////////////////////////////////////////
// dtkArray interface
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize = 8>
class dtkArray : private dtkArrayPrealloc<T, PreallocSize>
{
    typedef dtkTypedArrayData<T> Data;
    Data *d;
    bool isRawData;

public:
    typedef typename Data::iterator iterator;
    typedef typename Data::const_iterator const_iterator;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef qptrdiff difference_type;
    typedef qlonglong size_type;

public:
    dtkArray(void);
    explicit dtkArray(qlonglong size);
    dtkArray(qlonglong size, const T &value);
    dtkArray(const T *values, qlonglong size);
    dtkArray(std::initializer_list<T> args);
    dtkArray(const dtkArray &other);
#ifdef Q_COMPILER_RVALUE_REFS
    dtkArray(dtkArray &&other);
#endif

public:
    ~dtkArray(void);

public:
    dtkArray &operator=(const dtkArray &other);
#ifdef Q_COMPILER_RVALUE_REFS
    dtkArray &operator=(dtkArray &&other);
#endif

    bool operator==(const dtkArray &other) const;
    bool operator!=(const dtkArray &other) const;

public:
    template<qlonglong PreallocSizeOther>
    dtkArray(const dtkArray<T, PreallocSizeOther> &other);

    template<qlonglong PreallocSizeOther>
    dtkArray<T, PreallocSize> &operator=(const dtkArray<T, PreallocSizeOther> &other);

    template<qlonglong PreallocSizeOther>
    bool operator==(const dtkArray<T, PreallocSizeOther> &other) const;
    template<qlonglong PreallocSizeOther>
    bool operator!=(const dtkArray<T, PreallocSizeOther> &other) const;

    template<qlonglong PreallocSizeOther>
    bool isSharedWith(const dtkArray<T, PreallocSizeOther> &other) const;

public:
    void swap(dtkArray &other);

public:
    bool isDetached(void) const;
    void detach(void);
    bool isSharedWith(const dtkArray &other) const;
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)
    void setSharable(bool sharable);
#endif

public:
    bool empty(void) const;
    bool isEmpty(void) const;
    bool usePreallocation(void) const;

public:
    qlonglong size(void) const;
    qlonglong count(void) const;
    qlonglong length(void) const;
    qlonglong capacity(void) const;
    qlonglong preallocatedCapacity(void) const;

public:
    void clear(void);
    void resize(qlonglong size);
    void reserve(qlonglong size);
    void squeeze(void);

public:
    void append(const T &value);
    void append(const T &v1, const T &v2);
    void append(const T &v1, const T &v2, const T &v3);
    void append(const T &v1, const T &v2, const T &v3, const T &v4);
    void append(const T *values, qlonglong size);
    void append(std::initializer_list<T> args);
    void append(const dtkArray &other);

    void prepend(const T &value);

    void insert(qlonglong index, const T &value);
    void insert(qlonglong index, qlonglong length, const T &value);

    iterator insert(iterator before, const T &value);
    iterator insert(iterator before, qlonglong length, const T &value);

public:
    void setAt(qlonglong index, const T &value);
    void setAt(qlonglong index, const T *values, qlonglong length);

    void replace(qlonglong index, const T &value);
    void replace(qlonglong index, const T *values, qlonglong length);

    dtkArray &fill(const T &t, qlonglong size = -1);

    void setRawData(const T *raw_data, qlonglong size);
    void setWritableRawData(T *raw_data, qlonglong size);

public:
    static dtkArray fromRawData(const T *data, qlonglong size);
    static dtkArray fromWritableRawData(T *data, qlonglong size);

public:
    iterator erase(iterator begin, iterator end);
    iterator erase(iterator pos);

    void remove(qlonglong index);
    void remove(qlonglong index, qlonglong length);

    void removeFirst(void);
    void removeLast(void);
    void removeAt(qlonglong index);

    T takeFirst(void);
    T takeLast(void);
    T takeAt(qlonglong index);

public:
    void reverse(void);
    dtkArray reversed(void) const;

public:
    dtkArray &operator+=(const T &value);
    dtkArray &operator+=(const dtkArray &other);
    dtkArray &operator<<(const T &value);
    dtkArray &operator<<(const dtkArray &other);

public:
    template<qlonglong PreallocSizeOther>
    void append(const dtkArray<T, PreallocSizeOther> &other);
    template<qlonglong PreallocSizeOther>
    dtkArray<T, PreallocSize> &operator+=(const dtkArray<T, PreallocSizeOther> &other);
    template<qlonglong PreallocSizeOther>
    dtkArray<T, PreallocSize> &operator<<(const dtkArray<T, PreallocSizeOther> &other);

public:
    const T &at(qlonglong index) const;
    const T &operator[](qlonglong index) const;
    T &operator[](qlonglong index);

    T &first(void);
    const T &first(void) const;
    T &last(void);
    const T &last(void) const;

    T value(qlonglong index) const;
    T value(qlonglong index, const T &defaultValue) const;

public:
    bool startsWith(const T &t) const;
    bool endsWith(const T &t) const;
    bool contains(const T &t) const;

    qlonglong indexOf(const T &t, qlonglong from = 0) const;
    qlonglong lastIndexOf(const T &t, qlonglong from = -1) const;
    qlonglong count(const T &t) const;

    dtkArray mid(qlonglong pos, qlonglong length = -1) const;
    dtkArray left(qlonglong length) const;
    dtkArray right(qlonglong length) const;

public:
    void push_back(const T &t) { append(t); }
    void push_front(const T &t) { prepend(t); }
    void pop_back(void) { removeLast(); }
    void pop_front(void) { removeFirst(); }
    reference front(void) { return first(); }
    const_reference front(void) const { return first(); }
    reference back(void) { return last(); }
    const_reference back(void) const { return last(); }

public:
    T *data(void)
    {
        detach();
        return d->begin();
    }
    const T *data(void) const { return d->begin(); }
    const T *constData(void) const { return d->begin(); }

public:
    iterator begin(iterator = iterator())
    {
        detach();
        return d->begin();
    }
    const_iterator begin(const_iterator = const_iterator()) const { return d->constBegin(); }
    const_iterator cbegin(const_iterator = const_iterator()) const { return d->constBegin(); }
    const_iterator constBegin(const_iterator = const_iterator()) const { return d->constBegin(); }

    iterator end(iterator = iterator())
    {
        detach();
        return d->end();
    }
    const_iterator end(const_iterator = const_iterator()) const { return d->constEnd(); }
    const_iterator cend(const_iterator = const_iterator()) const { return d->constEnd(); }

    const_iterator constEnd(const_iterator = const_iterator()) const { return d->constEnd(); }

private:
    template<typename, qlonglong>
    friend class dtkArray;

    void copyData(Data *other_d);

    void reallocData(const qlonglong size, const qlonglong alloc,
                     dtkArrayData::AllocationOptions options = dtkArrayData::Default);
    void reallocData(const qlonglong size);
    void freeData(Data *d);

    void defaultConstruct(T *from, T *to);
    void copyConstruct(const T *srcFrom, const T *srcTo, T *dstFrom);
    void destruct(T *from, T *to);

    bool isValidIterator(const iterator &i) const;
};

// ///////////////////////////////////////////////////////////////////
// Helpers
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize>
QDebug &operator<<(QDebug debug, const dtkArray<T, PreallocSize> &array);

template<typename T, qlonglong PreallocSize>
QDataStream &operator<<(QDataStream &s, const dtkArray<T, PreallocSize> &array);
template<typename T, qlonglong PreallocSize>
QDataStream &operator>>(QDataStream &s, dtkArray<T, PreallocSize> &array);

template<typename T, qlonglong PreallocSize>
QDataStream &operator<<(QDataStream &s, const dtkArray<T *, PreallocSize> &array);
template<typename T, qlonglong PreallocSize>
QDataStream &operator>>(QDataStream &s, dtkArray<T *, PreallocSize> &array);

// ///////////////////////////////////////////////////////////////////
// Alias when c++1 is available
// ///////////////////////////////////////////////////////////////////

#ifdef Q_COMPILER_TEMPLATE_ALIAS
template<typename T>
using dtkArrayDynamic = dtkArray<T, 0>;
#endif

// ///////////////////////////////////////////////////////////////////
// Implementation
// ///////////////////////////////////////////////////////////////////

#include "dtkArray.tpp"

// /////////////////////////////////////////////////////////////////
// Credits
// /////////////////////////////////////////////////////////////////

/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
// dtkArray.h ends here
