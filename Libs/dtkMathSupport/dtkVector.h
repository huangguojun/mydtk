/* dtkVector.h ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 * Created: Tue Jun  8 14:26:31 2010 (+0200)
 * Version: $Id: 82fe818f418049a71caf1a9c8f667f8f322de570 $
 * Last-Updated: jeu. mars 26 11:21:41 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 75
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKVECTOR_H
#    define DTKVECTOR_H

#    include <cmath>

#    include "dtkMatrix.h"

// /////////////////////////////////////////////////////////////////
// dtkVector interface
// /////////////////////////////////////////////////////////////////

namespace dtkDeprecated {

template<class T = double>
class dtkVector : public dtkMatrix<T>
{
public:
    dtkVector(void) : dtkMatrix<T>(){};
    dtkVector(unsigned crowInit) : dtkMatrix<T>(crowInit, 1){};
    dtkVector(T *array, unsigned crowInit) : dtkMatrix<T>(array, crowInit, 1){};
    dtkVector(const dtkMatrix<T> &mat) : dtkMatrix<T>(mat){};
    dtkVector(const dtkVector &vec) : dtkMatrix<T>(vec){};
    dtkVector(const dtkMatrix<T> &, unsigned, unsigned, unsigned);
    dtkVector(const dtkVector &, unsigned, unsigned);
    virtual ~dtkVector(void){};

public:
    QString identifier(void) const;

public:
    void allocate(unsigned crowInit) { dtkMatrix<T>::allocate(crowInit, 1); }
    void allocate(unsigned crowInit, T *buffer) { dtkMatrix<T>::allocate(crowInit, 1, buffer); }

    void fromRawData(T *array, unsigned crowInit) { dtkMatrix<T>::fromRawData(array, crowInit, 1); }

    void mapInto(const dtkMatrix<T> &, unsigned, unsigned, unsigned);
    void mapInto(const dtkVector &, unsigned, unsigned);

    unsigned size(void) const { return this->numberOfRows(); };

public:
    T &operator[](unsigned irow) { return this->dtkMatrix<T>::operator[](irow)[0]; }
    const T &operator[](unsigned irow) const { return this->dtkMatrix<T>::operator[](irow)[0]; }

public:
    dtkVector operator+(const dtkVector &) const;
    dtkVector operator-(const dtkVector &) const;
    dtkVector operator-(void) const;
    dtkVector operator*(const T &)const;

    T operator*(const dtkVector &)const;

    dtkVector operator/(const T &value) const
    {
        T tTmp = dtkUnity<T>();
        tTmp /= value;
        return (*this) * tTmp;
    }

public:
    dtkVector &operator=(const dtkVector &vec);
    dtkVector &operator+=(const dtkVector &vec);
    dtkVector &operator-=(const dtkVector &vec);
    dtkVector &operator*=(const T &value);
    dtkVector &operator/=(const T &value);

    T operator!(void) const { return (*this).norm(); };

public:
    void storeAtRow(unsigned, const dtkVector &);

    T norm(void) const;

    dtkVector unit(void) const { return (*this) / this->norm(); }

    void makeUnit(void) { (*this) /= this->norm(); }
};

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

template<class T, unsigned crow>
class dtkVec : public T
{
public:
    dtkVec(void) : T(crow) {}

    T &operator=(const T &mtx) { return T::operator=(mtx); }
};

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

template<class T, unsigned crow>
class dtkZero<dtkVec<dtkVector<T>, crow>> : public dtkVec<dtkVector<T>, crow>
{
public:
    dtkZero(void) { fill(dtkZero<T>()); }
};

} // namespace dtkDeprecated

// /////////////////////////////////////////////////////////////////
// Implementation of the template class dtkVector's methods
// /////////////////////////////////////////////////////////////////

#    include "dtkVector.tpp"

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

#    include <QtCore>

typedef dtkDeprecated::dtkVector<qlonglong> dtkVectorInteger;

Q_DECLARE_METATYPE(dtkVectorInteger);
Q_DECLARE_METATYPE(dtkVectorInteger *);

typedef dtkDeprecated::dtkVector<qreal> dtkVectorReal;

Q_DECLARE_METATYPE(dtkVectorReal);
Q_DECLARE_METATYPE(dtkVectorReal *);

#endif

// /////////////////////////////////////////////////////////////////
// Credits
// /////////////////////////////////////////////////////////////////

// $Id: 82fe818f418049a71caf1a9c8f667f8f322de570 $

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
