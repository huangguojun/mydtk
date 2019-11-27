/* dtkMath.h ---
 *
 * Author: Thibaud Kloczko
 * Copyright (C) 2008 - Thibaud Kloczko, Inria.
 * Created: Tue Jun  8 15:37:50 2010 (+0200)
 * Version: $Id: e0ff35cf24b92629f6a2c81c60547c6ae35a62be $
 * Last-Updated: jeu. févr. 19 08:43:47 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 201
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKMATH_H
#define DTKMATH_H

#include <dtkMathSupportExport.h>

#include <QtCore/QtGlobal>

// /////////////////////////////////////////////////////////////////
// Sorting algorithm
// /////////////////////////////////////////////////////////////////

DTKMATHSUPPORT_EXPORT void dtkBubbleSort(unsigned int indices[], int size);
DTKMATHSUPPORT_EXPORT void dtkBubbleSort(qint32 *indices, qint32 size);
DTKMATHSUPPORT_EXPORT void dtkBubbleSort(qint64 *indices, qint64 size);

// /////////////////////////////////////////////////////////////////
// Minimum and maximum value of an array
// /////////////////////////////////////////////////////////////////

DTKMATHSUPPORT_EXPORT void dtkMinMaxValueOfArray(const qint32 *array, const qint32& size, qint32 *minValue, qint32 *maxValue);
DTKMATHSUPPORT_EXPORT void dtkMinMaxValueOfArray(const qint64 *array, const qint64& size, qint64 *minValue, qint64 *maxValue);
DTKMATHSUPPORT_EXPORT void dtkMinMaxValueOfArray(const qreal *array, const qlonglong& size, qreal *minValue, qreal *maxValue);

// /////////////////////////////////////////////////////////////////
// Some bit constants
// /////////////////////////////////////////////////////////////////

#define ONE_BIT_32 (1u)
#define HIGH_BIT_32 ((1u)<<31)
#define EXP_255_BIT_32 (255u<<23)

#define ONE_BIT_64 (1ull)
#define HIGH_BIT_64 ((1ull)<<63)
#define EXP_255_BIT_64 (255ull<<55)

// /////////////////////////////////////////////////////////////////
// Float and double comparisons
// /////////////////////////////////////////////////////////////////

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelative( float A,  float B,  float maxRelativeError);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelative(double A, double B, double maxRelativeError);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelative2( float A,  float B,  float maxRelativeError);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelative2(double A, double B, double maxRelativeError);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelativeOrAbsolute( float A,  float B,  float maxRelativeError,  float maxAbsoluteError);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualRelativeOrAbsolute(double A, double B, double maxRelativeError, double maxAbsoluteError);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualUlpsSimple( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualUlpsSimple(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqual2sComplement( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqual2sComplement(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualMixedAbsoluteRelative(const double A, const double B, const double tolerance);

DTKMATHSUPPORT_EXPORT bool dtkIsInfinite( float A);
DTKMATHSUPPORT_EXPORT bool dtkIsInfinite(double A);

DTKMATHSUPPORT_EXPORT bool dtkIsNan( float A);
DTKMATHSUPPORT_EXPORT bool dtkIsNan(double A);

DTKMATHSUPPORT_EXPORT qint32 dtkSign( float A);
DTKMATHSUPPORT_EXPORT qint64 dtkSign(double A);

DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualUlps( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkAlmostEqualUlps(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT void dtkPrintNumber( float f, qint32 offset);
DTKMATHSUPPORT_EXPORT void dtkPrintNumber(double f, qint64 offset);

DTKMATHSUPPORT_EXPORT bool dtkLesserThanUlps( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkLesserThanUlps(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT bool dtkLesserThanOrAlmostEqualUlps( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkLesserThanOrAlmostEqualUlps(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT bool dtkGreaterThanUlps( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkGreaterThanUlps(double A, double B, qint64 maxUlps);

DTKMATHSUPPORT_EXPORT bool dtkGreaterThanOrAlmostEqualUlps( float A,  float B, qint32 maxUlps);
DTKMATHSUPPORT_EXPORT bool dtkGreaterThanOrAlmostEqualUlps(double A, double B, qint64 maxUlps);

#include "dtkMath.tpp"

#endif
