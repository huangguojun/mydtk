/* dtkMatrix.cpp ---
 *
 * Author: Julien Wintz
 * Created: Mon Jul 15 16:04:38 2013 (+0200)
 * Version:
 * Last-Updated: Mon Jul 22 12:14:44 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 414
 */

/* Change Log:
 *
 */

#include "dtkMatrix.h"

dtkMatrix::dtkMatrix(void)
{
    this->dirty = false;
}

dtkMatrix::~dtkMatrix(void)
{

}

dtkMatrix& dtkMatrix::operator=(const dtkMatrix& other)
{
    this->cpy(other);

    if (other.dirty)
        delete const_cast<dtkMatrix *>(&other);

    return (*this);
}

dtkMatrix& dtkMatrix::operator+=(const dtkMatrix& other)
{
    return sum(other);
}

dtkMatrix& dtkMatrix::operator-=(const dtkMatrix& other)
{
    return sub(other);
}

dtkMatrix& dtkMatrix::operator*=(const dtkMatrix& other)
{
    return mul(other);
}

dtkMatrix& dtkMatrix::lazyClone(void)
{
    if (this->dirty)
        return *this;

    return this->cln();
}

dtkMatrix& operator+(const dtkMatrix& lhs, const dtkMatrix& rhs)
{
    dtkMatrix& r = const_cast<dtkMatrix&>(lhs).lazyClone();
    r.dirty = true;
    r.sum(rhs);

    if (rhs.dirty)
        delete const_cast<dtkMatrix *>(&rhs);

    return r;
}

dtkMatrix& operator-(const dtkMatrix& lhs, const dtkMatrix& rhs)
{
    dtkMatrix& r = const_cast<dtkMatrix&>(lhs).lazyClone();
    r.dirty = true;
    r.sub(rhs);

    if (rhs.dirty)
        delete const_cast<dtkMatrix *>(&rhs);

    return r;
}

dtkMatrix& operator*(const dtkMatrix& lhs, const dtkMatrix& rhs)
{
    dtkMatrix& r = const_cast<dtkMatrix&>(lhs).lazyClone();
    r.dirty = true;
    r.mul(rhs);

    if (rhs.dirty)
        delete const_cast<dtkMatrix *>(&rhs);

    return r;
}

QDebug operator<<(QDebug debug, const dtkMatrix *matrix)
{
    debug << *matrix;

    return debug;
}

QDebug operator<<(QDebug debug, const dtkMatrix& matrix)
{
    debug.nospace() << "\n";

    for (qulonglong i = 0; i < matrix.rowCount(); i++) {
        debug.nospace() << "(";

        for (qulonglong j = 0; j < matrix.colCount(); j++) {
            debug.nospace() << " " << matrix.at(i, j);
        }

        debug.nospace() << ")" << "\n";
    }

    return debug.space();
}
