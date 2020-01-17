/* dtkMatrixSquared.tpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 * Created: Mon Jul 12 15:58:19 2010 (+0200)
 * Version: $Id: f43ad5141b744efad7fb2267edafcfbff71f7e56 $
 * Last-Updated: jeu. mars 26 11:13:04 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 22
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKMATRIXSQUARED_TPP
#define DTKMATRIXSQUARED_TPP

#include "dtkMatrix.h"

namespace dtkDeprecated {

// /////////////////////////////////////////////////////////////////
// dtkMatrixSquared implementation
// /////////////////////////////////////////////////////////////////

    template <typename T> inline dtkMatrixSquared<T>::dtkMatrixSquared(const dtkMatrix<T>& mat,
            unsigned irowStart,
            unsigned icolStart,
            unsigned irowEnd) : dtkMatrix<T>(mat,
                        irowStart,
                        icolStart,
                        irowEnd,
                        icolStart + irowEnd - irowStart)
    {
    }

    template <typename T> inline dtkMatrixSquared<T>::dtkMatrixSquared(const dtkMatrixSquared<T>& matSquared,
            unsigned irowStart,
            unsigned icolStart,
            unsigned irowEnd) : dtkMatrix<T>(matSquared,
                        irowStart,
                        icolStart,
                        irowEnd,
                        icolStart + irowEnd - irowStart)
    {
    }

    template <typename T> QString dtkMatrixSquared<T>::identifier(void) const
    {
        return QString("dtkMatrixSquared<%1>").arg(typeid(T).name());
    }

    template <typename T> inline void dtkMatrixSquared<T>::mapInto(const dtkMatrixSquared<T>& matSquared,
            unsigned irowStart,
            unsigned icolStart,
            unsigned irowEnd)
    {
        dtkMatrix<T>::mapInto(matSquared,
                              irowStart,
                              icolStart,
                              irowEnd,
                              icolStart + irowEnd - irowStart);
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator =(const dtkMatrixSquared<T>& matSquared)
    {
        return static_cast<dtkMatrixSquared<T> &>(dtkMatrix<T>::operator=(matSquared));
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator +=(const dtkMatrixSquared<T>& matSquared)
    {
        return static_cast<dtkMatrixSquared<T> &>(dtkMatrix<T>::operator+=(matSquared));
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator -=(const dtkMatrixSquared& matSquared)
    {
        return static_cast<dtkMatrixSquared<T> &>(dtkMatrix<T>::operator-=(matSquared));
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator *=(const T& value)
    {
        return static_cast<dtkMatrixSquared<T> &>(dtkMatrix<T>::operator*=(value));
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator *=(const dtkMatrixSquared<T>& matSquared)
    {
        return (*this) = (*this) * matSquared;
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator /=(const T& value)
    {
        T tTmp = dtkUnity<T>();
        tTmp /= value;
        return (*this) *= tTmp;
    }

    template <typename T> inline dtkMatrixSquared<T>& dtkMatrixSquared<T>::operator /=(const dtkMatrixSquared<T>& matSquared)
    {
        return (*this) = (*this) / matSquared;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator +(const dtkMatrixSquared<T>& matSquared) const
    {
        return dtkMatrixSquared<T>(*this) += matSquared;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator -(const dtkMatrixSquared<T>& matSquared) const
    {
        return dtkMatrixSquared<T>(*this) -= matSquared;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator -(void) const
    {
        T tTmp = dtkZero<T>();
        tTmp -= dtkUnity<T>();
        return (*this) * tTmp;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator *(const T& value) const
    {
        return dtkMatrixSquared<T>(*this) *= value;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator *(const dtkMatrixSquared<T>& matSquared) const
    {
        dtkMatrixSquared matSquaredResult(this->size());
        matSquaredResult.storeProduct(*this, matSquared);
        return matSquaredResult;
    }

    template <typename T> dtkMatrixSquared<T> dtkMatrixSquared<T>::operator /(const dtkMatrixSquared<T>& matSquared) const
    {
        return (*this) * inv(matSquared);
    }

    template <typename T> void dtkMatrixSquared<T>::storeInverse(const dtkMatrixSquared<T>& matSquared)
    {
        (*this) = matSquared;
        makeInverse();
    }

    template <typename T> void dtkMatrixSquared<T>::makeUnity(void)
    {
        unsigned crow(this->numberOfRows()), ccol(this->numberOfColumns());

        for (unsigned irow = 0; irow < crow; ++irow) {
            for (unsigned icol = 0; icol < ccol; ++icol) {
                if (irow == icol)
                    (*this)[irow][icol] = dtkUnity<T>();
                else
                    (*this)[irow][icol] = dtkZero<T>();
            }
        }
    }

    template <typename T> void dtkMatrixSquared<T>::makeAdjoint(void)
    {
        // we need a copy of this
        dtkMatrixSquared matSquaredCopy(*this);
        // for easier access to crows
        unsigned crowCopy = matSquaredCopy.numberOfRows();
        T elemTmp;
        // will eventually contain det(matSquaredCopy)
        T elemDet = dtkUnity<T>();

        // make this a unity matrix
        makeUnity();

        // start row reduction
        for (unsigned irow = 0; irow < crowCopy; ++irow) {
            // if element [irow][irow] is zero, add a row with non-zero
            // element at column irow
            if (matSquaredCopy[irow][irow] == dtkZero<T>()) {
                for (unsigned irowTmp = irow; irowTmp < crowCopy; ++irowTmp) {
                    if (matSquaredCopy[irowTmp][irow] != dtkZero<T>()) {
                        // has no effect on adj(matSquared)
                        matSquaredCopy.addRowToRow(irowTmp, irow);
                        // repeat action on this
                        this->addRowToRow(irowTmp, irow);
                        break;
                    };
                };
            };

            elemTmp = matSquaredCopy[irow][irow];

            T tTmp = dtkUnity<T>();

            tTmp /= elemTmp;

            matSquaredCopy.multiplyRow(irow, tTmp);

            // repeat action on this
            multiplyRow(irow, tTmp);

            elemDet *= elemTmp;

            for (unsigned irowToAddTo = 0; irowToAddTo < crowCopy; ++irowToAddTo) {
                if (irowToAddTo != irow) {
                    elemTmp = -matSquaredCopy[irowToAddTo][irow];
                    matSquaredCopy.addRowToRow(irow, irowToAddTo, elemTmp);
                    // repeat action on this
                    addRowToRow(irow, irowToAddTo, elemTmp);
                };
            };
        };

        // this now contains its adjoint
        (*this) *= elemDet;
    }

    template <typename T> void dtkMatrixSquared<T>::storeAdjoint(const dtkMatrixSquared<T>& matSquared)
    {
        (*this) = matSquared;
        makeAdjoint();
    }

    template <typename T> void dtkMatrixSquared<T>::makeInverse(void)
    {
        // we need a copy of this
        dtkMatrixSquared matSquaredCopy(*this);
        // for easier access to crows
        unsigned crowCopy = matSquaredCopy.numberOfRows();
        T elemTmp;

        // make this a unity matrix
        makeUnity();

        // start row reduction
        for (unsigned irow = 0; irow < crowCopy; ++irow) {
            // if element [irow][irow] is zero, add a row with non-zero
            // element at column irow
            if (matSquaredCopy[irow][irow] == dtkZero<T>()) {
                for (unsigned irowTmp = irow; irowTmp < crowCopy; ++irowTmp) {
                    // has no effect on inv(matSquared)
                    if (matSquaredCopy[irowTmp][irow] != dtkZero<T>()) {
                        matSquaredCopy.addRowToRow(irowTmp, irow);
                        // repeat action on this
                        this->addRowToRow(irowTmp, irow);
                        break;
                    };
                };
            };

            elemTmp = matSquaredCopy[irow][irow];

            // NOTE: This used to work with g++ <= 3.2.
            // matSquaredCopy.multiplyRow(irow,
            //           static_cast<const T &>(dtkUnity<T>()/elemTmp));
            // multiplyRow(irow, static_cast<const T &>(dtkUnity<T>()/elemTmp));
            T tTmp = dtkUnity<T>();

            tTmp /= elemTmp;

            matSquaredCopy.multiplyRow(irow, tTmp);

            this->multiplyRow(irow, tTmp);

            for (unsigned irowToAddTo = 0; irowToAddTo < crowCopy; ++irowToAddTo) {
                if (irowToAddTo != irow) {
                    elemTmp = -matSquaredCopy[irowToAddTo][irow];
                    matSquaredCopy.addRowToRow(irow, irowToAddTo, elemTmp);
                    // repeat action on this
                    this->addRowToRow(irow, irowToAddTo, elemTmp);
                };
            };
        };

        // this now contains its inverse
    }

    template <typename T> inline dtkMatrixSquared<T> operator *(const T& value,
            const dtkMatrixSquared<T>& matSquared)
    {
        return matSquared * value;
    }

    template <typename T> dtkMatrixSquared<T> dtkTranspose(const dtkMatrixSquared<T>& matSquared)
    {
        dtkMatrixSquared<T> matSquaredResult(matSquared.size());
        matSquaredResult.storeTranspose(matSquared);
        return matSquaredResult;
    }

    template <typename T> dtkMatrixSquared<T> dtkAdjoint(const dtkMatrixSquared<T>& matSquared)
    {
        dtkMatrixSquared<T> matSquaredResult(matSquared);
        matSquaredResult.makeAdjoint();
        return matSquaredResult;
    }

    template <typename T> dtkMatrixSquared<T> dtkInverse(const dtkMatrixSquared<T>& matSquared)
    {
        dtkMatrixSquared<T> matSquaredResult(matSquared);
        matSquaredResult.makeInverse();
        return matSquaredResult;
    }

    template <typename T> T dtkDeterminant(const dtkMatrixSquared<T>& matSquared)
    {
        // a copy of the input matrix
        dtkMatrixSquared<T> matSquaredCopy(matSquared);
        unsigned crowCopy = matSquaredCopy.numberOfRows();

        // start row reduction
        T elemTmp;
        // will eventually contain det(matSquaredCopy)
        T elemDet = dtkUnity<T>();

        for (unsigned irow = 0; irow < crowCopy; ++irow) {
            // if element [irow][irow] is zero, add a row with non-zero
            // element at column irow
            if (matSquaredCopy[irow][irow] == dtkZero<T>()) {
                for (unsigned irowTmp = irow; irowTmp < crowCopy; ++irowTmp) {
                    // has no effect on inv(matSquared)
                    if (matSquaredCopy[irowTmp][irow] != dtkZero<T>()) {
                        matSquaredCopy.addRowToRow(irowTmp, irow);
                        break;
                    };
                };
            };

            elemTmp =  matSquaredCopy[irow][irow];

            elemDet *= elemTmp;

            if (elemDet == dtkZero<T>()) {
                // once elemDet is zero it will stay zero
                return elemDet;
            }

            matSquaredCopy.multiplyRow(irow, dtkUnity<T>() / elemTmp);

            for (unsigned irowToAddTo = 0; irowToAddTo < crowCopy; ++irowToAddTo) {
                if (irowToAddTo != irow) {
                    matSquaredCopy.addRowToRow(irow,
                                               irowToAddTo,
                                               -matSquaredCopy[irowToAddTo][irow]);
                };
            };
        };

        return elemDet;
    }

    template <typename T> T dtkMatrixSquaredTrace(const dtkMatrixSquared<T>& matSquared)
    {
        T elemTmp = dtkZero<T>();

        for (uint i = 0; i < matSquared.size(); i++)
            elemTmp += matSquared[i][i];

        return elemTmp;
    }

} // end of namespace

#endif
