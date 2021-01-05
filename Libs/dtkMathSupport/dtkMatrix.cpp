/* dtkMatrix.cpp ---
 *
 * Author: tkloczko
 * Copyright (C) 2011 - Thibaud Kloczko, Inria.
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkMatrix.h"

// /////////////////////////////////////////////////////////////////
// Partial specializations for atomic types
// /////////////////////////////////////////////////////////////////

template<>
QByteArray *dtkDeprecated::dtkMatrix<double>::serialize(void)
{
    unsigned i_end = m_crow * m_ccol;

    if (i_end <= 0)
        return NULL;

    QByteArray *array = new QByteArray;
    QDataStream stream(array, QIODevice::WriteOnly);
    stream << m_crow;
    stream << m_ccol;

    for (unsigned i = 0; i < i_end; ++i)
        stream << (*m_rgrow)[i];

    return array;
};

template<>
void dtkDeprecated::dtkMatrix<double>::deserialize(const QByteArray &array)
{
    QDataStream stream(array);

    stream >> m_crow;
    stream >> m_ccol;

    unsigned i_end = m_crow * m_ccol;

    if (i_end <= 0)
        return;

    this->allocate(m_crow, m_ccol);

    for (unsigned i = 0; i < i_end; ++i)
        stream >> (*m_rgrow)[i];
};
