/* dtkArray.tpp ---
 *
 * Author: Thibaud Kloczko
 * Created: Tue Jul  2 09:41:02 2013 (+0200)
 * Version:
 * Last-Updated: Tue Jul  2 10:55:20 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 10
 */

/* Credits:
 *
 *  See EOF.
 */

#pragma once

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::free(T *dataptr, int count_)
{
    while (count_-- > 0) {
        dataptr->~T();
        ++dataptr;
    }
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::release(void)
{
    if (m_data) {
        if (!m_data->ref.deref()) {
            if (QTypeInfo<T>::isComplex)
                free(m_start, m_end - m_start);

            ::free(m_data);
        }
    } else if (this->isPrealloc(m_start)) {
        if (QTypeInfo<T>::isComplex)
            free(m_start, m_end - m_start);
    }
}

// Copy values to initialized memory, replacing previous values.
template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::copyReplace(T *dst, const T *src, int acount)
{
    if (!QTypeInfo<T>::isStatic) {
        ::memmove(dst, src, acount * sizeof(T));
    } else {
        while (acount-- > 0)
            *dst++ = *src++;
    }
}

// Make a copy of m_data, while remaining exception-safe.
template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE typename dtkArray<T, PreallocSize>::Data *dtkArray<T, PreallocSize>::copyData(const T *src, int asize, int acapacity)
{
    Data *newdata = reinterpret_cast<Data *>(malloc(sizeof(Data) + sizeof(T) * (acapacity - 1)));
    Q_CHECK_PTR(newdata);
    newdata->ref = 1;
    newdata->capacity = acapacity;
    T *dst = newdata->array;
    int copied = 0;
    QT_TRY {
        while (copied < asize)
        {
            new (dst) T(*src++);
            ++dst;
            ++copied;
        }
    } QT_CATCH(...) {
        while (copied-- > 0)
            (--dst)->~T();

        ::free(newdata);
        QT_RETHROW;
    }
    return newdata;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::reallocate(int acapacity)
{
    int size_ = m_end - m_start;

    if (!QTypeInfo<T>::isStatic) {
        Data *newdata = reinterpret_cast<Data *>
                        (realloc(m_data, sizeof(Data) + sizeof(T) * (acapacity - 1)));
        Q_CHECK_PTR(newdata);
        newdata->capacity = acapacity;
        m_data = newdata;
    } else {
        Data *newdata = copyData(m_data->array, size_, acapacity);
        free(m_data->array, size_);
        ::free(m_data);
        m_data = newdata;
    }

    m_start = m_data->array;
    m_end = m_start + size_;
    m_limit = m_start + acapacity;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::detach_helper(void)
{
    // If the reference count is 1, then the array may have been
    // copied and then the copy released.  So just reset the limit.
    if (m_data && m_data->ref == 1) {
        m_limit = m_start + m_data->capacity;
        return;
    }

    // Allocate a new block on the heap and copy the data across.
    int size_ = m_end - m_start;
    int newcapacity = dtkArrayAllocMore(size_, 0, sizeof(T));
    m_data = copyData(m_start, size_, newcapacity);

    // Update the start/end/append pointers for faster updates.
    m_start = m_data->array;
    m_end = m_start + size_;
    m_limit = m_start + newcapacity;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::assign(const dtkArray<T, PreallocSize>& other)
{
    if (other.m_data) {
        m_start = other.m_start;
        m_end = other.m_end;
        m_data = other.m_data;
        m_data->ref.ref();

        // We set the append limit of both objects to m_start, which forces
        // the next append() or rawData() in either object to copy-on-write.
        other.m_limit = m_limit = m_start;
    } else if (other.isPrealloc(other.m_start)) {
        // Make a deep copy of preallocated data.
        initPrealloc();
        m_data = 0;
        append(other.constRawData(), other.size());
    } else {
        // Shallow copy of raw data.
        m_start = other.m_start;
        m_end = other.m_end;
        m_limit = other.m_limit;
        m_data = 0;
    }
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::grow(int needed)
{
    int size_ = m_end - m_start;
    int newcapacity = dtkArrayAllocMore(size_, needed, sizeof(T));

    if (!m_data || m_data->ref != 1) {
        // Copy preallocated, raw, or shared data and expand the capacity.
        Data *newdata = copyData(m_start, size_, newcapacity);

        if (this->isPrealloc(m_start))
            free(m_start, size_);

        if (m_data)
            m_data->ref.deref();

        m_data = newdata;
        m_start = newdata->array;
        m_end = m_start + size_;
        m_limit = m_start + newcapacity;
    } else if ((size_ + needed) > m_data->capacity) {
        // Reallocate to create more capacity.
        reallocate(newcapacity);
    } else {
        // We have enough capacity - just fix the append limit.
        // This can happen when an array is copied and then the
        // copy is removed.
        m_limit = m_start + m_data->capacity;
    }
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::setSize(int newSize)
{
    if (newSize <= PreallocSize) {
        initPrealloc();
        m_data = 0;
    } else {
        int newcapacity = dtkArrayAllocMore(newSize, 0, sizeof(T));
        Data *newdata = reinterpret_cast<Data *>
                        (malloc(sizeof(Data) + sizeof(T) * (newcapacity - 1)));
        Q_CHECK_PTR(newdata);
        m_data = newdata;
        m_data->ref = 1;
        m_data->capacity = newcapacity;
        m_start = m_data->array;
        m_end = m_start;
        m_limit = m_start + newcapacity;
    }
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(void)
{
    initPrealloc();
    m_data = 0;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(int arraySize)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T();
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(int arraySize, const T& fillValue)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T(fillValue);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(const T *values, int arraySize)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T(*values++);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(const dtkArray<T, PreallocSize>& other)
{
    assign(other);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::dtkArray(const T *dataptr, int arraySize, bool isWritable)
{
    // Constructing a raw data array.
    m_start = const_cast<T *>(dataptr);
    m_end = m_start + arraySize;

    if (isWritable)
        m_limit = m_end;
    else
        m_limit = m_start;

    m_data = 0;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>::~dtkArray(void)
{
    release();
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator = (const dtkArray<T, PreallocSize>& other)
{
    if (this == &other)
        return *this;

    if (other.m_data && m_data == other.m_data)
        return *this;

    release();
    assign(other);
    return *this;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE int dtkArray<T, PreallocSize>::size(void) const
{
    return m_end - m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE int dtkArray<T, PreallocSize>::count(void) const
{
    return m_end - m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE int dtkArray<T, PreallocSize>::capacity(void) const
{
    if (m_data)
        return m_data->capacity;
    else if (this->isPrealloc(m_start))
        return PreallocSize;
    else
        return m_end - m_start; // raw data, m_limit == m_start
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE bool dtkArray<T, PreallocSize>::isEmpty(void) const
{
    return m_start == m_end;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE bool dtkArray<T, PreallocSize>::isDetached(void) const
{
    // If m_limit is the same as m_start, then the array
    // is either shared or contains raw data.
    return m_limit != m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::detach(void)
{
    if (m_limit == m_start)
        detach_helper();
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::clear(void)
{
    release();
    initPrealloc();
    m_data = 0;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE const T& dtkArray<T, PreallocSize>::operator[](int index) const
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkArray<T>::at", "index out of range");
    return m_start[index];
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE const T& dtkArray<T, PreallocSize>::at(int index) const
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkArray<T>::operator[]", "index out of range");
    return m_start[index];
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE T& dtkArray<T, PreallocSize>::operator[](int index)
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkArray<T>::operator[]", "index out of range");
    return rawData()[index];
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE T dtkArray<T, PreallocSize>::value(int index) const
{
    if (index >= 0 && index < size())
        return m_start[index];
    else
        return T();
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE T dtkArray<T, PreallocSize>::value(int index, const T& defaultValue) const
{
    if (index >= 0 && index < size())
        return m_start[index];
    else
        return defaultValue;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE T *dtkArray<T, PreallocSize>::extend(int newSize)
{
    Q_ASSERT(newSize > 0);

    if ((m_end + newSize) >= m_limit)
        grow(newSize);

    T *oldend = m_end;
    m_end += newSize;  // Note: new elements are not initialized.
    return oldend;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const T& newValue)
{
    if (m_end >= m_limit)
        grow(1);

    new (m_end) T(newValue);
    ++m_end;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const T& value1, const T& value2)
{
    if ((m_end + 1) >= m_limit)
        grow(2);

    new (m_end) T(value1);
    ++m_end; // Increment one at a time in case an exception is thrown.
    new (m_end) T(value2);
    ++m_end;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const T& value1, const T& value2, const T& value3)
{
    if ((m_end + 2) >= m_limit)
        grow(3);

    new (m_end) T(value1);
    ++m_end;
    new (m_end) T(value2);
    ++m_end;
    new (m_end) T(value3);
    ++m_end;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const T& value1, const T& value2, const T& value3, const T& value4)
{
    if ((m_end + 3) >= m_limit)
        grow(4);

    new (m_end) T(value1);
    ++m_end;
    new (m_end) T(value2);
    ++m_end;
    new (m_end) T(value3);
    ++m_end;
    new (m_end) T(value4);
    ++m_end;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const T *values, int countToAdd)
{
    if (countToAdd <= 0)
        return;

    if (!m_start || (m_end + countToAdd) > m_limit)
        grow(countToAdd);

    while (countToAdd-- > 0) {
        new (m_end) T(*values++);
        ++m_end;
    }
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::append(const dtkArray<T, PreallocSize>& other)
{
    if (isEmpty()) {
        *this = other;
    } else {
        if (&other == this || (m_data && other.m_data == m_data))
            grow(size());   // Appending to ourselves: make some room.

        append(other.constRawData(), other.size());
    }
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::prepend(const T& newValue)
{
    insert(begin(), 1, newValue);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::insert(int index, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index <= size(),
               "dtkArray<T>::insert", "index out of range");
    insert(begin() + index, 1, newValue);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::insert(int index, int countToAdd, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index <= size(),
               "dtkArray<T>::insert", "index out of range");
    insert(begin() + index, countToAdd, newValue);
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::insert(iterator before, int countToAdd, const T& newValue)
{
    // Check the parameters.
    int size_ = this->size();
    int offset = int(before - m_start);
    Q_ASSERT_X(offset >= 0 && offset <= size_,
               "dtkArray<T>::insert", "iterator offset is out of range");
    Q_ASSERT(countToAdd >= 0);

    if (countToAdd <= 0)
        return m_start + offset;

    // Reserve extra space and then copy-on-write.
    reserve(size_ + countToAdd);
    detach();

    // Move items up to make room, and replace at the insert point.
    if (QTypeInfo<T>::isStatic) {
        int newcount = countToAdd;

        while (newcount > 0) {
            new (m_end++) T();
            --newcount;
        }

        int posn = size_;

        while (posn > offset) {
            --posn;
            m_start[posn + countToAdd] = m_start[posn];
        }

        while (countToAdd > 0) {
            --countToAdd;
            m_start[offset + countToAdd] = newValue;
        }
    } else {
        ::memmove(m_start + offset + countToAdd, m_start + offset,
                  (size_ - offset) * sizeof(T));
        m_end += countToAdd;

        while (countToAdd > 0) {
            --countToAdd;
            new (m_start + offset + countToAdd) T(newValue);
        }
    }

    // Return the new iterator at the insert position.
    return m_start + offset;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::insert(iterator before, const T& newValue)
{
    return insert(before, 1, newValue);
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::replace(int index, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkArray<T>::replace", "index out of range");
    rawData()[index] = newValue;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::replace(int index, const T *values, int countToAdd)
{
    if (index < 0 || countToAdd <= 0)
        return;

    int replaceSize = index + countToAdd;

    if (replaceSize > size())
        resize(replaceSize);

    copyReplace(rawData() + index, values, countToAdd);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::remove(int index)
{
    remove(index, 1);
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::remove(int index, int countToRemove)
{
    // Truncate the range to be removed.
    int currentSize = size();

    if (index < 0) {
        countToRemove += index;
        index = 0;
    }

    if (countToRemove > 0 && (index + countToRemove) > currentSize)
        countToRemove = currentSize - index;

    if (countToRemove <= 0)
        return;

    // Perform the removal.
    if (index == 0 && countToRemove >= currentSize) {
        clear();
        return;
    }

    T *start = rawData();
    copyReplace(start + index, start + index + countToRemove,
                (currentSize - (index + countToRemove)));
    resize(currentSize - countToRemove);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::erase(iterator from, iterator to)
{
    int index = from - m_start;
    remove(index, to - from);
    return m_start + index;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE typename dtkArray<T, PreallocSize>::iterator dtkArray<T, PreallocSize>::erase(iterator pos)
{
    int index = pos - m_start;
    remove(index, 1);
    return m_start + index;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE int dtkArray<T, PreallocSize>::indexOf(const T& needle, int from) const
{
    if (from < 0)
        from = qMax(from + size(), 0);

    const T *ptr = m_start + from;

    while (ptr < m_end) {
        if (*ptr == needle)
            return ptr - m_start;

        ++ptr;
    }

    return -1;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE int dtkArray<T, PreallocSize>::lastIndexOf(const T& needle, int from) const
{
    int size_ = count();

    if (from < 0)
        from += size_;
    else if (from >= size_)
        from = size_ - 1;

    if (from >= 0) {
        const T *ptr = m_start + from;

        while (ptr >= m_start) {
            if (*ptr == needle)
                return ptr - m_start;

            --ptr;
        }
    }

    return -1;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE bool dtkArray<T, PreallocSize>::contains(const T& needle) const
{
    const T *ptr = m_start;

    while (ptr < m_end) {
        if (*ptr == needle)
            return true;

        ++ptr;
    }

    return false;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE int dtkArray<T, PreallocSize>::count(const T& needle) const
{
    const T *ptr = m_start;
    int n = 0;

    while (ptr < m_end) {
        if (*ptr == needle)
            ++n;

        ++ptr;
    }

    return n;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::resize(int newSize)
{
    if (newSize < 0)
        return;

    int currentSize = count();

    if (newSize < currentSize) {
        T *start = rawData();  // Force copy on write if necessary.

        if (QTypeInfo<T>::isComplex)
            free(start + newSize, currentSize - newSize);

        m_end = start + newSize;
    } else if (newSize > currentSize) {
        grow(newSize - currentSize);

        while (currentSize++ < newSize) {
            new (m_end) T();
            ++m_end;
        }
    }
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::reserve(int newCapacity)
{
    int cap = capacity();

    if (newCapacity > cap)
        grow(newCapacity - this->size());
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::fill(const T& fillValue, int fillCount)
{
    if (fillCount >= 0)
        resize(fillCount);

    T *ptr = m_start;

    while (ptr < m_end)
        *ptr++ = fillValue;

    return *this;
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::squeeze(void)
{
    int currentSize = count();

    if (currentSize <= 0) {
        clear();
    } else if (currentSize < capacity() && m_data) {
        reallocate(currentSize);
    }
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkArray<T, PreallocSize>::reverse(void)
{
    if (count() > 0) {
        T *src = m_start;
        T *dst = m_end - 1;

        while (src < dst)
            qSwap(*(dst--), *(src++));
    }
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::reversed(void) const
{
    dtkArray<T, PreallocSize> result;
    int count_ = size();

    if (count_ > 0) {
        result.extend(count_);
        const T *src = m_start;
        T *dst = result.m_end - 1;

        if (!QTypeInfo<T>::isComplex) {
            while (src != m_end)
                *(dst--) = *(src++);
        } else {
            while (src != m_end)
                new (dst--) T(*src++);
        }
    }

    return result;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::mid(int index, int length) const
{
    int count_ = size();
    Q_ASSERT(index >= 0 && index <= count_);

    if (length < 0 || (index + length) > count_)
        length = count_ - index;

    if (index == 0 && length == count_)
        return *this;

    dtkArray<T, PreallocSize> result;
    result.append(constRawData() + index, length);
    return result;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::left(int length) const
{
    return mid(0, length);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::right(int length) const
{
    int size_ = count();

    if (length < 0 || length >= size_)
        length = size_;

    return mid(size_ - length, length);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE void dtkArray<T, PreallocSize>::setRawData(const T *raw_data, int size, RawDataType data_type)
{
    Q_ASSERT(size >= 0);

    if (m_start != raw_data)
        release();

    // Store the raw data array
    m_start = const_cast<T *>(raw_data);
    m_end = m_start + size;

    if (data_type == Writable)
        m_limit = m_end;
    else
        m_limit = m_start;

    m_data = 0;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE T *dtkArray<T, PreallocSize>::rawData(void)
{
    detach();
    return m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE const T *dtkArray<T, PreallocSize>::rawData(void) const
{
    return m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE const T *dtkArray<T, PreallocSize>::constRawData(void) const
{
    return m_start;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::fromRawData(const T *data, int size)
{
    Q_ASSERT(size >= 0);
    return dtkArray<T, PreallocSize>(data, size, false);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize> dtkArray<T, PreallocSize>::fromWritableRawData(T *data, int size)
{
    Q_ASSERT(size >= 0);
    return dtkArray<T, PreallocSize>(data, size, true);
}

template <typename T, int PreallocSize>
Q_OUTOFLINE_TEMPLATE bool dtkArray<T, PreallocSize>::operator == (const dtkArray<T, PreallocSize>& other) const
{
    if (this == &other)
        return true;

    const T *thisData = constRawData();
    const T *otherData = other.constRawData();

    if (thisData == otherData)
        return true;

    int thisCount = count();
    int otherCount = other.count();

    if (thisCount == 0 && otherCount == 0)
        return true;

    if (thisCount != otherCount)
        return false;

    for (int index = 0; index < thisCount; ++index, ++thisData, ++otherData)
        if (*thisData != *otherData)
            return false;

    return true;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE bool dtkArray<T, PreallocSize>::operator != (const dtkArray<T, PreallocSize>& other) const
{
    return !(*this == other);
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator+=(const T& newValue)
{
    append(newValue);
    return *this;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator+=(const dtkArray<T, PreallocSize>& other)
{
    append(other);
    return *this;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator<<(const T& newValue)
{
    append(newValue);
    return *this;
}

template <typename T, int PreallocSize>
Q_INLINE_TEMPLATE dtkArray<T, PreallocSize>& dtkArray<T, PreallocSize>::operator<<(const dtkArray<T, PreallocSize>& other)
{
    append(other);
    return *this;
}

#ifndef QT_NO_DATASTREAM

template <typename T, int PreallocSize>
QDataStream& operator << (QDataStream& stream, const dtkArray<T, PreallocSize>& array)
{
    int size = array.size();
    stream << quint32(size);

    for (int index = 0; index < size; ++index)
        stream << array.at(index);

    return stream;
}

template <typename T, int PreallocSize>
QDataStream& operator >> (QDataStream& stream, dtkArray<T, PreallocSize>& array)
{
    array.clear();
    quint32 size;
    stream >> size;
    array.reserve(size);

    for (int index = 0; index < int(size); ++index) {
        T t;
        stream >> t;
        array.append(t);

        if (stream.atEnd())
            break;
    }

    return stream;
}

#endif

#ifndef QT_NO_DEBUG_STREAM

template <typename T, int PreallocSize>
QDebug operator << (QDebug dbg, const dtkArray<T, PreallocSize>& array)
{
    dbg.nospace() << "dtkArray(\n";
    int size = array.size();

    for (int index = 0; index < size; ++index) {
        dbg << "   " << index << ":  " << array.at(index) << "\n";
    }

    dbg << ")\n";
    return dbg.space();
}

#endif


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

