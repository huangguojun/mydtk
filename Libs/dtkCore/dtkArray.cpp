// Version: $Id: 8f1044d07b37993ca9012da829d754e19e6862d4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

/*!
  \class dtkArray
  \inmodule dtkCore
  \brief The dtkArray class is a template class that provides a dynamic array.

  dtkArray is similar to QVector except that its size limit is
  respectively 2^31 for 32 bits machines and 2^63 for 64 bits ones.

  dtkArray instances have a preallocated data area for quickly building
  small arrays on the stack without malloc overhead. Once the array
  grows beyond the preallocated size, it is copied to the heap. The
  size of the preallocated area, which defaults to 8, can be specified
  with the second template parameter:

  \code
  dtkArray<double, 32> array;
  \endcode

  Moreover, dtkArray can be set from raw data without any
  copy. According to the writable permission, the dtkArray will detach
  or not if any modification is performed.

*/

/*! \fn dtkArray::dtkArray(void)

    Constructs an empty array.

    \sa resize()
*/

/*! \fn dtkArray::dtkArray(qlonglong size)

    Constructs an array with an initial size of \a size elements.

    The elements are initialized with a \l{default-constructed
    value}.

    \sa resize()
*/

/*! \fn dtkArray::dtkArray(qlonglong size, const T& value)

    Constructs an array with an initial size of \a size elements.
    Each element is initialized with \a value.

    \sa resize(), fill()
*/

/*! \fn dtkArray::dtkArray(const dtkArray& other)

    Constructs a copy of \a other.

    When preallocated memory is used, namely when array size is lower
    than PreallocSize, the copy is always done.

    However, as soon as array size is greater than PreallocSize,
    dtkArray behaves the same way than the QVector. This operation
    then takes \l{constant time}, because in this case dtkArray is
    \l{implicitly shared}. This makes returning a dtkArray from a
    function very fast. If a shared instance is modified, it will be
    copied (copy-on-write), and that takes \l{linear time}.

    \sa operator=()
*/

/*! \fn dtkArray::dtkArray(const T *values, qlonglong size);

  Constructs an array of \a size elements, initialized from \a values.
*/

#ifdef Q_COMPILER_RVALUE_REFS
/*!
    \fn dtkArray::dtkArray(dtkArray&& other)

    Move-constructs a dtkArray instance, making it point at the same
    object that \a other was pointing to.
*/
#endif

/*! \fn dtkArray::dtkArray(const dtkArray<T, PreallocSizeOther>& other)

    Constructs a copy of \a other which has not the same preallocated
    memory.

    When array size is lower than PreallocSize, the copy is always
    done.

    However, as soon as array size is greater than PreallocSize,
    dtkArray behaves the same way than the QVector. This operation
    then takes \l{constant time}, because in this case dtkArray is
    \l{implicitly shared}. This makes returning a dtkArray from a
    function very fast. If a shared instance is modified, it will be
    copied (copy-on-write), and that takes \l{linear time}.

    \sa operator=()
*/

/*! \fn dtkArray::~dtkArray(void)

    Destroys the array.
*/

/*! \fn dtkArray& dtkArray::operator = (const dtkArray& other)

    Assigns \a other to this array and returns a reference to this
    array.
*/

#ifdef Q_COMPILER_RVALUE_REFS
/*!
    \fn dtkArray& dtkArray::operator = (dtkArray&& other)

    Move-assigns \a other to this dtkArray instance.
*/
#endif

/*! \fn bool dtkArray::operator == (const dtkArray& other) const

    Returns \c true if \a other is equal to this array; otherwise
    returns \c false.

    Two arrays are considered equal if they contain the same values
    in the same order.

    This function requires the value type to have an implementation
    of \c operator==().

    \sa operator!=()
*/

/*! \fn bool dtkArray::operator == (const dtkArray<T, PreallocSizeOther>& other)
   const

    Returns \c true if \a other is equal to this array; otherwise
    returns \c false.

    Two arrays are considered equal if they contain the same values
    in the same order.

    This function requires the value type to have an implementation
    of \c operator==().

    \sa operator!=()
*/

/*! \fn bool dtkArray::operator != (const dtkArray& other) const

    Returns \c true if \a other is not equal to this array; otherwise
    returns \c false.

    Two arrays are considered equal if they contain the same values
    in the same order.

    This function requires the value type to have an implementation
    of \c operator==().

    \sa operator==()
*/

/*! \fn bool dtkArray::operator != (const dtkArray<T, PreallocSizeOther>& other)
   const

    Returns \c true if \a other is not equal to this array; otherwise
    returns \c false.

    Two arrays are considered equal if they contain the same values
    in the same order.

    This function requires the value type to have an implementation
    of \c operator==().

    \sa operator==()
*/

/*! \fn void dtkArray::swap(dtkArray& other)

    Swaps array \a other with this array. This operation is very fast
    when both array are sharable.
*/

/*! \fn bool dtkArray::isDetached(void) const

    \internal
*/

/*! \fn void dtkArray::detach(void)

    \internal
*/

/*! \fn bool dtkArray::isSharedWith(const dtkArray& other) const

    \internal
*/

/*! \fn void dtkArray::setSharable(bool sharable)

    \internal
*/

/*! \fn bool dtkArray::empty(void) const

    Returns \c true if the array has size 0; otherwise returns \c false.

    \sa size(), resize()
*/

/*! \fn bool dtkArray::isEmpty(void) const

    Returns \c true if the array has size 0; otherwise returns \c false.

    \sa size(), resize()
*/

/*! \fn bool dtkArray::usePreallocation(void) const

    \internal
*/

/*! \fn int dtkArray::size(void) const

    Returns the number of items in the array.

    \sa isEmpty(), resize()
*/

/*! \fn int dtkArray::count(void) const

    Returns the number of items in the array.

    \sa isEmpty(), resize()
*/

/*! \fn int dtkArray::length(void) const

    Returns the number of items in the array.

    \sa isEmpty(), resize()
*/

/*! \fn int dtkArray::capacity(void) const

    Returns the maximum number of items that can be stored in the
    array without forcing a reallocation.

    The sole purpose of this function is to provide a means of fine
    tuning dtkArray's memory usage. In general, you will rarely ever
    need to call this function. If you want to know how many items are
    in the array, call size().

    \sa reserve(), squeeze()
*/

/*! \fn int dtkArray::preallocatedCapacity(void) const

    \internal
*/

/*! \fn void dtkArray::clear(void)

    Removes all the elements from the array and releases the heap
    memory used by the array.
*/

/*! \fn void dtkArray::resize(qlonglong size)

    Sets the size of the array to \a size. If \a size is greater than the
    current size, elements are added to the end; the new elements are
    initialized with a \l{default-constructed value}. If \a size is less
    than the current size, elements are removed from the end.

    \sa size()
*/

/*! \fn void dtkArray::reserve(qlonglong size)

    Attempts to allocate memory for at least \a size elements. If you
    know in advance how large the array will be, you can call this
    function, and if you call resize() often you are likely to get
    better performance. If \a size is an underestimate, the worst
    that will happen is that the dtkArray will be a bit slower.

    The sole purpose of this function is to provide a means of fine
    tuning dtkArray's memory usage. In general, you will rarely ever
    need to call this function. If you want to change the size of the
    array, call resize().

    \sa squeeze(), capacity()
*/

/*! \fn void dtkArray::squeeze(void)

    Releases any memory not required to store the items.

    The sole purpose of this function is to provide a means of fine
    tuning dtkArray's memory usage. In general, you will rarely ever
    need to call this function.

    \sa reserve(), capacity()
*/

/*!
    \fn void dtkArray::append(const T& value)

    Inserts \a value at the end of the array.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 7

    This is the same as calling resize(size() + 1) and assigning \a
    value to the new last element in the array.

    This operation is relatively fast, because dtkArray typically
    allocates more memory than necessary, so it can grow without
    reallocating the entire array each time.

    Moreover, as long as the size remains lower than PreallocSize, no
    extra memory is used.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const T& v1, const T& v2)

    Inserts \a v1 and \a v2 at the end of the array.

    It is faster than appending twice.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const T& v1, const T& v2, const T& v3)

    Inserts \a v1, \a v2 and \a v3 at the end of the array.

    It is faster than appending three times.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const T& v1, const T& v2, const T& v3, const T&
   v4)

    Inserts \a v1, \a v2, \a v3 and \a v4 at the end of the array.

    It is faster than appending four times.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const T *values, qlonglong size)

    Appends the \a size elements of \a values to this array.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const dtkArray& other)

    Appends all the items of \a other to this array.

    \sa operator<<(), prepend(), insert()
*/

/*!
    \fn void dtkArray::append(const dtkArray<T, PreallocSizeOther>& other)

    Appends all the items of \a other to this array.

    \sa operator<<(), prepend(), insert()
*/

/*! \fn void dtkArray::prepend(const T& value)

    Inserts \a value at the beginning of the array.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 8

    This is the same as array.insert(0, \a value).

    For large arrays, this operation can be slow (\l{linear time}),
    because it requires moving all the items in the array by one
    position further in memory. If you want a container class that
    provides a fast prepend() function, use QList or QLinkedList
    instead.

    \sa append(), insert()
*/

/*! \fn void dtkArray::insert(qlonglong index, const T& value)

    Inserts \a value at index position \a index in the array. If \a
    index is 0, the value is prepended to the array. If \a index is
    size(), the value is appended to the array.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 9

    For large arrays, this operation can be slow (\l{linear time}),
    because it requires moving all the items at indexes \a index and
    above by one position further in memory. If you want a container
    class that provides a fast insert() function, use QLinkedList
    instead.

    \sa append(), prepend(), remove()
*/

/*! \fn void dtkArray::insert(qlonglong index, qlonglong length, const T& value)

    \overload

    Inserts \a length times \a value at index position \a index in the
    array.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 10
*/

/*! \fn dtkArray::iterator dtkArray::insert(iterator before, const T& value)

    \overload

    Inserts \a value in front of the item pointed to by the iterator
    \a before. Returns an iterator pointing at the inserted item.
*/

/*! \fn dtkArray::iterator dtkArray::insert(iterator before, qlonglong length,
   const T &value)

    Inserts \a length times \a value in front of the item pointed to
    by the iterator \a before. Returns an iterator pointing at the
    first of the inserted items.
*/

/*! \fn void dtkArray::setAt(qlonglong index, const T& value)

    Replaces the item at position \a index with \a value.

    \a index must be a valid index position in the array (i.e., 0 <= \a
    index < size()).

    \sa operator[](), remove()
*/

/*! \fn void dtkArray::setAt(qlonglong index, const T *values, qlonglong length)

    \overload

    Replaces the \a length elements of this array with the contents of
    \a values, starting at \a index.

    If (\a index + \a length) is larger than the current size of this
    array, the array will be extended to that size.

    \sa operator[](), remove()
*/

/*! \fn void dtkArray::replace(qlonglong index, const T& value)

    \overload

    Replaces the item at position \a index with \a value.

    \sa setAt()
*/

/*! \fn void dtkArray::replace(qlonglong index, const T *values, qlonglong
   length)

    \overload

    Replaces the \a length elements of this array with the contents of
    \a values, starting at \a index.

    \sa setAt()
*/

/*! \fn dtkArray& dtkArray::fill(const T &value, qlonglong size = -1)

    Assigns \a value to all items in the array. If \a size is
    different from -1 (the default), the array is resized to size \a
    size beforehand.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 11

    \sa resize()
*/

/*! \fn void dtkArray::setRawData(const T *raw_data, qlonglong size)

    Replaces the current data of the array by the \a raw_data and
    resize the array to \a size.

    The array is then unsharable which means that any modification of
    the array leads to a deep copy.

    The memory at \a raw_data must remain valid until the returned
    array is destroyed or modified.

    \sa setWritableRawData(), fromRawData()

*/

/*! \fn void dtkArray::setWritableRawData(T *raw_data, qlonglong size)

    Replaces the current data of the array by the \a raw_data and
    resize the array to \a size.

    This function takes a reference to \a raw_data, but does not copy
    the elements until the array is reallocated to a larger size. The
    memory at \a raw_data must remain valid until the returned array
    is destroyed or reallocated.

    The elements of \a raw_data will be modified in-place. This
    differs from setRawData() which will make a copy of the elements
    of \a raw_data when the array is modified.

    If the returned array is resized to less than \a size, then a copy
    will not be made, and append() can be used to append new items up
    to \a size. Further calls to append() after \a size will force the
    array to be reallocated.

    If the returned array is resized to more than \a size, then a copy
    of the data will be made and further modifications will not affect
    the elements at \a raw_data.

    \sa setRawData(), fromWritableRawData()
*/

/*! \fn dtkArray dtkArray::fromRawData(const T *raw_data, qlonglong size)

    Returns an array consisting of the \a size elements from \a raw_data.

    The array is unsharable which means that any modification of
    the array leads to a deep copy.

    The memory at \a raw_data must remain valid until the returned
    array is destroyed or modified.

    \sa setRawData(), fromWritableRawData()
*/

/*! \fn dtkArray dtkArray::fromWritableRawData(T *raw_data, qlonglong size)

    Returns an array consisting of the \a size elements from \a raw_data.

    This function takes a reference to \a raw_data, but does not copy
    the elements until the array is reallocated to a larger size. The
    memory at \a raw_data must remain valid until the returned array
    is destroyed or reallocated.

    The elements of \a raw_data will be modified in-place. This
    differs from setRawData() which will make a copy of the elements
    of \a raw_data when the array is modified.

    If the returned array is resized to less than \a size, then a copy
    will not be made, and append() can be used to append new items up
    to \a size. Further calls to append() after \a size will force the
    array to be reallocated.

    If the returned array is resized to more than \a size, then a copy
    of the data will be made and further modifications will not affect
    the elements at \a raw_data.

    \sa setWritableRawData(), fromRawData()
*/

/*! \fn dtkArray::iterator dtkArray::erase(iterator pos)

    Removes the item pointed to by the iterator \a pos from the
    array, and returns an iterator to the next item in the array
    (which may be end()).

    \sa insert(), remove()
*/

/*! \fn dtkArray::iterator dtkArray::erase(iterator begin, iterator end)

    \overload

    Removes all the items from \a begin up to (but not including) \a
    end. Returns an iterator to the same item that \a end referred to
    before the call.
*/

/*! \fn void dtkArray::remove(qlonglong index)

    \overload

    Removes the element at position \a index.

    \sa insert(), replace(), fill()
*/

/*! \fn void dtkArray::remove(qlonglong index, qlonglong length)

    \overload

    Removes \a length elements from the middle of the array, starting at
    position \a index.

    \sa insert(), replace(), fill()
*/

/*! \fn void dtkArray::removeFirst(void)

    Removes the first item in the array. Calling this function is
    equivalent to calling remove(0). The array must not be empty. If
    the array can be empty, call isEmpty() before calling this
    function.

    \sa remove(), takeFirst(), isEmpty()
*/

/*! \fn void dtkArray::removeLast(void)

    Removes the last item in the array. Calling this function is
    equivalent to calling remove(size() - 1). The array must not be
    empty. If the array can be empty, call isEmpty() before calling
    this function.

    \sa remove(), takeLast(), removeFirst(), isEmpty()
*/

/*! \fn void dtkArray::removeAt(qlonglong index)

    Removes the element at position \a index.
    Equivalent to
    \code
    remove(index);
    \endcode

    \sa remove(), QList::removeAt()
*/

/*! \fn T dtkArray::takeFirst(void)

    Removes the first item in the array and returns it. This function
    assumes the array is not empty. To avoid failure, call isEmpty()
    before calling this function.

    \sa takeLast(), removeFirst()
*/

/*! \fn T dtkArray::takeLast(void)

    Removes the last item in the list and returns it. This function
    assumes the array is not empty. To avoid failure, call isEmpty()
    before calling this function.

    If you don't use the return value, removeLast() is more
    efficient.

    \sa takeFirst(), removeLast()
*/

/*! \fn T dtkArray::takeAt(qlonglong index)

    Removes the element at position \a index and returns it.

    Equivalent to
    \code
    T t = at(index);
    remove(index);
    return t;
    \endcode

    \sa takeFirst(), takeLast(), QList::takeAt()
*/

/*! \fn void dtkArray::reverse(void)

    Reverses the order of this array in place.

    \sa reversed()
*/

/*! \fn dtkArray dtkArray::reversed(void) const

    Returns a copy of this array with elements in the reverse order.

    \sa reverse()
*/

/*! \fn T *dtkArray::data(void)

    Returns a pointer to the data stored in the array. The pointer
    can be used to access and modify the items in the array.

    Example:
    \snippet code/src_corelib_tools_qvector.cpp 6

    The pointer remains valid as long as the array isn't
    reallocated.

    This function is mostly useful to pass an array to a function
    that accepts a plain C++ array.

    \sa constData(), operator[]()
*/

/*! \fn const T *dtkArray::data(void) const

    \overload
*/

/*! \typedef dtkArray::iterator

    The dtkArray::iterator typedef provides an STL-style non-const
    iterator for dtkArray.

    \warning Iterators on implicitly shared containers do not work
    exactly like STL-iterators. You should avoid copying a container
    while iterators are active on that container. For more information,
    read \l{Implicit sharing iterator problem}.

    \sa dtkArray::begin(), dtkArray::end(), dtkArray::const_iterator
*/

/*! \typedef dtkArray::const_iterator

    The dtkArray::const_iterator typedef provides an STL-style const
    iterator for dtkArray.

    \warning Iterators on implicitly shared containers do not work
    exactly like STL-iterators. You should avoid copying a container
    while iterators are active on that container. For more information,
    read \l{Implicit sharing iterator problem}.

    \sa dtkArray::constBegin(), dtkArray::constEnd(), dtkArray::cbegin(),
   dtkArray::cend(), dtkArray::iterator
*/

/*! \typedef dtkArray::Iterator

    Qt-style synonym for dtkArray::iterator.
*/

/*! \typedef dtkArray::ConstIterator

    Qt-style synonym for dtkArray::const_iterator.
*/

/*! \typedef dtkArray::const_pointer

    Typedef for const T *. Provided for STL compatibility.
*/

/*! \typedef dtkArray::const_reference

    Typedef for T &. Provided for STL compatibility.
*/

/*! \typedef dtkArray::difference_type

    Typedef for qptrdiff. Provided for STL compatibility.
*/

/*! \typedef dtkArray::pointer

    Typedef for T *. Provided for STL compatibility.
*/

/*! \typedef dtkArray::reference

    Typedef for T &. Provided for STL compatibility.
*/

/*! \typedef dtkArray::size_type

    Typedef for qlonglong. Provided for STL compatibility.
*/

/*! \typedef dtkArray::value_type

    Typedef for T. Provided for STL compatibility.
*/

//
// dtkArray.cpp ends here
