/* dtkQuaternion.tpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 * Created: Mon Jul 12 16:18:19 2010 (+0200)
 * Version: $Id: 7787d17a64131dea86035c5f1195c74ef74792f8 $
 * Last-Updated: jeu. mars 26 11:16:30 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 6
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */


#ifndef DTKQUATERNION_TPP
#define DTKQUATERNION_TPP

#include "dtkVector.h"
#include "dtkVector3D.h"

using std::exp;
using std::log;

// /////////////////////////////////////////////////////////////////
// dtkQuaternion implementation
// /////////////////////////////////////////////////////////////////

namespace dtkDeprecated {

    template <class T> inline dtkQuaternion<T>::dtkQuaternion(const T& elemIm0,
            const T& elemIm1,
            const T& elemIm2,
            const T& elemReal) : dtkVector<T>(4U)
    {
        (*this)[0] = elemIm0;
        (*this)[1] = elemIm1;
        (*this)[2] = elemIm2;
        (*this)[3] = elemReal;
    }

    template <class T> inline dtkQuaternion<T>::dtkQuaternion(const T& elemReal) : dtkVector<T>(4U)
    {
        (*this)[0] = dtkZero<T>();
        (*this)[1] = dtkZero<T>();
        (*this)[2] = dtkZero<T>();
        (*this)[3] = elemReal;
    }

    template <class T> inline dtkQuaternion<T>::dtkQuaternion(const dtkVector3D<T>& vecIm,
            const T& elemReal) : dtkVector<T>(4U)
    {
        (*this)[0] = vecIm[0];
        (*this)[1] = vecIm[1];
        (*this)[2] = vecIm[2];
        (*this)[3] = elemReal;
    }

    template <class T> inline dtkQuaternion<T>::dtkQuaternion(const dtkMatrix<T>& mat,
            unsigned irowStart,
            unsigned icolStart) : dtkVector<T>(mat, irowStart, icolStart, irowStart + 3)
    {
    }

    template <class T> inline dtkQuaternion<T>::dtkQuaternion(const dtkVector<T>& vec,
            unsigned irowStart) : dtkVector<T>(vec, irowStart, irowStart + 3)
    {
    }

    template <class T> dtkQuaternion<T>::dtkQuaternion(const T& r,
            const dtkVector3D<T>& vec,
            const T& angle) : dtkVector<T>(4U)
    {
        T rsina = r * sin(angle);

        (*this)[0] = vec[0] * rsina;
        (*this)[1] = vec[1] * rsina;
        (*this)[2] = vec[2] * rsina;
        (*this)[3] = r * cos(angle);
    }

    template <typename T> QString dtkQuaternion<T>::identifier(void) const
    {
        return QString("dtkQuaternion<%1>").arg(typeid(T).name());
    }

    template <class T> inline void dtkQuaternion<T>::mapInto(const dtkMatrix<T>& mat,
            unsigned irowStart,
            unsigned icolStart)
    {
        dtkVector<T>::mapInto(mat, irowStart, icolStart, irowStart + 3);
    }

    template <class T> inline void dtkQuaternion<T>::mapInto(const dtkVector<T>& vec,
            unsigned irowStart)
    {
        dtkVector<T>::mapInto(vec, irowStart, irowStart + 3);
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator =(const dtkQuaternion<T>& qtn)
    {
        return static_cast<dtkQuaternion&>(dtkMatrix<T>::operator=(qtn));
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator =(const dtkVector3D<T>& vec)
    {
        (*this)[0] = vec[0];
        (*this)[1] = vec[1];
        (*this)[2] = vec[2];
        (*this)[3] = dtkZero<T>();

        return *this;
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator +=(const dtkQuaternion<T>& qtn)
    {
        return static_cast<dtkQuaternion&>(dtkMatrix<T>::operator+=(qtn));
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator -=(const dtkQuaternion<T>& qtn)
    {
        return static_cast<dtkQuaternion&>(dtkMatrix<T>::operator-=(qtn));
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator *=(const T& value)
    {
        return static_cast<dtkQuaternion&>(dtkMatrix<T>::operator*=(value));
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator *=(const dtkQuaternion<T>& qtn)
    {
        return (*this) = (*this) * qtn;
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator /=(const T& value)
    {
        T tTmp = dtkUnity<T>();
        tTmp /= value;
        return (*this) *= tTmp;
    }

    template <class T> inline dtkQuaternion<T>& dtkQuaternion<T>::operator /=(const dtkQuaternion<T>& qtn)
    {
        return (*this) = (*this) / qtn;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator +(const dtkQuaternion<T>& qtn) const
    {
        return dtkQuaternion<T>(*this) += qtn;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator -(const dtkQuaternion<T>& qtn) const
    {
        return dtkQuaternion<T>(*this) -= qtn;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator -(void) const
    {
        T tTmp = dtkZero<T>();
        tTmp -= dtkUnity<T>();
        return (*this) * tTmp;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator *(const T& value) const
    {
        return dtkQuaternion<T>(*this) *= value;
    }

    template <class T> void dtkQuaternion<T>::storeProduct(const dtkQuaternion<T>& qtnLeft,
            const dtkQuaternion<T>& qtnRight)
    {
        T qtnLeft0 = qtnLeft[0];
        T qtnLeft1 = qtnLeft[1];
        T qtnLeft2 = qtnLeft[2];
        T qtnLeft3 = qtnLeft[3];

        T qtnRight0 = qtnRight[0];
        T qtnRight1 = qtnRight[1];
        T qtnRight2 = qtnRight[2];
        T qtnRight3 = qtnRight[3];

        (*this)[0] =
            qtnLeft0 * qtnRight3 + qtnLeft1 * qtnRight2
            - qtnLeft2 * qtnRight1 + qtnLeft3 * qtnRight0;

        (*this)[1] =
            -qtnLeft0 * qtnRight2 + qtnLeft1 * qtnRight3
            + qtnLeft2 * qtnRight0 + qtnLeft3 * qtnRight1;

        (*this)[2] =
            qtnLeft0 * qtnRight1 - qtnLeft1 * qtnRight0
            + qtnLeft2 * qtnRight3 + qtnLeft3 * qtnRight2;

        (*this)[3] =
            -qtnLeft0 * qtnRight0 - qtnLeft1 * qtnRight1
            - qtnLeft2 * qtnRight2 + qtnLeft3 * qtnRight3;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator *(const dtkQuaternion<T>& qtn) const
    {
        dtkQuaternion qtnResult;
        qtnResult.storeProduct(*this, qtn);
        return qtnResult;
    }

    template <class T> dtkQuaternion<T> dtkQuaternion<T>::operator /(const dtkQuaternion<T>& qtn) const
    {
        return (*this) * dtkInverse(qtn);
    }

    template <class T> void dtkQuaternion<T>::storeConjugate(const dtkQuaternion<T>& qtn)
    {
        (*this) = qtn;
        makeConjugate();
    }

    template <class T> void dtkQuaternion<T>::makeConjugate(void)
    {
        T tTmp = dtkZero<T>();
        tTmp -= dtkUnity<T>();

        (*this)[0] *= tTmp;
        (*this)[1] *= tTmp;
        (*this)[2] *= tTmp;
    }

    template <class T> inline dtkQuaternion<T> operator *(const T& value,
            const dtkQuaternion<T>& qtn)
    {
        return qtn * value;
    }

    template <class T> inline T dtkRe(const dtkQuaternion<T>& qtn)
    {
        return qtn[3];
    }

    template <class T> dtkVector3D<T> dtkIm(const dtkQuaternion<T>& qtn)
    {
        return dtkVector3D<T>(qtn[0], qtn[1], qtn[2]);
    }

    template <class T> dtkQuaternion<T> dtkConjugate(const dtkQuaternion<T>& qtn)
    {
        dtkQuaternion<T> qtnResult(qtn);
        qtnResult.makeConjugate();
        return qtnResult;
    }

    template <class T> dtkQuaternion<T> dtkInverse(const dtkQuaternion<T>& qtn)
    {
        T qtn0 = qtn[0];
        T qtn1 = qtn[1];
        T qtn2 = qtn[2];
        T qtn3 = qtn[3];

        T tTmp = qtn0;
        tTmp *= qtn0;
        tTmp += qtn1 * qtn1 + qtn2 * qtn2 + qtn3 * qtn3;
        return dtkConjugate(qtn) / tTmp;
    }

    template <class T> inline dtkQuaternion<T> dtkDirection(const dtkQuaternion<T>& qtn)
    {
        return qtn.unit();
    }

    template <class T> inline T dtkArgument(const dtkQuaternion<T>& qtn)
    {
        return atan2(dtkNorm(dtkIm(qtn)), dtkRe(qtn));
    }

    template <class T> dtkQuaternion<T> dtkExp(const dtkQuaternion<T>& qtn)
    {
        dtkVector3D<T> vec = dtkIm(qtn);

        T len = dtkNorm(vec);

        if (len == dtkZero<T>())
            return dtkExp(dtkRe(qtn)) * dtkQuaternion<T>(dtkZero<T>(), dtkZero<T>(), dtkZero<T>(), cos(len));
        else
            return dtkExp(dtkRe(qtn)) * dtkQuaternion<T>(dtkDirection(vec) * sin(len), cos(len));
    }

    template <class T> dtkQuaternion<T> dtkLogQ(const dtkQuaternion<T>& qtn)
    {
        dtkVector3D<T> vec = dtkIm(qtn);

        T len = dtkNorm(vec);

        if (len == dtkZero<T>())
            return dtkQuaternion<T>(dtkZero<T>(), dtkZero<T>(), dtkZero<T>(), dtkLogQ(dtkNorm(qtn)));
        else
            return dtkQuaternion<T>(dtkDirection(vec) * dtkArgument(qtn), dtkLogQ(dtkNorm(qtn)));
    }

    template <class T> inline dtkQuaternion<T> dtkPowQ(const dtkQuaternion<T>& qtn1, const dtkQuaternion<T>& qtn2)
    {
        return dtkExp(qtn2 * dtkLogQ(qtn1));
    }

} // end of namespace

#endif
