// Version: $Id: bebfafe7bc795c8598d0f7d161492fecc2b7587d $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkCore>

#include "dtkMatrix_p.h"

#include <dtkMathExport.h>

class DTKMATH_EXPORT dtkMatrix : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qulonglong colCount READ colCount)
    Q_PROPERTY(qulonglong rowCount READ rowCount)

public:
             dtkMatrix(void);
    virtual ~dtkMatrix(void);

public:
    virtual qulonglong colCount(void) const = 0;
    virtual qulonglong rowCount(void) const = 0;

public:
    virtual QVariant at(qulonglong i, qulonglong j) const = 0;
    virtual void setAt(qulonglong i, qulonglong j, QVariant value) = 0;

public:
    virtual void       cpy(const dtkMatrix& other) = 0;
    virtual dtkMatrix& cln(void) = 0;
    virtual dtkMatrix& sum(const dtkMatrix& other) = 0;
    virtual dtkMatrix& sub(const dtkMatrix& other) = 0;
    virtual dtkMatrix& mul(const dtkMatrix& other) = 0;

public:
    virtual void allocate(qulonglong r, qulonglong c) = 0;

public:
    virtual void *buffer(void) = 0;

public:
    dtkMatrix& operator =(const dtkMatrix& other);
    dtkMatrix& operator+=(const dtkMatrix& other);
    dtkMatrix& operator-=(const dtkMatrix& other);
    dtkMatrix& operator*=(const dtkMatrix& other);

protected:
    dtkMatrix& lazyClone(void);

public:
    friend DTKMATH_EXPORT dtkMatrix& operator+(const dtkMatrix& lhs, const dtkMatrix& rhs);
    friend DTKMATH_EXPORT dtkMatrix& operator-(const dtkMatrix& lhs, const dtkMatrix& rhs);
    friend DTKMATH_EXPORT dtkMatrix& operator*(const dtkMatrix& lhs, const dtkMatrix& rhs);

public:
    bool dirty;
};

DTKMATH_EXPORT dtkMatrix& operator+(const dtkMatrix& lhs, const dtkMatrix& rhs);
DTKMATH_EXPORT dtkMatrix& operator-(const dtkMatrix& lhs, const dtkMatrix& rhs);
DTKMATH_EXPORT dtkMatrix& operator*(const dtkMatrix& lhs, const dtkMatrix& rhs);

DTKMATH_EXPORT QDebug operator<<(QDebug debug, const dtkMatrix *matrix);
DTKMATH_EXPORT QDebug operator<<(QDebug debug, const dtkMatrix& matrix);

// DTK_DECLARE_OBJECT(dtkMatrix *)
// DTK_DECLARE_PLUGIN(dtkMatrix, DTKMATH_EXPORT)
// DTK_DECLARE_PLUGIN_FACTORY(dtkMatrix, DTKMATH_EXPORT)
// DTK_DECLARE_PLUGIN_MANAGER(dtkMatrix, DTKMATH_EXPORT)

//
// dtkMatrix.h ends here
