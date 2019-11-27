/* dtkVariant.tpp ---
 *
 * Author: Thibaud Kloczko, Inria.
 * Created: 2012 Fri Oct 26 09:04:46 (+0200)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKVARIANT_TPP
#define DTKVARIANT_TPP

// /////////////////////////////////////////////////////////////////
// dtkVariant specific implementation declarations
// /////////////////////////////////////////////////////////////////

template<> DTKCORE_EXPORT qlonglong *dtkVariant::value(void) const;

// /////////////////////////////////////////////////////////////////
// dtkVariant implementation
// /////////////////////////////////////////////////////////////////

inline dtkVariant::dtkVariant(void) : QVariant(), m_object(0), m_matrix(0), m_container(0), m_value_b(false), m_value_i(0), m_value_r(0), m_value_s(QString())
{

};

template<typename T> void dtkVariant::setValue(const T& value)
{
    dtkVariantPrivate<T, dtkTypeInfo<T>::dtkObjectPointer>::setValue(*this, value);
};

template<typename T> T dtkVariant::value(void) const
{
    return dtkVariantPrivate<T, dtkTypeInfo<T>::dtkObjectPointer>::value(*this);
};

#endif
