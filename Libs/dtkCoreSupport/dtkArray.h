/* dtkArray.h ---
 *
 * Author: Thibaud Kloczko
 * Created: Mon Jul  1 11:58:29 2013 (+0200)
 * Version:
 * Last-Updated: mer. sept. 17 14:53:19 2014 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 148
 */

/* Commentary:
 *
 *  Credits see EOF
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkCoreSupportExport.h>
#include "dtkGlobal.h"

#include <QtCore/qglobal.h>
#include <QtCore/qatomic.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <string.h>

// ///////////////////////////////////////////////////////////////////
// Preallocated data area for quickly building small arrays on the stack without malloc overhead
// ///////////////////////////////////////////////////////////////////

#if defined(Q_DECL_ALIGN) && defined(Q_ALIGNOF)

#if defined(Q_CC_GNU) && (__GNUC__ >= 4 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
typedef char __attribute__((__may_alias__)) dtkArrayAlignedChar;
#else
typedef char dtkArrayAlignedChar;
#endif

template <typename T, int PreallocSize, size_t AlignT> struct dtkArrayAlignedPrealloc;

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 1> {
    dtkArrayAlignedChar Q_DECL_ALIGN(1) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 2> {
    dtkArrayAlignedChar Q_DECL_ALIGN(2) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 4> {
    dtkArrayAlignedChar Q_DECL_ALIGN(4) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 8> {
    dtkArrayAlignedChar Q_DECL_ALIGN(8) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 16> {
    dtkArrayAlignedChar Q_DECL_ALIGN(16) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 32> {
    dtkArrayAlignedChar Q_DECL_ALIGN(32) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 64> {
    dtkArrayAlignedChar Q_DECL_ALIGN(64) data[sizeof(T) * PreallocSize];
};

template <typename T, int PreallocSize> struct dtkArrayAlignedPrealloc<T, PreallocSize, 128> {
    dtkArrayAlignedChar Q_DECL_ALIGN(128) data[sizeof(T) * PreallocSize];
};

#else

template <typename T, int PreallocSize, size_t AlignT> union dtkArrayAlignedPrealloc {
    char data[sizeof(T) * PreallocSize];
    qint64 q_for_alignment_1;
    double q_for_alignment_2;
};

#endif

// ///////////////////////////////////////////////////////////////////
// dtkArrayData base class
// ///////////////////////////////////////////////////////////////////

template <typename T, int PreallocSize> class dtkArrayDataOld
{
public:
#if defined(Q_ALIGNOF)
    dtkArrayAlignedPrealloc<T, PreallocSize, Q_ALIGNOF(T)> m_prealloc;
#else
    dtkArrayAlignedPrealloc<T, PreallocSize, sizeof(T)> m_prealloc;
#endif

    inline T *prealloc(void) {
        return reinterpret_cast<T *>(m_prealloc.data);
    }

    inline bool isPrealloc(const T *start) const {
        return start == reinterpret_cast<const T *>(m_prealloc.data);
    }
};

template <typename T> class dtkArrayDataOld<T, 0>
{
public:
    inline T *prealloc(void) {
        return 0;
    }

    inline bool isPrealloc(const T *start) const {
        Q_UNUSED(start);
        return false;
    }
};

// ///////////////////////////////////////////////////////////////////
// dtkArray interface
// ///////////////////////////////////////////////////////////////////

template <typename T, int PreallocSize = 8> class dtkArray : private dtkArrayDataOld<T, PreallocSize>
{
public:
    enum RawDataType {
        ReadOnly = 0x001,
        Writable = 0x002
    };

public:
    dtkArray(void);
    explicit dtkArray(int arraySize);
    dtkArray(int size, const T& value);
    dtkArray(const T *values, int size);
    dtkArray(const dtkArray<T, PreallocSize>& other);

public:
    ~dtkArray(void);

public:
    typedef T *iterator;
    typedef const T *const_iterator;

    dtkArray<T, PreallocSize>& operator = (const dtkArray<T, PreallocSize>& other);

    int size(void) const;
    int count(void) const;
    int capacity(void) const;

    bool isEmpty(void) const;

    bool isDetached(void) const;

    void detach(void);

    void clear(void);

    const T&         at(int index) const;
    const T& operator[](int index) const;
    T& operator[](int index);

    T value(int index) const;
    T value(int index, const T& defaultValue) const;

    T *extend(int size);

    void append(const T& value);
    void append(const T& value1, const T& value2);
    void append(const T& value1, const T& value2, const T& value3);
    void append(const T& value1, const T& value2, const T& value3, const T& value4);
    void append(const T *values, int count);
    void append(const dtkArray<T, PreallocSize>& other);

    void prepend(const T& value);

    void insert(int index, const T& value);
    void insert(int index, int count, const T& value);
    iterator insert(iterator before, int count, const T& value);
    iterator insert(iterator before, const T& value);

    void replace(int index, const T& newValue);
    void replace(int index, const T *values, int countToAdd);

    void remove(int index);
    void remove(int index, int countToRemove);
    void removeFirst() {
        remove(0);
    }
    void removeLast() {
        remove(size() - 1);
    }

    iterator erase(iterator begin, iterator end);
    iterator erase(iterator pos);

    int indexOf(const T& value, int from = 0) const;
    int lastIndexOf(const T& value, int from = -1) const;
    bool contains(const T& value) const;
    int count(const T& value) const;

    void resize(int size);
    void reserve(int size);
    void squeeze(void);

    dtkArray<T, PreallocSize>& fill(const T& fillValue, int fillCount = -1);

    void reverse(void);
    dtkArray<T, PreallocSize> reversed(void) const;

    dtkArray<T, PreallocSize> mid(int index, int length = -1) const;
    dtkArray<T, PreallocSize> left(int length) const;
    dtkArray<T, PreallocSize> right(int length) const;

    void setRawData(const T *raw_data, int size, RawDataType data_type = ReadOnly);

    T *rawData(void);
    const T *rawData(void) const;
    const T *constRawData(void) const;

    static dtkArray<T, PreallocSize> fromRawData(const T *data, int size);
    static dtkArray<T, PreallocSize> fromWritableRawData(T *data, int size);

    bool operator == (const dtkArray<T, PreallocSize>& other) const;
    bool operator != (const dtkArray<T, PreallocSize>& other) const;

    dtkArray<T, PreallocSize>& operator += (const T& value);
    dtkArray<T, PreallocSize>& operator += (const dtkArray<T, PreallocSize>& other);
    dtkArray<T, PreallocSize>& operator << (const T& value);
    dtkArray<T, PreallocSize>& operator << (const dtkArray<T, PreallocSize>& other);

    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef ptrdiff_t difference_type;
    typedef int size_type;

    inline iterator begin(void) {
        return rawData();
    }
    inline const_iterator begin(void) const {
        return constRawData();
    }
    inline const_iterator constBegin(void) const {
        return constRawData();
    }
    inline iterator end(void) {
        return rawData() + size();
    }
    inline const_iterator end(void) const {
        return constRawData() + size();
    }
    inline const_iterator constEnd(void) const {
        return constRawData() + size();
    }

    inline T& first(void) {
        Q_ASSERT(!isEmpty());
        return *begin();
    }
    inline const T& first(void) const {
        Q_ASSERT(!isEmpty());
        return *begin();
    }
    inline T& last(void) {
        Q_ASSERT(!isEmpty());
        return *(end() - 1);
    }
    inline const T& last(void) const {
        Q_ASSERT(!isEmpty());
        return *(end() - 1);
    }
    inline bool startsWith(const T& t) const {
        return !isEmpty() && first() == t;
    }
    inline bool endsWith(const T& t) const {
        return !isEmpty() && last() == t;
    }

    inline void push_back(const T& newValue) {
        append(newValue);
    }
    inline void push_front(const T& newValue) {
        prepend(newValue);
    }
    inline void pop_back(void) {
        Q_ASSERT(!isEmpty());
        removeLast();
    }
    inline void pop_front(void) {
        Q_ASSERT(!isEmpty());
        removeFirst();
    }
    inline bool empty(void) const {
        return isEmpty();
    }
    inline reference front(void) {
        return first();
    }
    inline const_reference front(void) const {
        return first();
    }
    inline reference back(void) {
        return last();
    }
    inline const_reference back(void) const {
        return last();
    }

private:
    struct Data {
        QBasicAtomicInt ref;
        int capacity;
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

    inline void initPrealloc(void) {
        m_end = m_start = dtkArrayDataOld<T, PreallocSize>::prealloc();
        m_limit = m_start + PreallocSize;
    }

    dtkArray(const T *data, int size, bool isWritable);

    void free(T *data, int count);
    void release(void);
    void copyReplace(T *dst, const T *src, int count);
    Data *copyData(const T *src, int size, int capacity);
    void reallocate(int capacity);
    void detach_helper(void);
    void assign(const dtkArray<T, PreallocSize>& other);
    void grow(int needed);
    void setSize(int newSize);
};

int DTKCORESUPPORT_EXPORT dtkArrayAllocMore(int alloc, int extra, int sizeOfT);

// ///////////////////////////////////////////////////////////////////
// dtkArray implementation
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

