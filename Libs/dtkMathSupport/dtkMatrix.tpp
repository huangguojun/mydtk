/* dtkMatrix.tpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */


#ifndef DTKMATRIX_TPP
#define DTKMATRIX_TPP

#include <dtkMathSupportExport.h>

#include <iostream>
#include <iomanip>
#include <typeinfo>

#include <QtCore/QDebug>
#include <QtCore/QString>

enum { N_NOTALLOCATED, N_ALLOCATED, N_MAPPED };

namespace dtkDeprecated {

// /////////////////////////////////////////////////////////////////
// Template specializations forward declarations
// /////////////////////////////////////////////////////////////////

    template <> DTKMATHSUPPORT_EXPORT QByteArray *dtkMatrix<double>::serialize(void);
    template <> DTKMATHSUPPORT_EXPORT void dtkMatrix<double>::deserialize(const QByteArray& array);

// /////////////////////////////////////////////////////////////////
// dtkMatrix implementation
// /////////////////////////////////////////////////////////////////

//! Default constructor.
    template <typename T> inline dtkMatrix<T>::dtkMatrix(void)
    {
        initialize();
    };

//! Creates a matrix, allocates rows and columns
    template <typename T> inline dtkMatrix<T>::dtkMatrix(unsigned crow, unsigned ccol)
    {
        initialize();
        allocate(crow, ccol);
    }

//! Creates a matrix, allocates rows and columns
    template <typename T> inline dtkMatrix<T>::dtkMatrix(T *array, unsigned crow, unsigned ccol)
    {
        initialize();
        fromRawData(array, crow, ccol);
    }

//! Copy constructor.
    template <typename T> inline dtkMatrix<T>::dtkMatrix(const dtkMatrix<T>& mat)
    {
        initialize();

        if (mat.m_nMatStatus == N_NOTALLOCATED)
            return;
        else
            (*this) = mat ;
    }

//! Mapped matrix constructor.
    template <typename T> inline dtkMatrix<T>::dtkMatrix(const dtkMatrix<T>& mat,
            unsigned irowStart,
            unsigned icolStart,
            unsigned irowEnd,
            unsigned icolEnd)
    {
        initialize();
        mapInto(mat, irowStart, icolStart, irowEnd, icolEnd);
    }

    template <typename T> inline dtkMatrix<T>::~dtkMatrix(void)
    {
        if (m_crow)
            deallocate();
    }

//! Private method giving initial values for matrix attributes.
    template <typename T> inline void dtkMatrix<T>::initialize(void)
    {
        m_crow       = 0;
        m_ccol       = 0;
        m_rgrow      = NULL;
        m_nMatStatus = N_NOTALLOCATED;
    }

//! Returns identifier of the matrix.
    template <typename T> QString dtkMatrix<T>::identifier(void) const
    {
        return QString("dtkMatrix<%1>").arg(typeid(T).name());
    }

//! Returns description of the contain of the matrix.
    template <typename T> QString dtkMatrix<T>::description(void) const
    {
        QString string;

        string = "[ " ;

        for (unsigned i = 0; i < numberOfRows(); i++) {
            if (i > 0)
                string.append("; ");

            QString string2 = QString("%1").arg(m_rgrow[i][0]);
            string += string2;

            for (unsigned j = 1; j < numberOfColumns(); j++) {
                string.append(", ");
                QString string3 = QString("%1").arg(m_rgrow[i][j]);
                string += string3;
            }
        }

        string.append(" ]");

        return string;
    }

//!
    template <typename T> void dtkMatrix<T>::allocate(unsigned crowInit, unsigned ccolInit)
    {
        if (m_nMatStatus != N_NOTALLOCATED) {
            if (m_crow == crowInit && m_ccol == ccolInit)
                return;
            else
                deallocate();
        }

        m_crow = crowInit;
        m_ccol = ccolInit;

        if ((m_crow * m_ccol) == 0) {
            m_nMatStatus = N_NOTALLOCATED;
            m_crow = 0;
            m_ccol = 0;
            return;
        }


        m_rgrow = new T*[m_crow];
        T *ptTmp = new T[m_crow * m_ccol];

        for (unsigned irow = 0; irow < m_crow; ++irow)
            m_rgrow[irow] = &(ptTmp[irow * m_ccol]);

        m_nMatStatus = N_ALLOCATED;
    }

//!
    template <typename T> void dtkMatrix<T>::deallocate(void)
    {
        switch (m_nMatStatus) {
        case N_NOTALLOCATED:
            break;

        case N_MAPPED:
            delete [] m_rgrow;
            break;

        case N_ALLOCATED:
            delete [] *m_rgrow;
            delete [] m_rgrow;
            break;
        };

        initialize();
    }

//!
    /*!
     *
     */
    template <typename T> void dtkMatrix<T>::fromRawData(T *array, unsigned crowInit, unsigned ccolInit)
    {
        if (m_nMatStatus != N_NOTALLOCATED)
            deallocate();

        m_crow = crowInit;
        m_ccol = ccolInit;

        if ((m_crow * m_ccol) == 0) {
            m_nMatStatus = N_NOTALLOCATED;
            m_crow = 0;
            m_ccol = 0;
            return;
        }

        m_rgrow = new T*[m_crow];
        T *ptTmp = array;

        for (unsigned irow = 0; irow < m_crow; ++irow)
            m_rgrow[irow] = &(ptTmp[irow * m_ccol]);

        m_nMatStatus = N_MAPPED;
    }

//!
    /*!
     * Maps a matrix into another matrix, deallocates first if neccesary
     * allocates space on the free store for a matrix, deallocates first
     * if neccesary.
     */
    template <typename T> void dtkMatrix<T>::mapInto(const dtkMatrix<T>& mat,
            unsigned irowStart,
            unsigned icolStart,
            unsigned irowEnd,
            unsigned icolEnd )
    {
        if (m_nMatStatus != N_NOTALLOCATED)
            deallocate();

        m_crow = irowEnd - irowStart + 1;
        m_ccol = icolEnd - icolStart + 1;
        m_rgrow = new T*[m_crow];

        for (unsigned irow = 0; irow < m_crow; ++irow)
            m_rgrow[irow] = &mat.m_rgrow[irow + irowStart][icolStart];

        m_nMatStatus = N_MAPPED;
    }

    template <typename T> inline int dtkMatrix<T>::getStatus(void) const
    {
        return m_nMatStatus;
    }

    template <typename T> inline unsigned dtkMatrix<T>::getRows(void) const
    {
        return m_crow;
    }

    template <typename T> inline unsigned dtkMatrix<T>::getCols(void) const
    {
        return m_ccol;
    }

//!
    template <typename T> dtkMatrix<T> dtkMatrix<T>::operator +(const dtkMatrix<T>& mat) const
    {
        return dtkMatrix<T>( *this ) += mat;
    }

//!
    template <typename T> dtkMatrix<T> dtkMatrix<T>::operator -(const dtkMatrix<T>& mat) const
    {
        return dtkMatrix<T>( *this ) -= mat;
    }

//!
    template <typename T> dtkMatrix<T> dtkMatrix<T>::operator -(void) const
    {
        T tTmp  = dtkZero<T>();
        tTmp -= dtkUnity<T>();
        return (*this) * tTmp;
    }

//!
    template <typename T> dtkMatrix<T> dtkMatrix<T>::operator *(const T& value) const
    {
        return dtkMatrix<T>( *this ) *= value;
    }

//!
    template <typename T> dtkMatrix<T> dtkMatrix<T>::operator *(const dtkMatrix<T>& mat) const
    {
        dtkMatrix matResult( m_crow , mat.m_ccol );
        matResult.storeProduct( *this , mat );
        return matResult;
    }

//! Assignment operator.
    template <typename T> dtkMatrix<T>& dtkMatrix<T>::operator =(const dtkMatrix<T>& mat)
    {
        if (m_nMatStatus == N_NOTALLOCATED)
            allocate(mat.m_crow, mat.m_ccol);

        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] = mat.m_rgrow[irow][icol];

        return *this;
    }

//!
    template <typename T> dtkMatrix<T>& dtkMatrix<T>::operator +=(const dtkMatrix<T>& mat)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] += mat.m_rgrow[irow][icol];

        return *this;
    }

//!
    template <typename T> dtkMatrix<T>& dtkMatrix<T>::operator -=(const dtkMatrix<T>& mat)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] -= mat.m_rgrow[irow][icol];

        return *this;
    }

//!
    template <typename T> dtkMatrix<T>& dtkMatrix<T>::operator *=(const T& value)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] *= value;

        return *this;
    }

//!
    template <typename T> int dtkMatrix<T>::operator ==(const dtkMatrix<T>& mat) const
    {
        if ((m_crow == mat.m_crow) && (m_ccol == mat.m_ccol)) {
            for (unsigned irow = 0; irow < m_crow; ++irow) {
                for (unsigned icol = 0; icol < m_ccol; ++icol)
                    if (m_rgrow[irow][icol] != mat.m_rgrow[irow][icol])
                        return 0;
            }

            return 1;
        } else
            return 0;
    }

//! Stores mat1 + mat2 in this.
    template <typename T> void dtkMatrix<T>::storeSum(const dtkMatrix<T>& mat1,
            const dtkMatrix<T>& mat2)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] = mat1.m_rgrow[irow][icol] + mat2.m_rgrow[irow][icol];
    }

//! Stores mat1*mat2 in this.
    template <typename T> void dtkMatrix<T>::storeProduct(const dtkMatrix<T>& mat1,
            const dtkMatrix<T>& mat2)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow) {
            for (unsigned icol = 0; icol < m_ccol; ++icol) {
                m_rgrow[irow][icol] = dtkZero<T>();

                for (unsigned icol2 = 0; icol2 < mat1.m_ccol; ++icol2)
                    m_rgrow[irow][icol] += mat1.m_rgrow[irow][icol2] * mat2.m_rgrow[icol2][icol];
            }
        }
    }

//! Stores a matrix mat at position (irowStart,icolStart). Matrix mat should fit in this.
    template <typename T> void dtkMatrix<T>::storeAtPosition(unsigned irowStart,
            unsigned icolStart,
            const dtkMatrix<T>& mat)
    {
        for (unsigned irow = 0; irow < mat.m_crow; ++irow)
            for (unsigned icol = 0; icol < mat.m_ccol; ++icol)
                m_rgrow[irow + irowStart][icol + icolStart] = mat.m_rgrow[irow][icol];
    }

//! Stores transpose of mat in this.
    template <typename T> void dtkMatrix<T>::storeTranspose(const dtkMatrix<T>& mat)
    {
        for (unsigned irow = 0; irow < m_crow; ++irow)
            for (unsigned icol = 0; icol < m_ccol; ++icol)
                m_rgrow[irow][icol] = mat.m_rgrow[icol][irow];
    }

//!
    template <typename T> inline void dtkMatrix<T>::fill(const T& elemFill)
    {
        unsigned iEnd = m_crow * m_ccol;

        for (unsigned i = 0; i < iEnd; ++i)
            (*m_rgrow)[i] = elemFill;
    }

//!
    template <typename T> inline void dtkMatrix<T>::makeTranspose(void)
    {
        T temp;

        for (unsigned irow = 0; irow < m_crow; ++irow) {
            for (unsigned icol = 0; icol < m_ccol; ++icol) {
                temp = m_rgrow[irow][icol];
                m_rgrow[irow][icol] = m_rgrow[icol][irow];
                m_rgrow[icol][irow] = temp;
            }
        }
    }

//!
    template <typename T> void dtkMatrix<T>::interchangeRows(unsigned irow1, unsigned irow2)
    {
        T *rowSav = m_rgrow[irow1];

        m_rgrow[irow1] = m_rgrow[irow2];
        m_rgrow[irow2] = rowSav;
    }

//!
    template <typename T> void dtkMatrix<T>::multiplyRow(unsigned irow, const T& value)
    {
        for (unsigned icol = 0; icol < m_ccol; ++icol)
            m_rgrow[irow][icol] *= value;
    }

//!
    template <typename T> void dtkMatrix<T>::addRowToRow(unsigned irowSrc,
            unsigned irowDest,
            const T& value)
    {
        for (unsigned icol = 0; icol < m_ccol; ++icol)
            m_rgrow[irowDest][icol] += m_rgrow[irowSrc][icol] * value;
    }

//! Returns product mat*value.
    template <typename T> inline dtkMatrix<T> operator *(const T& value,
            const dtkMatrix<T>& mat)
    {
        return mat * value;
    }

//! Returns transpose of mat.
    template <typename T> dtkMatrix<T> dtkTranspose(const dtkMatrix<T>& mat)
    {
        dtkMatrix<T> matTranspose( mat.numberOfColumns(), mat.numberOfRows() );
        matTranspose.storeTranspose(mat);
        return matTranspose;
    }

//!
    template <typename T> std::ostream& operator <<(std::ostream& os, const dtkMatrix<T>& mat)
    {
        os << "[" ;

        for (unsigned i = 0; i < mat.numberOfRows(); i++) {
            if (i > 0)
                os << "; ";

            os  << mat[i][0];

            for (unsigned j = 1; j < mat.numberOfColumns(); j++)
                os  << ", " << mat[i][j];
        }

        os << "]";

        return os;
    }

//!
    template <typename T> QDebug operator<<(QDebug dbg, const dtkMatrix<T>& mat)
    {
        dbg.nospace() << "[" ;

        for (unsigned i = 0; i < mat.numberOfRows(); i++) {
            if (i > 0)
                dbg.nospace() << ";";

            dbg.nospace() << mat[i][0];

            for (unsigned j = 1; j < mat.numberOfColumns(); j++)
                dbg.nospace() << "," << mat[i][j];
        }

        dbg.nospace() << "]";

        return dbg.space();
    }

//!
    template <typename T> QDebug operator<<(QDebug dbg, const dtkMatrix<T> *mat)
    {
        dbg.nospace() << "[" ;

        for (unsigned i = 0; i < mat->numberOfRows(); i++) {
            if (i > 0)
                dbg.nospace() << ";";

            dbg.nospace() << mat[i][0];

            for (unsigned j = 1; j < mat->numberOfColumns(); j++)
                dbg.nospace() << "," << mat[i][j];
        }

        dbg.nospace() << "]";

        return dbg.space();
    }

    template <typename T> QByteArray *dtkMatrix<T>::serialize(void)
    {
        return NULL;
    };


    template <typename T>  void dtkMatrix<T>::deserialize(const QByteArray& array)
    {

    };

} // end namespace

#endif
