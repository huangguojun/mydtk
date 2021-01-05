/* dtkMatrix.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Jun  8 13:10:13 2010 (+0200)
 * Version: $Id: 5f3e508692ccd90585706206e5e7b44eddc2227d $
 * Last-Updated: jeu. mars 26 16:39:16 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 227
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKMATRIX_H
#    define DTKMATRIX_H

#    include <QtCore>

namespace dtkDeprecated {

//! Template class dtkZero
/*!
 * A class that represents zero.  It is useful because it can also
 * represent zero for structured types provided the notion of zero
 *  exists for that type.
 */
template<typename T>
class dtkZero
{
public:
    operator T() { return 0; }
};

//! Template class dtkUnity
/*!
 * A class that represents unity.  It is useful because it can also
 * represent unity for structured types provided the notion of unity
 * exists for that type.
 */
template<typename T>
class dtkUnity
{
public:
    operator T() { return 1; }
};

//! Template class dtkMatrix
/*!
 * Template class dtkMatrix provides a mathematical matrix with parameterised
 * dimensions and elements. It provides the matrix operations that are used
 * extensively in engineering and science. The unsigned template arguments r
 * ans c represent the matrix's dtkber of rows and columns respectively.
 * Template argument T represents the type of elements that are in the matrix.
 * The default element type for this template is double.
 */
template<typename T = double>
class dtkMatrix
{
public:
    typedef T element;

public:
    dtkMatrix(void);
    dtkMatrix(unsigned, unsigned);
    dtkMatrix(T *, unsigned, unsigned);
    dtkMatrix(const dtkMatrix &);
    dtkMatrix(const dtkMatrix &, unsigned, unsigned, unsigned, unsigned);
    virtual ~dtkMatrix(void);

private:
    void initialize(void);

public:
    virtual QString identifier(void) const;
    QString description(void) const;

public:
    void allocate(unsigned, unsigned);
    void deallocate(void);

    void fromRawData(T *, unsigned, unsigned);
    void mapInto(const dtkMatrix &, unsigned, unsigned, unsigned, unsigned);

    Q_DECL_DEPRECATED int getStatus(void) const;
    Q_DECL_DEPRECATED unsigned getRows(void) const;
    Q_DECL_DEPRECATED unsigned getCols(void) const;

    inline int status(void) const { return m_nMatStatus; };
    inline unsigned numberOfRows(void) const { return m_crow; };
    inline unsigned numberOfColumns(void) const { return m_ccol; };

public:
    T *operator[](unsigned irow) { return m_rgrow[irow]; };

    const T *operator[](unsigned irow) const { return m_rgrow[irow]; };

    dtkMatrix operator+(const dtkMatrix &) const;
    dtkMatrix operator-(const dtkMatrix &) const;
    dtkMatrix operator-(void) const;
    dtkMatrix operator*(const T &)const;
    dtkMatrix operator*(const dtkMatrix &)const;
    dtkMatrix operator/(const T &value) const
    {
        T tTmp = dtkUnity<T>();
        tTmp /= value;
        return (*this) * tTmp;
    }

public:
    dtkMatrix &operator=(const dtkMatrix &);
    dtkMatrix &operator+=(const dtkMatrix &);
    dtkMatrix &operator-=(const dtkMatrix &);
    dtkMatrix &operator*=(const T &);
    dtkMatrix &operator/=(const T &value)
    {
        T tTmp = dtkUnity<T>();
        tTmp /= value;
        return (*this) *= tTmp;
    }

public:
    int operator==(const dtkMatrix &) const;
    int operator!=(const dtkMatrix &mat) const { return !((*this) == mat); }

public:
    void storeSum(const dtkMatrix &, const dtkMatrix &);
    void storeProduct(const dtkMatrix &, const dtkMatrix &);
    void storeTranspose(const dtkMatrix &);
    void storeAtPosition(unsigned, unsigned, const dtkMatrix &);

    void fill(const T &);

    void makeTranspose(void);

    void interchangeRows(unsigned, unsigned);
    void addRowToRow(unsigned, unsigned, const T & = dtkUnity<T>());
    void multiplyRow(unsigned, const T &);

public:
    QByteArray *serialize(void);
    void deserialize(const QByteArray &array);

private:
    unsigned m_crow;
    unsigned m_ccol;
    T **m_rgrow;

    int m_nMatStatus;
};

//! Template class dtkMat
/*!
 * Templates to create self-dimensioning dtkMatrix classes - or one
 * of its derived classes - using the syntax of a default
 * constructor. This facility is required for using matrices as
 * elements of matrices since these will always be created by a call
 * to the default constructor.
 */
template<typename T, unsigned crow, unsigned ccol>
class dtkMat : public T
{
public:
    dtkMat(void) : T(crow, ccol) {}

    T &operator=(const T &mtx) { return T::operator=(mtx); }
};

//! Template class dtkZero
/*!
 * A zero matrix can only be constructed for a
 * matrix of known dimensions. Hence the use of dtkMat<T,n,m>.
 */
template<typename T, unsigned crow, unsigned ccol>
class dtkZero<dtkMat<dtkMatrix<T>, crow, ccol>> : public dtkMat<dtkMatrix<T>, crow, ccol>
{
public:
    dtkZero(void) { fill(dtkZero<T>()); }
};

} // namespace dtkDeprecated

// /////////////////////////////////////////////////////////////////
// Implementation of the template class dtkMatrix's methods
// /////////////////////////////////////////////////////////////////

#    include "dtkMatrix.tpp"

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

#    include <QtCore>

typedef dtkDeprecated::dtkMatrix<qlonglong> dtkMatrixInteger;
typedef dtkDeprecated::dtkMatrix<qreal> dtkMatrixReal;

Q_DECLARE_METATYPE(dtkMatrixReal);
Q_DECLARE_METATYPE(dtkMatrixReal *);

#endif

// /////////////////////////////////////////////////////////////////
// Credits

// /////////////////////////////////////////////////////////////////

// $Id: 5f3e508692ccd90585706206e5e7b44eddc2227d $

// CwMtx matrix and vector math library
// Copyright (C) 1999-2001  Harry Kuiper
// Copyright (C) 2000  Will DeVore (template conversion)

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
