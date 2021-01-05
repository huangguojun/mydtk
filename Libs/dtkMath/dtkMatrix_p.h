/* dtkMatrix_p.h ---
 *
 * Author: Julien Wintz
 * Created: Mon Jul 15 17:36:33 2013 (+0200)
 * Version:
 * Last-Updated: Mon Jul 22 11:47:07 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 15
 */

/* Change Log:
 *
 */

#pragma once

#include <QtCore>

template<typename T>
class dtkMatrixPrivate
{
public:
    dtkMatrixPrivate(void);
    dtkMatrixPrivate(qulonglong r, qulonglong c);
    virtual ~dtkMatrixPrivate(void);

public:
    T at(qulonglong i, qulonglong j);
    void setAt(qulonglong i, qulonglong j, T value);

public:
    void allocate(qulonglong r, qulonglong c);

public:
    T *buffer;

public:
    qulonglong r_count;
    qulonglong c_count;
};

#include "dtkMatrix.tpp"
