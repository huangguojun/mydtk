/* dtkSparseMatrix.h ---
 *
 * Author: Thibaud Kloczko
 * Created: lun. janv. 13 12:12:22 2014 (+0100)
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkCore>

template < typename T > class dtkSparseMatrix;

// /////////////////////////////////////////////////////////////////
// dtkSparseMatrixHandler
// /////////////////////////////////////////////////////////////////

template < typename T > class dtkSparseMatrixHandler
{
public:
    dtkSparseMatrixHandler(void) {
        ;
    }
    virtual ~dtkSparseMatrixHandler(void) {
        ;
    }

public:
    virtual qlonglong     rowCount(void) const = 0;
    virtual qlonglong     colCount(void) const = 0;
    virtual qlonglong nonZeroCount(void) const = 0;

    virtual qlonglong nonZeroCount(qlonglong index) const = 0;

public:
    virtual void clear(void) = 0;
    virtual void reset(void) = 0;

    virtual void resize(qlonglong row_count, qlonglong col_count) = 0;
    virtual void reserve(qlonglong non_zero_count) = 0;

    virtual void append(qlonglong i, qlonglong j, const T& value) = 0;
    virtual void insert(qlonglong i, qlonglong j, const T& value) = 0;

public:
    virtual T at(qlonglong i, qlonglong j) const = 0;
    virtual void setAt(qlonglong i, qlonglong j, const T& value) = 0;

public:
    friend class dtkSparseMatrix<T>::item;

    virtual T value(qlonglong pos) const = 0;
    virtual void setValue(qlonglong pos, const T& value) = 0;

    virtual QPair<qlonglong, qlonglong> coordinates(qlonglong pos) const = 0;

    virtual qlonglong pos(qlonglong row_id, qlonglong col_id) const = 0;
};

// /////////////////////////////////////////////////////////////////
// dtkSparseMatrix
// /////////////////////////////////////////////////////////////////

template < typename T > class dtkSparseMatrix : public QObject
{
public:
    class item;
    class iterator;
    class const_iterator;

public:
    explicit inline dtkSparseMatrix(void) : m_handler(0) {
        ;
    }
    explicit inline dtkSparseMatrix(dtkSparseMatrixHandler<T> *handler) : m_handler(handler) {
        ;
    }
    /* explicit inline dtkSparseMatrix(qlonglong row_count, qlonglong col_count); */
    /* explicit inline dtkSparseMatrix(qlonglong row_count, qlonglong col_count, qlonglong non_zero_count); */

    /* public: */
    /*     inline dtkSparseMatrix(const dtkSparseMatrix& other); */

public:
    virtual ~dtkSparseMatrix(void) {
        if (m_handler) delete m_handler;

        m_handler = 0;
    }

public:
    class item
    {
        friend class dtkSparseMatrix<T>::iterator;
        friend class dtkSparseMatrix<T>::const_iterator;
        dtkSparseMatrixHandler<T> *h;
        qlonglong id;

    public:
        inline  item(void) : h(0), id(0) {
            ;
        }
        inline  item(dtkSparseMatrixHandler<T> *handler, const qlonglong& index) : h(handler), id(index) {
            ;
        }
        inline  item(dtkSparseMatrixHandler<T> *handler, const qlonglong& row_id, const qlonglong& col_id) : h(handler), id(handler->pos(row_id, col_id)) {
            ;
        }
        inline  item(const item& o) : h(o.h), id(o.id) {
            ;
        }
        inline ~item(void) {
            ;
        }

    public:
        inline item& operator = (const item& o) {
            h = o.h;
            id = o.id;
            return *this;
        }

    public:
        inline T value(void) const {
            return h->value(id);
        }

        inline QPair<qlonglong, qlonglong> coordinates(void) const {
            return h->coordinates(id);
        }

        inline qlonglong pos(void) const {
            return id;
        }

    public:
        inline item& operator =  (const T& value) {
            h->setValue(id, value);
            return *this;
        }
        inline item& operator += (const T& value) {
            h->setValue(id, h->value(id) + value);
            return *this;
        }
        inline item& operator -= (const T& value) {
            h->setValue(id, h->value(id) - value);
            return *this;
        }
        inline item& operator *= (const T& value) {
            h->setValue(id, h->value(id) * value);
            return *this;
        }
        inline item& operator /= (const T& value) {
            h->setValue(id, h->value(id) / value);
            return *this;
        }

    public:
        inline bool operator == (const T& value) const {
            return (h->value(id) == value);
        }
        inline bool operator != (const T& value) const {
            return (h->value(id) != value);
        }
        inline bool operator <  (const T& value) const {
            return (h->value(id) <  value);
        }
        inline bool operator >  (const T& value) const {
            return (h->value(id) >  value);
        }
        inline bool operator <= (const T& value) const {
            return (h->value(id) <= value);
        }
        inline bool operator >= (const T& value) const {
            return (h->value(id) >= value);
        }

    public:
        inline bool  operator == (const item& o) const {
            return id == o.id;
        }
        inline bool  operator != (const item& o) const {
            return id != o.id;
        }
        inline bool  operator <  (const item& o) const {
            return id  < o.id;
        }
        inline bool  operator >  (const item& o) const {
            return id  > o.id;
        }
        inline bool  operator <= (const item& o) const {
            return id <= o.id;
        }
        inline bool  operator >= (const item& o) const {
            return id >= o.id;
        }
    };
    friend class item;

public:
    class iterator
    {
        item it;

    public:
        inline  iterator(void) : it(item()) {
            ;
        }
        inline  iterator(dtkSparseMatrixHandler<T> *handler, const qlonglong& index = 0) : it(handler, index) {
            ;
        }
        inline  iterator(const iterator& o) : it(o.it) {
            ;
        }
        inline ~iterator(void) {
            ;
        }

    public:
        inline iterator& operator = (const iterator& o) {
            it = o.it;
            return *this;
        }

    public:
        inline item operator *  (void)        const {
            return it;
        }
        inline item operator [] (qlonglong j) const {
            return item(it.h, it.id + j);
        }

    public:
        inline T value(void) const {
            return it.h->value(it.id);
        }

        inline QPair<qlonglong, qlonglong> coordinates(void) const {
            return it.h->coordinates(it.id);
        }

        inline qlonglong pos(void) const {
            return it.id;
        }

    public:
        inline bool operator == (const iterator& o) const {
            return it.id == o.it.id;
        }
        inline bool operator != (const iterator& o) const {
            return it.id != o.it.id;
        }
        inline bool operator <  (const iterator& o) const {
            return it.id  < o.it.id;
        }
        inline bool operator >  (const iterator& o) const {
            return it.id  > o.it.id;
        }
        inline bool operator <= (const iterator& o) const {
            return it.id <= o.it.id;
        }
        inline bool operator >= (const iterator& o) const {
            return it.id >= o.it.id;
        }

    public:
        inline iterator& operator ++ (void) {
            ++(it.id);
            return *this;
        }
        inline iterator  operator ++ (int)  {
            ++(it.id);
            return iterator(it.h, it.id - 1);
        }
        inline iterator& operator -- (void) {
            --(it.id);
            return *this;
        }
        inline iterator  operator -- (int)  {
            --(it.id);
            return iterator(it.h, it.id + 1);
        }
        inline iterator& operator += (qlonglong j) {
            it.id += j;
            return *this;
        }
        inline iterator& operator -= (qlonglong j) {
            it.id -= j;
            return *this;
        }
        inline iterator  operator +  (qlonglong j) const {
            return iterator(it.h, it.id + j);
        }
        inline iterator  operator -  (qlonglong j) const {
            return iterator(it.h, it.id - j);
        }

    public:
        inline qlonglong operator -  (const iterator& o) const {
            return it.id - o.it.id;
        }

    };
    friend class iterator;

public:
    class const_iterator
    {
        item it;

    public:
        inline  const_iterator(void) : it(item()) {
            ;
        }
        inline  const_iterator(const dtkSparseMatrixHandler<T> *handler, const qlonglong& index) : it(const_cast<dtkSparseMatrixHandler<T> *>(handler), index) {
            ;
        }
        inline  const_iterator(const const_iterator& o) : it(o.it) {
            ;
        }
        explicit inline  const_iterator(const iterator& o) : it(*o) {
            ;
        }
        inline ~const_iterator(void) {
            ;
        }

    public:
        inline const_iterator& operator = (const const_iterator& o) {
            it = o.it;
            return *this;
        }

    public:
        inline const item operator *  (void)        const {
            return it;
        }
        inline const item operator [] (qlonglong j) const {
            return item(it.h, it.id + j);
        }

    public:
        inline T value(void) const {
            return it.h->value(it.id);
        }

        inline QPair<qlonglong, qlonglong> coordinates(void) const {
            return it.h->coordinates(it.id);
        }

        inline qlonglong pos(void) const {
            return it.id;
        }

    public:
        inline bool operator == (const const_iterator& o) const {
            return it.id == o.it.id;
        }
        inline bool operator != (const const_iterator& o) const {
            return it.id != o.it.id;
        }
        inline bool operator <  (const const_iterator& o) const {
            return it.id  < o.it.id;
        }
        inline bool operator >  (const const_iterator& o) const {
            return it.id  > o.it.id;
        }
        inline bool operator <= (const const_iterator& o) const {
            return it.id <= o.it.id;
        }
        inline bool operator >= (const const_iterator& o) const {
            return it.id >= o.it.id;
        }

    public:
        inline const_iterator& operator ++ (void) {
            ++(it.id);
            return *this;
        }
        inline const_iterator  operator ++ (int)  {
            ++(it.id);
            return const_iterator(it.h, it.id - 1);
        }
        inline const_iterator& operator -- (void) {
            --(it.id);
            return *this;
        }
        inline const_iterator  operator -- (int)  {
            --(it.id);
            return const_iterator(it.h, it.id + 1);
        }
        inline const_iterator& operator += (qlonglong j) {
            (it.id) += j;
            return *this;
        }
        inline const_iterator& operator -= (qlonglong j) {
            (it.id) -= j;
            return *this;
        }
        inline const_iterator  operator +  (qlonglong j) const {
            return const_iterator(it.h, it.id + j);
        }
        inline const_iterator  operator -  (qlonglong j) const {
            return const_iterator(it.h, it.id - j);
        }

    public:
        inline qlonglong operator -  (const const_iterator& o) const {
            return it.id - o.it.id;
        }
    };
    friend class const_iterator;

public:
    inline qlonglong     rowCount(void) const {
        return m_handler->rowCount();
    }
    inline qlonglong     colCount(void) const {
        return m_handler->colCount();
    }
    inline qlonglong nonZeroCount(void) const {
        return m_handler->nonZeroCount();
    }

    inline qlonglong nonZeroCount(qlonglong index) const {
        return m_handler->nonZeroCount(index);
    }

public:
    inline void clear(void) {
        m_handler->clear();
    }
    inline void reset(void) {
        m_handler->reset();
    }

    inline void resize(qlonglong row_count, qlonglong col_count) {
        m_handler->resize(row_count, col_count);
    }
    inline void reserve(qlonglong non_zero_count) {
        m_handler->reserve(non_zero_count);
    }

    inline void append(qlonglong i, qlonglong j, const T& value) {
        m_handler->append(i, j, value);
    }
    inline void insert(qlonglong i, qlonglong j, const T& value) {
        m_handler->insert(i, j, value);
    }

public:
    inline T at(qlonglong i, qlonglong j) const {
        return m_handler->at(i, j);
    }
    inline void  setAt(qlonglong i, qlonglong j, const T& value) {
        m_handler->setAt(i, j, value);
    }

public:
    inline       item operator()(qlonglong i, qlonglong j)       {
        return item(m_handler, i, j);
    }
    inline const item operator()(qlonglong i, qlonglong j) const {
        return item(m_handler, i, j);
    }

public:
    inline       iterator      begin(void)       {
        return       iterator(m_handler, 0);
    }
    inline const_iterator      begin(void) const {
        return const_iterator(m_handler, 0);
    }
    inline const_iterator     cbegin(void) const {
        return const_iterator(m_handler, 0);
    }
    inline const_iterator constBegin(void) const {
        return const_iterator(m_handler, 0);
    }
    inline       iterator        end(void)       {
        return       iterator(m_handler, this->nonZeroCount());
    }
    inline const_iterator        end(void) const {
        return const_iterator(m_handler, this->nonZeroCount());
    }
    inline const_iterator       cend(void) const {
        return const_iterator(m_handler, this->nonZeroCount());
    }
    inline const_iterator   constEnd(void) const {
        return const_iterator(m_handler, this->nonZeroCount());
    }

private:
    dtkSparseMatrixHandler<T> *m_handler;
};

// /////////////////////////////////////////////////////////////////
// dtkSparseMatrixHandlerCsr
// /////////////////////////////////////////////////////////////////

template < typename T > class dtkSparseMatrixHandlerCsr : public dtkSparseMatrixHandler<T>
{
public:
    /*              dtkSparseMatrixHandlerCsr(void); */
    ~dtkSparseMatrixHandlerCsr(void) {
        this->clear();
    }

public:
    inline qlonglong     rowCount(void) const {
        return m_row_count;
    }
    inline qlonglong     colCount(void) const {
        return m_col_count;
    }
    inline qlonglong nonZeroCount(void) const {
        return m_array.count();
    }

    inline qlonglong nonZeroCount(qlonglong i) const {
        return m_non_zero_count.at(i);
    }

public:
    void clear(void);
    void reset(void);

    void resize(qlonglong row_count, qlonglong col_count);
    void reserve(qlonglong non_zero_count);

    void append(qlonglong i, qlonglong j, const T& value);
    void insert(qlonglong i, qlonglong j, const T& value);

public:
    inline T at(qlonglong i, qlonglong j) const {
        return m_array.at(this->pos(i, j));
    }
    inline void setAt(qlonglong i, qlonglong j, const T& value) {
        m_array[this->pos(i, j)] = value;
    }

public:
    inline T value (qlonglong pos) const {
        return m_array.at(pos);
    }
    inline void setValue(qlonglong pos, const T& value) {
        m_array[pos] = value;
    }

    inline QPair<qlonglong, qlonglong> coordinates(qlonglong pos) const {
        return qMakePair(this->searchRow(pos, 0, m_row_count - 1), m_col_indices.at(pos));
    }

    qlonglong pos(qlonglong i, qlonglong j) const;

protected:
    qlonglong m_row_count;
    qlonglong m_col_count;

    QVector<T> m_array;
    QVector<qlonglong> m_row_start;
    QVector<qlonglong> m_non_zero_count;
    QVector<qlonglong> m_col_indices;

protected:
    qlonglong searchRow(qlonglong pos, qlonglong min, qlonglong max) const;
};

// /////////////////////////////////////////////////////////////////

template < typename T >
qlonglong dtkSparseMatrixHandlerCsr<T>::searchRow(qlonglong pos, qlonglong min, qlonglong max) const
{
    if (pos >= m_row_start.at(max)) return max;

    if (pos < m_row_start.at(min + 1)) return min;

    qlonglong mid = (max - min) / 2 + min;
    qlonglong pos_mid = m_row_start.at(mid);

    if (pos_mid > pos) return this->searchRow(pos, min + 1, mid - 1);

    if (pos < m_row_start.at(mid + 1)) return mid;

    return this->searchRow(pos, mid + 1, max - 1);
};

template < typename T >
qlonglong dtkSparseMatrixHandlerCsr<T>::pos(qlonglong i, qlonglong j) const
{
    qlonglong pos = m_row_start.at(i);
    qlonglong end = pos + m_non_zero_count.at(i);

    while (j != m_col_indices.at(pos)) {
        ++pos;

        if (pos == end)
            return -1;
    }

    return pos;
};

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::clear(void)
{
    m_row_count = 0;
    m_col_count = 0;
    m_array.clear();
    m_row_start.clear();
    m_non_zero_count.clear();
    m_col_indices.clear();
};

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::reset(void)
{
    qlonglong zero = 0;
    m_array.fill(T(0));
    m_row_start.fill(zero);
    m_non_zero_count.fill(zero);
    m_col_indices.fill(zero);
};

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::resize(qlonglong row_count, qlonglong col_count)
{
    m_row_count = row_count;
    m_col_count = col_count;
    m_row_start.resize(row_count);
    m_non_zero_count.resize(row_count);
    this->reset();
};

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::reserve(qlonglong non_zero_count)
{
    m_array.reserve(non_zero_count);
    m_col_indices.reserve(non_zero_count);
};

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::append(qlonglong i, qlonglong j, const T& value)
{
    m_array.append(value);
    m_col_indices.append(j);
    m_non_zero_count[i] += 1;

    for (qlonglong k = i + 1; k < m_row_count; ++k) {
        m_row_start[k] += 1;
    }
}

template < typename T >
void dtkSparseMatrixHandlerCsr<T>::insert(qlonglong i, qlonglong j, const T& value)
{
    qlonglong pos = m_row_start.at(i);
    qlonglong end = pos + m_non_zero_count.at(i);

    while (pos < end) {
        if (j < m_col_indices.at(pos))
            break;

        ++pos;
    }

    m_array.insert(pos, value);
    m_col_indices.insert(pos, j);

    m_non_zero_count[i] += 1;

    for (qlonglong k = i + 1; k < m_row_count; ++k) {
        m_row_start[k] += 1;
    }
}

// /////////////////////////////////////////////////////////////////
// dtkSparseMatrixHandlerHash
// /////////////////////////////////////////////////////////////////

template < typename T > class dtkSparseMatrixHandlerHash : public dtkSparseMatrixHandler<T>
{
public:
    /*              dtkSparseMatrixHandlerCsr(void); */
    ~dtkSparseMatrixHandlerHash(void) {
        this->clear();
    }

    typedef QPair<qlonglong, qlonglong> MatrixEntry;

public:
    inline qlonglong     rowCount(void) const {
        return m_row_count;
    }
    inline qlonglong     colCount(void) const {
        return m_col_count;
    }
    inline qlonglong nonZeroCount(void) const {
        return m_map.count();
    }

    inline qlonglong nonZeroCount(qlonglong i) const {
        return m_non_zero_count.at(i);
    }

public:
    void clear(void);
    void reset(void);

    void resize(qlonglong row_count, qlonglong col_count);
    void reserve(qlonglong non_zero_count);

    void append(qlonglong i, qlonglong j, const T& value);
    void insert(qlonglong i, qlonglong j, const T& value);

public:
    inline T at(qlonglong i, qlonglong j) const {
        return m_map.value(qMakePair(i, j), T(0));
    }
    inline void setAt(qlonglong i, qlonglong j, const T& value) {
        m_map.insert(qMakePair(i, j), value);
    }

public:
    T value (qlonglong pos) const;
    void setValue(qlonglong pos, const T& value);

    QPair<qlonglong, qlonglong> coordinates(qlonglong pos) const;

    qlonglong pos(qlonglong i, qlonglong j) const;

protected:
    qlonglong m_row_count;
    qlonglong m_col_count;

    QMap<MatrixEntry, T> m_map;
    QVector<qlonglong> m_non_zero_count;
};

// /////////////////////////////////////////////////////////////////

template < typename T >
void dtkSparseMatrixHandlerHash<T>::clear(void)
{
    m_map.clear();
    m_row_count = 0;
    m_col_count = 0;
    m_non_zero_count.clear();
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::reset(void)
{
    typename QMap<MatrixEntry, T>::iterator it = m_map.begin();

    while (it != m_map.end()) {
        *it = T(0);
        ++it;
    }

    m_non_zero_count.fill(0);
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::resize(qlonglong row_count, qlonglong col_count)
{
    m_row_count = row_count;
    m_col_count = col_count;
    m_non_zero_count.resize(row_count);
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::reserve(qlonglong non_zero_count)
{
    Q_UNUSED(non_zero_count);
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::append(qlonglong i, qlonglong j, const T& value)
{
    m_map.insert(qMakePair(i, j), value);
    m_non_zero_count[i] += 1;
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::insert(qlonglong i, qlonglong j, const T& value)
{
    m_map.insert(qMakePair(i, j), value);
    m_non_zero_count[i] += 1;
}

template < typename T >
T dtkSparseMatrixHandlerHash<T>::value(qlonglong pos) const
{
    typename QMap<MatrixEntry, T>::const_iterator it(m_map.constBegin()); it += pos;
    return it.value();
}

template < typename T >
void dtkSparseMatrixHandlerHash<T>::setValue(qlonglong pos, const T& value)
{
    typename QMap<MatrixEntry, T>::iterator it(m_map.begin()); it += pos;
    it.value() = value;
}

template < typename T >
QPair<qlonglong, qlonglong> dtkSparseMatrixHandlerHash<T>::coordinates(qlonglong pos) const
{
    typename QMap<MatrixEntry, T>::const_iterator it(m_map.constBegin()); it += pos;
    return it.key();
}

template < typename T >
qlonglong dtkSparseMatrixHandlerHash<T>::pos(qlonglong i, qlonglong j) const
{
    typename QMap<MatrixEntry, T>::const_iterator end = m_map.find(qMakePair(i, j));
    typename QMap<MatrixEntry, T>::const_iterator it = m_map.begin();
    qlonglong pos = 0;

    while (it != end ) {
        ++pos;
        ++it;
    }

    return pos;
}
