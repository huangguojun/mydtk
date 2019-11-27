/* dtkVariant.cpp ---
 *
 * Author: Thibaud Kloczko, Inria.
 * Created: 2012 Fri Oct 26 09:16:21 (+0200)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkVariant.h"

#include "dtkAbstractObject.h"

#include <dtkMath>

#include <dtkContainer/dtkAbstractContainerWrapper.h>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkVariant implementation
// /////////////////////////////////////////////////////////////////

dtkVariant::dtkVariant(const dtkVariant& other) : QVariant(other), m_object(other.m_object), m_matrix(other.m_matrix), m_container(other.m_container), m_value_b(other.m_value_b), m_value_i(other.m_value_i), m_value_r(other.m_value_r), m_value_s(other.m_value_s)
{

}

dtkVariant::~dtkVariant(void)
{

}

QString dtkVariant::identifier(void) const
{
    if (m_object)
        return m_object->identifier();

    if (m_matrix)
        return m_matrix->identifier();

    return this->typeName();
}

QString dtkVariant::description(void) const
{
    if (m_object)
        return m_object->description();

    if (m_matrix)
        return m_matrix->description();

    return this->value<QString>();
}


void dtkVariant::clear(void)
{
    this->QVariant::clear();
    m_object = NULL;
    m_matrix = NULL;
    m_container = NULL;
}

template<> qlonglong *dtkVariant::value(void) const
{

}
