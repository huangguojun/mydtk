/* dtkMathArray.tpp ---
 *
 * Author: Thibaud Kloczko
 * Created: Tue Jul 23 14:57:58 2013 (+0200)
 */

/* Credits:
 *
 *  See EOF.
 */

#pragma once

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::free(T *dataptr, qlonglong count_)
{
    while (count_-- > 0) {
        dataptr->~T();
        ++dataptr;
    }
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::release(void)
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
template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::copyReplace(T *dst, const T *src, qlonglong acount)
{
    if (!QTypeInfo<T>::isStatic) {
        ::memmove(dst, src, acount * sizeof(T));
    } else {
        while (acount-- > 0)
            *dst++ = *src++;
    }
}

// Make a copy of m_data, while remaining exception-safe.
template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE typename dtkMathArray<T, PreallocSize>::Data *dtkMathArray<T, PreallocSize>::copyData(const T *src, qlonglong asize, qlonglong acapacity)
{
    Data *newdata = reinterpret_cast<Data *>(malloc(sizeof(Data) + sizeof(T) * (acapacity - 1)));
    Q_CHECK_PTR(newdata);
    newdata->ref.store(1);
    newdata->capacity = acapacity;
    T *dst = newdata->array;
    qlonglong copied = 0;
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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::reallocate(qlonglong acapacity)
{
    qlonglong size_ = m_end - m_start;

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

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::detach_helper(void)
{
    // If the reference count is 1, then the array may have been
    // copied and then the copy released.  So just reset the limit.
    if (m_data && m_data->ref.load() == 1) {
        m_limit = m_start + m_data->capacity;
        return;
    }

    // Allocate a new block on the heap and copy the data across.
    qlonglong size_ = m_end - m_start;
    qlonglong newcapacity = dtkMathArrayAllocMore(size_, 0, sizeof(T));
    m_data = copyData(m_start, size_, newcapacity);

    // Update the start/end/append pointers for faster updates.
    m_start = m_data->array;
    m_end = m_start + size_;
    m_limit = m_start + newcapacity;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::assign(const dtkMathArray<T, PreallocSize>& other)
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

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::grow(qlonglong needed)
{
    qlonglong size_ = m_end - m_start;
    qlonglong newcapacity = dtkMathArrayAllocMore(size_, needed, sizeof(T));

    if (!m_data || m_data->ref.load() != 1) {
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

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::setSize(qlonglong newSize)
{
    if (newSize <= PreallocSize) {
        initPrealloc();
        m_data = 0;
    } else {
        qlonglong newcapacity = dtkMathArrayAllocMore(newSize, 0, sizeof(T));
        Data *newdata = reinterpret_cast<Data *>(malloc(sizeof(Data) + sizeof(T) * (newcapacity - 1)));
        Q_CHECK_PTR(newdata);
        m_data = newdata;
        m_data->ref.store(1);
        m_data->capacity = newcapacity;
        m_start = m_data->array;
        m_end = m_start;
        m_limit = m_start + newcapacity;
    }
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(void)
{
    initPrealloc();
    m_data = 0;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(qlonglong arraySize)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T();
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(qlonglong arraySize, const T& fillValue)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T(fillValue);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(const T *values, qlonglong arraySize)
{
    setSize(arraySize);

    while (arraySize-- > 0)
        new (m_end++) T(*values++);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(const dtkMathArray<T, PreallocSize>& other)
{
    assign(other);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::dtkMathArray(const T *dataptr, qlonglong arraySize, bool isWritable)
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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>::~dtkMathArray(void)
{
    release();
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::operator = (const dtkMathArray<T, PreallocSize>& other)
{
    if (this == &other)
        return *this;

    if (other.m_data && m_data == other.m_data)
        return *this;

    release();
    assign(other);
    return *this;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::size(void) const
{
    return m_end - m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::count(void) const
{
    return m_end - m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::capacity(void) const
{
    if (m_data)
        return m_data->capacity;
    else if (this->isPrealloc(m_start))
        return PreallocSize;
    else
        return m_end - m_start; // raw data, m_limit == m_start
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE bool dtkMathArray<T, PreallocSize>::isEmpty(void) const
{
    return m_start == m_end;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE bool dtkMathArray<T, PreallocSize>::isDetached(void) const
{
    // If m_limit is the same as m_start, then the array
    // is either shared or contains raw data.
    return m_limit != m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::detach(void)
{
    if (m_limit == m_start)
        detach_helper();
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::clear(void)
{
    release();
    initPrealloc();
    m_data = 0;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE const T& dtkMathArray<T, PreallocSize>::operator[](qlonglong index) const
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkMathArray<T>::at", "index out of range");
    return m_start[index];
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE const T& dtkMathArray<T, PreallocSize>::at(qlonglong index) const
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkMathArray<T>::operator[]", "index out of range");
    return m_start[index];
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE T& dtkMathArray<T, PreallocSize>::operator[](qlonglong index)
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkMathArray<T>::operator[]", "index out of range");
    return rawData()[index];
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE T dtkMathArray<T, PreallocSize>::value(qlonglong index) const
{
    if (index >= 0 && index < size())
        return m_start[index];
    else
        return T();
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE T dtkMathArray<T, PreallocSize>::value(qlonglong index, const T& defaultValue) const
{
    if (index >= 0 && index < size())
        return m_start[index];
    else
        return defaultValue;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE T *dtkMathArray<T, PreallocSize>::extend(qlonglong newSize)
{
    Q_ASSERT(newSize > 0);

    if ((m_end + newSize) >= m_limit)
        grow(newSize);

    T *oldend = m_end;
    m_end += newSize;  // Note: new elements are not initialized.
    return oldend;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const T& newValue)
{
    if (m_end >= m_limit)
        grow(1);

    new (m_end) T(newValue);
    ++m_end;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const T& value1, const T& value2)
{
    if ((m_end + 1) >= m_limit)
        grow(2);

    new (m_end) T(value1);
    ++m_end; // Increment one at a time in case an exception is thrown.
    new (m_end) T(value2);
    ++m_end;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const T& value1, const T& value2, const T& value3)
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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const T& value1, const T& value2, const T& value3, const T& value4)
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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const T *values, qlonglong countToAdd)
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

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::append(const dtkMathArray<T, PreallocSize>& other)
{
    if (isEmpty()) {
        *this = other;
    } else {
        if (&other == this || (m_data && other.m_data == m_data))
            grow(size());   // Appending to ourselves: make some room.

        append(other.constRawData(), other.size());
    }
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::prepend(const T& newValue)
{
    insert(begin(), 1, newValue);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::insert(qlonglong index, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index <= size(),
               "dtkMathArray<T>::insert", "index out of range");
    insert(begin() + index, 1, newValue);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::insert(qlonglong index, qlonglong countToAdd, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index <= size(),
               "dtkMathArray<T>::insert", "index out of range");
    insert(begin() + index, countToAdd, newValue);
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE typename dtkMathArray<T, PreallocSize>::iterator dtkMathArray<T, PreallocSize>::insert(iterator before, qlonglong countToAdd, const T& newValue)
{
    // Check the parameters.
    qlonglong size_ = this->size();
    qlonglong offset = int(before - m_start);
    Q_ASSERT_X(offset >= 0 && offset <= size_,
               "dtkMathArray<T>::insert", "iterator offset is out of range");
    Q_ASSERT(countToAdd >= 0);

    if (countToAdd <= 0)
        return m_start + offset;

    // Reserve extra space and then copy-on-write.
    reserve(size_ + countToAdd);
    detach();

    // Move items up to make room, and replace at the insert point.
    if (QTypeInfo<T>::isStatic) {
        qlonglong newcount = countToAdd;

        while (newcount > 0) {
            new (m_end++) T();
            --newcount;
        }

        qlonglong posn = size_;

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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE typename dtkMathArray<T, PreallocSize>::iterator dtkMathArray<T, PreallocSize>::insert(iterator before, const T& newValue)
{
    return insert(before, 1, newValue);
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::replace(qlonglong index, const T& newValue)
{
    Q_ASSERT_X(index >= 0 && index < size(),
               "dtkMathArray<T>::replace", "index out of range");
    rawData()[index] = newValue;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::replace(qlonglong index, const T *values, qlonglong countToAdd)
{
    if (index < 0 || countToAdd <= 0)
        return;

    qlonglong replaceSize = index + countToAdd;

    if (replaceSize > size())
        resize(replaceSize);

    copyReplace(rawData() + index, values, countToAdd);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::remove(qlonglong index)
{
    remove(index, 1);
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::remove(qlonglong index, qlonglong countToRemove)
{
    // Truncate the range to be removed.
    qlonglong currentSize = size();

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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE typename dtkMathArray<T, PreallocSize>::iterator dtkMathArray<T, PreallocSize>::erase(iterator from, iterator to)
{
    qlonglong index = from - m_start;
    remove(index, to - from);
    return m_start + index;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE typename dtkMathArray<T, PreallocSize>::iterator dtkMathArray<T, PreallocSize>::erase(iterator pos)
{
    qlonglong index = pos - m_start;
    remove(index, 1);
    return m_start + index;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::indexOf(const T& needle, qlonglong from) const
{
    if (from < 0)
        from = qMax(from + size(), static_cast<qlonglong>(0));

    const T *ptr = m_start + from;

    while (ptr < m_end) {
        if (*ptr == needle)
            return ptr - m_start;

        ++ptr;
    }

    return -1;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::lastIndexOf(const T& needle, qlonglong from) const
{
    qlonglong size_ = count();

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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE bool dtkMathArray<T, PreallocSize>::contains(const T& needle) const
{
    const T *ptr = m_start;

    while (ptr < m_end) {
        if (*ptr == needle)
            return true;

        ++ptr;
    }

    return false;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE qlonglong dtkMathArray<T, PreallocSize>::count(const T& needle) const
{
    const T *ptr = m_start;
    qlonglong n = 0;

    while (ptr < m_end) {
        if (*ptr == needle)
            ++n;

        ++ptr;
    }

    return n;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::resize(qlonglong newSize)
{
    if (newSize < 0)
        return;

    qlonglong currentSize = count();

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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::reserve(qlonglong newCapacity)
{
    qlonglong cap = capacity();

    if (newCapacity > cap)
        grow(newCapacity - this->size());
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::fill(const T& fillValue, qlonglong fillCount)
{
    if (fillCount >= 0)
        resize(fillCount);

    T *ptr = m_start;

    while (ptr < m_end)
        *ptr++ = fillValue;

    return *this;
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::squeeze(void)
{
    qlonglong currentSize = count();

    if (currentSize <= 0) {
        clear();
    } else if (currentSize < capacity() && m_data) {
        reallocate(currentSize);
    }
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::reverse(void)
{
    if (count() > 0) {
        T *src = m_start;
        T *dst = m_end - 1;

        while (src < dst)
            qSwap(*(dst--), *(src++));
    }
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::reversed(void) const
{
    dtkMathArray<T, PreallocSize> result;
    qlonglong count_ = size();

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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::mid(qlonglong index, qlonglong length) const
{
    qlonglong count_ = size();
    Q_ASSERT(index >= 0 && index <= count_);

    if (length < 0 || (index + length) > count_)
        length = count_ - index;

    if (index == 0 && length == count_)
        return *this;

    dtkMathArray<T, PreallocSize> result;
    result.append(constRawData() + index, length);
    return result;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::left(qlonglong length) const
{
    return mid(0, length);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::right(qlonglong length) const
{
    qlonglong size_ = count();

    if (length < 0 || length >= size_)
        length = size_;

    return mid(size_ - length, length);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE void dtkMathArray<T, PreallocSize>::setRawData(const T *raw_data, qlonglong size, RawDataType data_type)
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

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE T *dtkMathArray<T, PreallocSize>::rawData(void)
{
    detach();
    return m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE const T *dtkMathArray<T, PreallocSize>::rawData(void) const
{
    return m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE const T *dtkMathArray<T, PreallocSize>::constRawData(void) const
{
    return m_start;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::fromRawData(const T *data, qlonglong size)
{
    Q_ASSERT(size >= 0);
    return dtkMathArray<T, PreallocSize>(data, size, false);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize> dtkMathArray<T, PreallocSize>::fromWritableRawData(T *data, qlonglong size)
{
    Q_ASSERT(size >= 0);
    return dtkMathArray<T, PreallocSize>(data, size, true);
}

template <typename T, qlonglong PreallocSize>
Q_OUTOFLINE_TEMPLATE bool dtkMathArray<T, PreallocSize>::operator == (const dtkMathArray<T, PreallocSize>& other) const
{
    if (this == &other)
        return true;

    const T *thisData = constRawData();
    const T *otherData = other.constRawData();

    if (thisData == otherData)
        return true;

    qlonglong thisCount = count();
    qlonglong otherCount = other.count();

    if (thisCount == 0 && otherCount == 0)
        return true;

    if (thisCount != otherCount)
        return false;

    for (qlonglong index = 0; index < thisCount; ++index, ++thisData, ++otherData)
        if (*thisData != *otherData)
            return false;

    return true;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE bool dtkMathArray<T, PreallocSize>::operator != (const dtkMathArray<T, PreallocSize>& other) const
{
    return !(*this == other);
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::operator+=(const T& newValue)
{
    append(newValue);
    return *this;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::operator+=(const dtkMathArray<T, PreallocSize>& other)
{
    append(other);
    return *this;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::operator<<(const T& newValue)
{
    append(newValue);
    return *this;
}

template <typename T, qlonglong PreallocSize>
Q_INLINE_TEMPLATE dtkMathArray<T, PreallocSize>& dtkMathArray<T, PreallocSize>::operator<<(const dtkMathArray<T, PreallocSize>& other)
{
    append(other);
    return *this;
}

#ifndef QT_NO_DATASTREAM

template <typename T, qlonglong PreallocSize>
QDataStream& operator << (QDataStream& stream, const dtkMathArray<T, PreallocSize>& array)
{
    qlonglong size = array.size();
    stream << quint32(size);

    for (qlonglong index = 0; index < size; ++index)
        stream << array.at(index);

    return stream;
}

template <typename T, qlonglong PreallocSize>
QDataStream& operator >> (QDataStream& stream, dtkMathArray<T, PreallocSize>& array)
{
    array.clear();
    quint32 size;
    stream >> size;
    array.reserve(size);

    for (qlonglong index = 0; index < int(size); ++index) {
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

template <typename T, qlonglong PreallocSize>
QDebug operator << (QDebug dbg, const dtkMathArray<T, PreallocSize>& array)
{
    dbg.nospace() << "dtkMathArray(\n";
    qlonglong size = array.size();

    for (qlonglong index = 0; index < size; ++index) {
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

