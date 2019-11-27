/* dtkMath.cpp ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 * Created: Tue Jul  6 16:57:24 2010 (+0200)
 * Version: $Id: 5592571cad3c6aa43855a2645f07db2a6234c47c $
 * Last-Updated: jeu. mars 26 11:19:47 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 266
 */

/* Commentary:
 *
 * fabs - ok with doubles as well
 *
 * Comparison between float and between double is derived from the
 * work of Bruce Dawson available here:
 *
 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
 *
 */

/* Change log:
 *
 */

#include <dtkConfig.h>
#include "dtkMath.h"
#include "dtkVector3D.h"

#if defined(Q_OS_WIN)
// on vs 9 abs() does not work with long long int or __uint64
#define abs qAbs
#endif

//! dtkBubbleSort(unsigned int indices[], int size)
/*!
 * Sorts the integer of an array by growing values.
 */
void dtkBubbleSort(unsigned int indices[], int size)
{
    bool ordered = false;
    int j = 0;

    while ( (j < size - 1) && (!ordered) ) {
        ordered = true;

        for (int k = 0; k < (size - 1 - j); k++) {
            if (indices[k] > indices[k + 1]) {
                unsigned int temp = indices[k];
                indices[k] = indices[k + 1];
                indices[k + 1] = temp;
                ordered = false;
            }
        }

        j++;
    }
}

//! Sorts the integer of an array by growing values.
/*!
 *
 */
void dtkBubbleSort(qint32 *indices, qint32 size)
{
    bool ordered = false;
    qint32 j = 0;

    while ( (j < size - 1) && (!ordered) ) {
        ordered = true;

        for (qint32 k = 0; k < (size - 1 - j); k++) {
            if (indices[k] > indices[k + 1]) {
                qint32 temp = indices[k];
                indices[k] = indices[k + 1];
                indices[k + 1] = temp;
                ordered = false;
            }
        }

        j++;
    }
}

//! Sorts the integer of an array by growing values.
/*!
 *
 */
void dtkBubbleSort(qint64 *indices, qint64 size)
{
    bool ordered = false;
    qint64 j = 0;

    while ( (j < size - 1) && (!ordered) ) {
        ordered = true;

        for (qint64 k = 0; k < (size - 1 - j); k++) {
            if (indices[k] > indices[k + 1]) {
                qint64 temp = indices[k];
                indices[k] = indices[k + 1];
                indices[k + 1] = temp;
                ordered = false;
            }
        }

        j++;
    }
}

//! dtkMinMaxValueOfArray(const qint32* array, const qint32& size, qint32* minValue, qint32* maxValue)
/*!
 * Gives the minimum and maximum integer of an array.
 */
void dtkMinMaxValueOfArray(const qint32 *array, const qint32& size, qint32 *minValue, qint32 *maxValue)
{
    if (!minValue || !maxValue)
        return;

    if (size < 1) {
        *minValue = 0;
        *maxValue = 0;
        return;
    }

    qint32 minimum = array[0];
    qint32 maximum = minimum;

    for (qint32 i = 1; i < size; i++) {
        if (array[i] < minimum)
            minimum = array[i];

        if (array[i] > maximum)
            maximum = array[i];
    }

    *minValue = minimum;
    *maxValue = maximum;
}

//! Gives the minimum and maximum integer of an array.
/*!
 *
 */
void dtkMinMaxValueOfArray(const qint64 *array, const qint64& size, qint64 *minValue, qint64 *maxValue)
{
    if (!minValue || !maxValue)
        return;

    if (size < 1) {
        *minValue = 0;
        *maxValue = 0;
        return;
    }

    qint64 minimum = array[0];
    qint64 maximum = minimum;

    for (qint64 i = 1; i < size; i++) {
        if (array[i] < minimum)
            minimum = array[i];

        if (array[i] > maximum)
            maximum = array[i];
    }

    *minValue = minimum;
    *maxValue = maximum;
}

//! Gives the minimum and maximum real of an array.
/*!
 *
 */
void dtkMinMaxValueOfArray(const qreal *array, const qlonglong& size, qreal *minValue, qreal *maxValue)
{
    if (!minValue || !maxValue)
        return;

    if (size < 1) {
        *minValue = 0;
        *maxValue = 0;
        return;
    }

    qreal minimum = array[0];
    qreal maximum = minimum;

    for (qlonglong i = 1; i < size; i++) {
        if (array[i] < minimum)
            minimum = array[i];

        if (array[i] > maximum)
            maximum = array[i];
    }

    *minValue = minimum;
    *maxValue = maximum;
}

//! Non-optimal AlmostEqual function - not recommended.
/*!
 *
 */
bool dtkAlmostEqualRelative(float A, float B, float maxRelativeError)
{
    if (A == B)
        return true;

    float relativeError = fabs((A - B) / B);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

bool dtkAlmostEqualRelative(double A, double B, double maxRelativeError)
{
    if (A == B)
        return true;

    double relativeError = fabs((A - B) / B);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

//! Slightly better AlmostEqual function but still not recommended
/*!
 *
 */
bool dtkAlmostEqualRelative2(float A, float B, float maxRelativeError)
{
    if (A == B)
        return true;

    float relativeError;

    if (fabs(B) > fabs(A))
        relativeError = fabs((A - B) / B);
    else
        relativeError = fabs((A - B) / A);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

bool dtkAlmostEqualRelative2(double A, double B, double maxRelativeError)
{
    if (A == B)
        return true;

    double relativeError;

    if (fabs(B) > fabs(A))
        relativeError = fabs((A - B) / B);
    else
        relativeError = fabs((A - B) / A);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

//! Slightly better AlmostEqual function and still not recommended
/*!
 *
 */
bool dtkAlmostEqualRelativeOrAbsolute(float A, float B, float maxRelativeError, float maxAbsoluteError)
{
    if (fabs(A - B) < maxAbsoluteError)
        return true;

    float relativeError;

    if (fabs(B) > fabs(A))
        relativeError = fabs((A - B) / B);
    else
        relativeError = fabs((A - B) / A);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

bool dtkAlmostEqualRelativeOrAbsolute(double A, double B, double maxRelativeError, double maxAbsoluteError)
{
    if (fabs(A - B) < maxAbsoluteError)
        return true;

    double relativeError;

    if (fabs(B) > fabs(A))
        relativeError = fabs((A - B) / B);
    else
        relativeError = fabs((A - B) / A);

    if (relativeError <= maxRelativeError)
        return true;

    return false;
}

//! Initial AlmostEqualULPs version.
/*!
 *  Fast and simple, but some limitations.
 */
bool dtkAlmostEqualUlpsSimple(float A, float B, qint32 maxUlps)
{
    if (A == B)
        return true;

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 *BB = reinterpret_cast<qint32 *>(&B);

    qint32 intDiff = abs(*AA) - abs(*BB);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

bool dtkAlmostEqualUlpsSimple(double A, double B, qint64 maxUlps)
{
    if (A == B)
        return true;

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 *BB = reinterpret_cast<qint64 *>(&B);

    qint64 intDiff = labs(*AA - *BB);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

//! Usable AlmostEqual function.
/*!
 *
 */
bool dtkAlmostEqual2sComplement(float A, float B, qint32 maxUlps)
{
    if (maxUlps < 0 || maxUlps > 4 * 1024 * 1024)
        return false;

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 *BB = reinterpret_cast<qint32 *>(&B);

    qint32 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    qint32 intDiff = abs(aInt - bInt);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

bool dtkAlmostEqual2sComplement(double A, double B, qint64 maxUlps)
{
    if (maxUlps < 0 || maxUlps > 8 * 1024 * 1024)
        return false;

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 *BB = reinterpret_cast<qint64 *>(&B);

    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = labs(aInt - bInt);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

//! Compares two numbers using mixed absolute and relative comparison.
/*! The absolute tolerance test fails when a and b become large, and the
 *  relative tolerance test fails when they become small.
 *  This function combines these two tests together in a single test.
 */
bool dtkAlmostEqualMixedAbsoluteRelative(const double A, const double B,
        const double tolerance)
{
    double diff = fabs(A - B);

    double largest = qMax(fabs(A), fabs(B));

    if (diff <= tolerance * qMax(1., largest))
        return 1;

    return 0;
}

// /////////////////////////////////////////////////////////////////
// Support functions and conditional compilation directives for the
// master AlmostEqual function.
// ////////////////////////////////////////////////////////////////

#define INFINITYCHECK
#define NANCHECK
#define SIGNCHECK

//! Is \a A infinite.
/*!
 *  An infinity has an exponent of 255 (shift left 23 positions) and a
 *  zero mantissa. There are two infinities - positive and negative.
 */
bool dtkIsInfinite(float A)
{
    const qint32 kInfAsInt = EXP_255_BIT_32;

    qint32 *AA = reinterpret_cast<qint32 *>(&A);

    if ((qint32)((*AA & (HIGH_BIT_32 - ONE_BIT_32))) == kInfAsInt)
        return true;

    return false;
}

bool dtkIsInfinite(double A)
{
    const qint64 kInfAsInt = EXP_255_BIT_64;

    qint64 *AA = reinterpret_cast<qint64 *>(&A);

    if ((qint64)((*AA & (HIGH_BIT_64 - ONE_BIT_64))) == kInfAsInt)
        return true;

    return false;
}

//! Is \a A NAN.
/*!
 *  A NAN has an exponent of 255 (shifted left 23 positions) and a
 *  non-zero mantissa.
 */
bool dtkIsNan(float A)
{
    qint32 *AA = reinterpret_cast<qint32 *>(&A);

    qint32 exp = *AA & EXP_255_BIT_32;
    qint32 mantissa = *AA & ((1u << 23) - 1u); //0x007FFFFF

    if (exp == EXP_255_BIT_32 && mantissa != 0)
        return true;

    return false;
}

bool dtkIsNan(double A)
{
    qint64 *AA = reinterpret_cast<qint64 *>(&A);

    qint64 exp = *AA & EXP_255_BIT_64;
    qint64 mantissa = *AA & ((1ull << 55) - 1ull); //0x007FFFFFFFFFFFFF

    if (exp == EXP_255_BIT_64 && mantissa != 0)
        return true;

    return false;
}

//! Returns the sign of \a A.
/*!
 *  The sign bit of a number is the high bit.
 */
qint32 dtkSign(float A)
{
    qint32 *AA = reinterpret_cast<qint32 *>(&A);

    return (*AA) & HIGH_BIT_32;
}

qint64 dtkSign(double A)
{
    qint64 *AA = reinterpret_cast<qint64 *>(&A);

    return (*AA) & HIGH_BIT_64;
}

//! Final version of the AlmostEqualUlps function.
/*!
 *  The optional checks are included for completeness, but in many
 *  cases they are not necessary, or even not desirable.
 *
 *  If A or B are infinity (positive or negative) then only return true
 *  if they are exactly equal to each other - that is, if they are both
 *  infinities of the same sign. This check is only needed if you will
 *  be generating infinities and you don't want them 'close' to numbers
 *  near FLT_MAX.
 *
 *  If A or B are a NAN, return false. NANs are equal to nothing, not
 *  even themselves. This check is only needed if you will be
 *  generating NANs and you use a maxUlps greater than 4 million or
 *  you want to ensure that a NAN does not equal itself.
 *
 *  After adjusting floats so their representations are
 *  lexicographically ordered as twos-complement integers a very small
 *  positive number will compare as 'close' to a very small negative
 *  number. If this is not desireable, and if you are on a platform
 *  that supports subnormals (which is the only place the problem can
 *  show up) then you need this check. The check for A == B is because
 *  zero and negative zero have different signs but are equal to each
 *  other.
 *
 */
bool dtkAlmostEqualUlps(float A, float B, qint32 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A == B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A == B;

#endif

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 aInt = *AA;

    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    // Make bInt lexicographically ordered as a twos-complement int
    qint32 *BB = reinterpret_cast<qint32 *>(&B);
    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    // Now we can compare aInt and bInt to find out how far apart A and B
    // are.
    qint32 intDiff = abs(aInt - bInt);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

bool dtkAlmostEqualUlps(double A, double B, qint64 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A == B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A == B;

#endif

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 *BB = reinterpret_cast<qint64 *>(&B);
    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = labs(aInt - bInt);

    if (intDiff <= maxUlps)
        return true;

    return false;
}

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

//! Prints a number and its representation, in hex and decimal
/*!
 *
 */
void dtkPrintNumber(float A, qint32 offset)
{
    qint32 *AA = reinterpret_cast<qint32 *>(&A);

    (*AA) += offset;

#if !defined(__APPLE__)
    printf("%+1.8g, 0x%08x, %d\n", A, *AA, *AA);
#else
    printf("%+1.8g, %d, %d\n", A, *AA, *AA);
#endif
}

void dtkPrintNumber(double A, qint64 offset)
{
    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    (*AA) += offset;

#if !defined(__APPLE__)
    printf("%+1.8g, 0x%016llx, %lld\n", A, *AA, *AA);
#else
    printf("%+1.8g, %lld, %lld\n", A, *AA, *AA);
#endif
}

// /////////////////////////////////////////////////////////////////
// Additional functions for further comparisons
// /////////////////////////////////////////////////////////////////

//! Returns true when \a A and \B are far apart and A smaller than B.
/*!
 *
 */
bool dtkLesserThanUlps(float A, float B, qint32 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A < B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A < B;

#endif

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    qint32 *BB = reinterpret_cast<qint32 *>(&B);
    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    qint32 intDiff = aInt - bInt;

    if (abs(intDiff) <= maxUlps)
        return false;

    else if (intDiff > 0)
        return false;

    else
        return true;
}

bool dtkLesserThanUlps(double A, double B, qint64 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A < B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A < B;

#endif

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 *BB = reinterpret_cast<qint64 *>(&B);
    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = aInt - bInt;

    if (labs(intDiff) <= maxUlps)
        return false;

    else if (intDiff > 0)
        return false;

    else
        return true;
}

//! Returns true when \a A and \B are not too far apart or A smaller than B.
/*!
 *
 */
bool dtkLesserThanOrAlmostEqualUlps(float A, float B, qint32 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A <= B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A <= B;

#endif

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    qint32 *BB = reinterpret_cast<qint32 *>(&B);
    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    qint32 intDiff = aInt - bInt;

    if (abs(intDiff) <= maxUlps)
        return true;

    else if (intDiff > 0)
        return false;

    else
        return true;
}

bool dtkLesserThanOrAlmostEqualUlps(double A, double B, qint64 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A <= B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A <= B;

#endif

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 *BB = reinterpret_cast<qint64 *>(&B);
    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = aInt - bInt;

    if (labs(intDiff) <= maxUlps)
        return true;

    else if (intDiff > 0)
        return false;

    else
        return true;
}

//! Returns true when \a A and \B are far apart and A greater than B.
/*!
 *
 */
bool dtkGreaterThanUlps(float A,  float B, qint32 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A > B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A > B;

#endif

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    qint32 *BB = reinterpret_cast<qint32 *>(&B);
    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    qint32 intDiff = aInt - bInt;

    if (abs(intDiff) <= maxUlps)
        return false;

    else if (intDiff < 0)
        return false;

    else
        return true;
}

bool dtkGreaterThanUlps(double A, double B, qint64 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A > B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A > B;

#endif

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 *BB = reinterpret_cast<qint64 *>(&B);
    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = aInt - bInt;

    if (labs(intDiff) <= maxUlps)
        return false;

    else if (intDiff < 0)
        return false;

    else
        return true;
}

//! Returns true when \a A and \B are not too far apart or A greater than B.
/*!
 *
 */
bool dtkGreaterThanOrAlmostEqualUlps(float A,  float B, qint32 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A >= B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A >= B;

#endif

    qint32 *AA = reinterpret_cast<qint32 *>(&A);
    qint32 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_32 - aInt;

    qint32 *BB = reinterpret_cast<qint32 *>(&B);
    qint32 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_32 - bInt;

    qint32 intDiff = aInt - bInt;

    if (abs(intDiff) <= maxUlps)
        return true;

    else if (intDiff < 0)
        return false;

    else
        return true;
}

bool dtkGreaterThanOrAlmostEqualUlps(double A, double B, qint64 maxUlps)
{
#ifdef  INFINITYCHECK

    if (dtkIsInfinite(A) || dtkIsInfinite(B))
        return A >= B;

#endif

#ifdef  NANCHECK

    if (dtkIsNan(A) || dtkIsNan(B))
        return false;

#endif

#ifdef  SIGNCHECK

    if (dtkSign(A) != dtkSign(B))
        return A >= B;

#endif

    qint64 *AA = reinterpret_cast<qint64 *>(&A);
    qint64 aInt = *AA;

    if (aInt < 0)
        aInt = HIGH_BIT_64 - aInt;

    qint64 *BB = reinterpret_cast<qint64 *>(&B);
    qint64 bInt = *BB;

    if (bInt < 0)
        bInt = HIGH_BIT_64 - bInt;

    qint64 intDiff = aInt - bInt;

    if (labs(intDiff) <= maxUlps)
        return true;

    else if (intDiff < 0)
        return false;

    else
        return true;
}

namespace dtkDeprecated {

    template <> long double norm_func(const dtkVector<long double>& val)
    {
        long double elemResult = dtkZero<long double>();

        elemResult = (val) * (val);

        return sqrt( elemResult );
    }

    template <> inline float norm_func(const dtkVector<float>& val)
    {
        float elemResult = dtkZero<float>();

        elemResult = (val) * (val);

        return sqrt( elemResult );
    }

}
