// Version: $Id: 68f29d62e2aaf2d8fbb9b316509faca9cc1830c4 $
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

#include <dtkMathExport.h>

#include <QtCore/qatomic.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qglobal.h>

#include <string.h>

// ///////////////////////////////////////////////////////////////////
// Preallocated data area for quickly building small arrays on the stack without
// malloc overhead
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize, size_t AlignT>
union dtkMathArrayAlignedPrealloc {
    char data[sizeof(T) * PreallocSize];
    qint64 q_for_alignment_1;
    double q_for_alignment_2;
};

// ///////////////////////////////////////////////////////////////////
// dtkMathArrayPrealloc base class
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize>
class dtkMathArrayPrealloc
{
public:
    dtkMathArrayAlignedPrealloc<T, PreallocSize, sizeof(T)> m_prealloc;

    inline T *prealloc(void) { return reinterpret_cast<T *>(m_prealloc.data); }

    inline bool isPrealloc(const T *start) const
    {
        return start == reinterpret_cast<const T *>(m_prealloc.data);
    }
};

template<typename T>
class dtkMathArrayPrealloc<T, 0>
{
public:
    inline T *prealloc(void) { return 0; }

    inline bool isPrealloc(const T *start) const
    {
        Q_UNUSED(start);
        return false;
    }
};

// ///////////////////////////////////////////////////////////////////
// dtkMathArray interface
// ///////////////////////////////////////////////////////////////////

template<typename T, qlonglong PreallocSize = 8>
class dtkMathArray : private dtkMathArrayPrealloc<T, PreallocSize>
{
public:
    enum RawDataType { ReadOnly = 0x001, Writable = 0x002 };

public:
    dtkMathArray(void);
    explicit dtkMathArray(qlonglong arraySize);
    dtkMathArray(qlonglong size, const T &value);
    dtkMathArray(const T *values, qlonglong size);
    dtkMathArray(const dtkMathArray<T, PreallocSize> &other);

public:
    ~dtkMathArray(void);

public:
    typedef T *iterator;
    typedef const T *const_iterator;

    dtkMathArray<T, PreallocSize> &operator=(const dtkMathArray<T, PreallocSize> &other);

    qlonglong size(void) const;
    qlonglong count(void) const;
    qlonglong capacity(void) const;

    bool isEmpty(void) const;

    bool isDetached(void) const;

    void detach(void);

    void clear(void);

    const T &at(qlonglong index) const;
    const T &operator[](qlonglong index) const;
    T &operator[](qlonglong index);

    T value(qlonglong index) const;
    T value(qlonglong index, const T &defaultValue) const;

    T *extend(qlonglong size);

    void append(const T &value);
    void append(const T &value1, const T &value2);
    void append(const T &value1, const T &value2, const T &value3);
    void append(const T &value1, const T &value2, const T &value3, const T &value4);
    void append(const T *values, qlonglong count);
    void append(const dtkMathArray<T, PreallocSize> &other);

    void prepend(const T &value);

    void insert(qlonglong index, const T &value);
    void insert(qlonglong index, qlonglong count, const T &value);
    iterator insert(iterator before, qlonglong count, const T &value);
    iterator insert(iterator before, const T &value);

    void replace(qlonglong index, const T &newValue);
    void replace(qlonglong index, const T *values, qlonglong countToAdd);

    void remove(qlonglong index);
    void remove(qlonglong index, qlonglong countToRemove);
    void removeFirst() { remove(0); }
    void removeLast() { remove(size() - 1); }

    iterator erase(iterator begin, iterator end);
    iterator erase(iterator pos);

    qlonglong indexOf(const T &value, qlonglong from = 0) const;
    qlonglong lastIndexOf(const T &value, qlonglong from = -1) const;
    bool contains(const T &value) const;
    qlonglong count(const T &value) const;

    void resize(qlonglong size);
    void reserve(qlonglong size);
    void squeeze(void);

    dtkMathArray<T, PreallocSize> &fill(const T &fillValue, qlonglong fillCount = -1);

    void reverse(void);
    dtkMathArray<T, PreallocSize> reversed(void) const;

    dtkMathArray<T, PreallocSize> mid(qlonglong index, qlonglong length = -1) const;
    dtkMathArray<T, PreallocSize> left(qlonglong length) const;
    dtkMathArray<T, PreallocSize> right(qlonglong length) const;

    void setRawData(const T *raw_data, qlonglong size, RawDataType data_type = ReadOnly);

    T *rawData(void);
    const T *rawData(void) const;
    const T *constRawData(void) const;

    static dtkMathArray<T, PreallocSize> fromRawData(const T *data, qlonglong size);
    static dtkMathArray<T, PreallocSize> fromWritableRawData(T *data, qlonglong size);

    bool operator==(const dtkMathArray<T, PreallocSize> &other) const;
    bool operator!=(const dtkMathArray<T, PreallocSize> &other) const;

    dtkMathArray<T, PreallocSize> &operator+=(const T &value);
    dtkMathArray<T, PreallocSize> &operator+=(const dtkMathArray<T, PreallocSize> &other);
    dtkMathArray<T, PreallocSize> &operator<<(const T &value);
    dtkMathArray<T, PreallocSize> &operator<<(const dtkMathArray<T, PreallocSize> &other);

    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef ptrdiff_t difference_type;
    typedef qlonglong size_type;

    inline iterator begin(void) { return rawData(); }
    inline const_iterator begin(void) const { return constRawData(); }
    inline const_iterator constBegin(void) const { return constRawData(); }
    inline iterator end(void) { return rawData() + size(); }
    inline const_iterator end(void) const { return constRawData() + size(); }
    inline const_iterator constEnd(void) const { return constRawData() + size(); }

    inline T &first(void)
    {
        Q_ASSERT(!isEmpty());
        return *begin();
    }
    inline const T &first(void) const
    {
        Q_ASSERT(!isEmpty());
        return *begin();
    }
    inline T &last(void)
    {
        Q_ASSERT(!isEmpty());
        return *(end() - 1);
    }
    inline const T &last(void) const
    {
        Q_ASSERT(!isEmpty());
        return *(end() - 1);
    }
    inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }

    inline void push_back(const T &newValue) { append(newValue); }
    inline void push_front(const T &newValue) { prepend(newValue); }
    inline void pop_back(void)
    {
        Q_ASSERT(!isEmpty());
        removeLast();
    }
    inline void pop_front(void)
    {
        Q_ASSERT(!isEmpty());
        removeFirst();
    }
    inline bool empty(void) const { return isEmpty(); }
    inline reference front(void) { return first(); }
    inline const_reference front(void) const { return first(); }
    inline reference back(void) { return last(); }
    inline const_reference back(void) const { return last(); }

private:
    struct Data
    {
        QBasicAtomicInt ref;
        qlonglong capacity;
        T array[1];
    };

    // Invariants:
    // 1. If the data is not shared, then the usual condition is
    //    for m_limit >= m_end.
    // 2. If the data is shared, then m_limit == m_start.
    //    This triggers the range check in append() to call grow(),
    //    which will copy-on-write.  It also triggers the detach
    //    check in rawData() and operator[] to cause a copy-on-write.
    // 3. If the data is not shared, but previously was, then
    //    m_limit == m_start.  This will trigger grow() or
    //    detach(), which may then notice that it doesn't have to
    //    copy-on-write.  In that case, m_limit is set back
    //    to m_start + m_data->capacity.
    // 4. If m_data is null, then m_start is either the same as
    //    m_prealloc, or it points at raw data (const or non-const).
    // 5. If the array contains const raw data, then m_limit will
    //    be set to m_start to force copy-on-write.
    T *m_start;
    T *m_end;
    mutable T *m_limit;
    Data *m_data;

    inline void initPrealloc(void)
    {
        m_end = m_start = dtkMathArrayPrealloc<T, PreallocSize>::prealloc();
        m_limit = m_start + PreallocSize;
    }

    dtkMathArray(const T *data, qlonglong size, bool isWritable);

    void free(T *data, qlonglong count);
    void release(void);
    void copyReplace(T *dst, const T *src, qlonglong count);
    Data *copyData(const T *src, qlonglong size, qlonglong capacity);
    void reallocate(qlonglong capacity);
    void detach_helper(void);
    void assign(const dtkMathArray<T, PreallocSize> &other);
    void grow(qlonglong needed);
    void setSize(qlonglong newSize);
};

DTKMATH_EXPORT qlonglong dtkMathArrayAllocMore(qlonglong alloc, qlonglong extra, qlonglong sizeOfT);

// ///////////////////////////////////////////////////////////////////
// dtkMathArray implementation
// ///////////////////////////////////////////////////////////////////

#include "dtkMathArray.tpp"

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
// dtkMathArray.h ends here
