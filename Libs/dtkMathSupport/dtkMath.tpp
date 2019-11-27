/* dtkMath.tpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Mar  9 11:05:40 2011 (+0100)
 * Version: $Id: 8cffd0949bd17bc6f0a94d365a92c566464412e6 $
 * Last-Updated: lun. mars 30 10:21:16 2015 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 131
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKMATH_TPP
#define DTKMATH_TPP

#include "dtkMatrixSquared.h"
#include "dtkQuaternion.h"
#include "dtkVector3D.h"

#include <math.h>

template<class T> T dtkDeg2Rad(const T& value)
{
    return M_PI * value / 180.;
}

template<class T> T dtkRad2Deg(const T& value)
{
    return 180. * value / M_PI;
}

template <class T> dtkDeprecated::dtkVector3D<T> dtkRotate(const dtkDeprecated::dtkQuaternion<T>& quaternion, const dtkDeprecated::dtkVector3D<T>& vector)
{
    dtkDeprecated::dtkQuaternion<T> q(quaternion * dtkDeprecated::dtkQuaternion<T>(vector[0], vector[1], vector[2], 0) * dtkDeprecated::dtkInverse<T>(quaternion));
    return dtkDeprecated::dtkVector3D<T>(q[0], q[1], q[2]);
}

//! dtkMixedProduct(const dtkVector3D &v0, const dtkVector3D &v1, const dtkVector3D &v2)
/*!
 * This function calculates the mixed product (also called triple scalar product)
 * formed by three dtkVector3D.
 */
template <class T>  T dtkMixedProduct(const dtkDeprecated::dtkVector3D<T>& v0,
                                      const dtkDeprecated::dtkVector3D<T>& v1,
                                      const dtkDeprecated::dtkVector3D<T>& v2)
{
    dtkDeprecated::dtkVector3D<T> v;
    v.storeOuterProduct(v1, v2);
    return (v0 * v);
}

//! dtkMixedProduct(const T v0[3], const T v1[3], const T v2[3])
/*!
 * This function calculates the mixed product (also called triple scalar product)
 * formed by three vectors v0, v1, v2.
 */
template <class T>  inline T dtkMixedProduct(const T v0[3],
        const T v1[3],
        const T v2[3])
{
    return v0[0] * v1[1] * v2[2] + v1[0] * v2[1] * v0[2] + v2[0] * v0[1] * v1[2] -
           v0[0] * v2[1] * v1[2] - v1[0] * v0[1] * v2[2] - v2[0] * v1[1] * v0[2];
}

//! dtkQuaternionFromEulerAxisAndAngle(const dtkVector3D<T> &dtkVecEulerAxis, const T &rotateEulerAngle)
/*!
 * This function returns a quaternion representing a rigid body's
 * attitude from its Euler axis and angle.
 * NOTE: Argument vector dtkVecEulerAxis MUST be a unit vector.
 */
template <class T> dtkDeprecated::dtkQuaternion<T> dtkQuaternionFromAxisAngle(const dtkDeprecated::dtkVector3D<T>& axis, const T& angle)
{
    dtkDeprecated::dtkVector3D<T> v = axis.unit();

    return dtkDeprecated::dtkQuaternion<T>(1.0, dtkDeprecated::dtkVector3D<T>(axis[0], axis[1], axis[2]), dtkDeg2Rad(angle / (T)2));
}

template <class T> dtkDeprecated::dtkQuaternion<T> dtkQuaternionFromHPR(T h, T p, T r)
{
    T  cosH, sinH, cosP, sinP, cosR, sinR;
    T  half_r, half_p, half_h;

    /* put angles into radians and divide by two, since all angles in formula
     *  are (angle/2)
     */

    half_h = dtkDeg2Rad(h / 2.0);
    half_p = dtkDeg2Rad(p / 2.0);
    half_r = dtkDeg2Rad(r / 2.0);

    cosH = cos(half_h);
    sinH = sin(half_h);

    cosP = cos(half_p);
    sinP = sin(half_p);

    cosR = cos(half_r);
    sinR = sin(half_r);

    return dtkDeprecated::dtkQuaternion<T>(
               sinR * cosP * cosH - cosR * sinP * sinH,
               cosR * sinP * cosH + sinR * cosP * sinH,
               cosR * cosP * sinH - sinR * sinP * cosH,
               cosR * cosP * cosH + sinR * sinP * sinH);
}

//! dtkEulerAxisFromQuaternion(const dtkQuaternion<T> &qtn)
/*!
 * This function returns Euler axis from a quaternion representing a rigid body's
 * attitude.
 */
template <class T> dtkDeprecated::dtkVector3D<T> dtkAxisFromQuaternion(const dtkDeprecated::dtkQuaternion<T>& qtn)
{
    dtkDeprecated::dtkVector3D<T> axis(qtn[0], qtn[1], qtn[2]);

    return axis.unit();
}

//! dtkQuaternionFromMatSquared(const dtkMatrixSquared<T> &mat)
/*!
 * This function returns the quaternion corresponding to a
 * transformation matrix.
*/
template <class T> dtkDeprecated::dtkQuaternion<T> dtkQuaternionFromMatSquared(const dtkDeprecated::dtkMatrixSquared<T>& mat)
{
    T sclrTmp = dtkDeprecated::dtkMatrixSquaredTrace(mat);

    dtkDeprecated::dtkQuaternion<T> qtn;

    if (sclrTmp > 0) {
        sclrTmp = 0.5 * sqrt(1 + sclrTmp);
        qtn[0] = 0.25 * (mat[1][2] - mat[2][1]) / sclrTmp;
        qtn[1] = 0.25 * (mat[2][0] - mat[0][2]) / sclrTmp;
        qtn[2] = 0.25 * (mat[0][1] - mat[1][0]) / sclrTmp;
        qtn[3] = sclrTmp;
    } else {
        // We could be in trouble, so we have to take
        // precautions. NOTE: Parts of this piece of code were taken
        // from the example in the article "Rotating Objects Using
        // Quaternions" in Game Developer Magazine written by Nick
        // Bobick, july 3, 1998, vol. 2, issue 26.
        int i = 0;

        if (mat[1][1] > mat[0][0]) i = 1;

        if (mat[2][2] > mat[i][i]) i = 2;

        switch (i) {
        case 0:
            sclrTmp = 0.5 * sqrt(1 + mat[0][0] - mat[1][1] - mat[2][2]);
            qtn[0] = sclrTmp;
            qtn[1] = 0.25 * (mat[0][1] + mat[1][0]) / sclrTmp;
            qtn[2] = 0.25 * (mat[0][2] + mat[2][0]) / sclrTmp;
            qtn[3] = 0.25 * (mat[1][2] - mat[2][1]) / sclrTmp;
            break;

        case 1:
            sclrTmp = 0.5 * sqrt(1 - mat[0][0] + mat[1][1] - mat[2][2]);
            qtn[0] = 0.25 * (mat[1][0] + mat[0][1]) / sclrTmp;
            qtn[1] = sclrTmp;
            qtn[2] = 0.25 * (mat[1][2] + mat[2][1]) / sclrTmp;
            qtn[3] = 0.25 * (mat[2][0] - mat[0][2]) / sclrTmp;
            break;

        case 2:
            sclrTmp = 0.5 * sqrt(1 - mat[0][0] - mat[1][1] + mat[2][2]);
            qtn[0] = 0.25 * (mat[2][0] + mat[0][2]) / sclrTmp;
            qtn[1] = 0.25 * (mat[2][1] + mat[1][2]) / sclrTmp;
            qtn[2] = sclrTmp;
            qtn[3] = 0.25 * (mat[0][1] - mat[1][0]) / sclrTmp;
            break;
        }
    }

    return qtn;
}

//! dtkEulerAngleFromQuaternion(const dtkQuaternion<T> &qtn)
/*!
 * This function returns Euler angle from a quaternion representing a rigid body's
 * attitude.
 */
template <class T> T dtkAngleFromQuaternion(const dtkDeprecated::dtkQuaternion<T>& qtn)
{
    return 2 * dtkRad2Deg(acos(qtn[3]));
}

//! dtkChangeOfBasis(dtkVector3D< dtkVector3D<T> >&from, dtkVector3D< dtkVector3D<T> >&to)
/*!
 * This is a "change of basis" from the "from" frame to the "to"
 * frame.  the "to" frame is typically the "Standard basis" frame.
 * The "from" is a frame that represents the current orientation
 * of the object in the shape of an orthonormal tripod.
*/
template <class T> dtkDeprecated::dtkMatrixSquared<T> dtkChangeOfBasis(dtkDeprecated::dtkVector3D< dtkDeprecated::dtkVector3D<T> >& from,
        dtkDeprecated::dtkVector3D< dtkDeprecated::dtkVector3D<T> >& to)
{
    dtkDeprecated::dtkMatrixSquared<T> A( 3 );
    enum { x , y , z };

    // _X,_Y,_Z is typically the standard basis frame.

    //  | x^_X , y^_X , z^_X |
    //  | x^_Y , y^_Y , z^_Y |
    //  | x^_Z , y^_Z , z^_Z |

    A[0][0] = from[x] * to[x];
    A[0][1] = from[y] * to[x];
    A[0][2] = from[z] * to[x];

    A[1][0] = from[x] * to[y];
    A[1][1] = from[y] * to[y];
    A[1][2] = from[z] * to[y];

    A[2][0] = from[x] * to[z];
    A[2][1] = from[y] * to[z];
    A[2][2] = from[z] * to[z];

    // This is simply the transpose done manually which would save
    // and inverse call.
    //  | x ^ _X , x ^ _Y , x ^ _Z |
    //  | y ^ _X , y ^ _Y , y ^ _Z |
    //  | z ^ _X , z ^ _Y , z ^ _Z |

    // And finally we return the matrix that takes the "from" frame
    // to the "to"/"Standard basis" frame.
    return A;
}

#endif

// /////////////////////////////////////////////////////////////////
// Credits
// /////////////////////////////////////////////////////////////////

// $Id: 8cffd0949bd17bc6f0a94d365a92c566464412e6 $

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
