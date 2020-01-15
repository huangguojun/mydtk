// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkCoreMetaType.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename F>
inline QMetaObject::Connection dtkCoreParameter::connect(F slot)
{
    m_enable_share_connection = false;
    if (!m_connection) {
        m_connection = connection(new dtkCoreParameterConnection());
    }

    auto c = QObject::connect(m_connection.get(), &dtkCoreParameterConnection::valueChanged, slot);
    if (c) {
        m_connection->value_list << c;
        if (!m_connection->param_list.contains(this)) {
            m_connection->param_list << this;
        }
    }
    return c;
}

template <typename F>
inline QMetaObject::Connection dtkCoreParameter::connectFail(F slot)
{
    m_enable_share_connection = false;
    if (!m_connection) {
        m_connection = connection(new dtkCoreParameterConnection());
    }

    auto c = QObject::connect(m_connection.get(), &dtkCoreParameterConnection::invalidValue, slot);
    if (c) {
        m_connection->invalid_list << c;
    }
    return c;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename Derive>
inline dtkCoreParameterBase<Derive>::dtkCoreParameterBase(const QString& label, const QString& doc) : dtkCoreParameter(label, doc)
{

}

template <typename Derive>
inline dtkCoreParameterBase<Derive>::dtkCoreParameterBase(const dtkCoreParameterBase& o) : dtkCoreParameter(o)
{

}

template <typename Derive>
inline QVariant dtkCoreParameterBase<Derive>::variant(void) const
{
    bool old = this->m_enable_share_connection;
    if (!old)
        this->m_enable_share_connection = true;
    QVariant res = dtk::variantFromValue(static_cast<const Derive&>(*this));
    this->m_enable_share_connection = old;
    return res;
}

template <typename Derive>
inline void dtkCoreParameterBase<Derive>::copyAndShare(dtkCoreParameter *source)
{
    if (!source) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done";

    } else {
        if (this != source) {
            Derive *s = dynamic_cast<Derive *>(source);
            if (s) {
                Derive *self = static_cast<Derive *>(this);
                *self = *s;
                if (s->m_connection) {
                    self->m_connection = s->m_connection;
                    if (!self->m_connection->param_list.contains(self)) {
                        self->m_connection->param_list << self;
                    }
                } else {
                    dtkWarn() << Q_FUNC_INFO << "Input parameter has no connection. Only copy of values is done.";
                }

            } else {
                dtkWarn() << Q_FUNC_INFO << "Input parameter is not of same type than target one. Nothing is done";
            }
        }
    }
}

template <typename Derive>
inline void dtkCoreParameterBase<Derive>::copyAndShare(const QVariant& v)
{
    if (!v.canConvert<Derive>()) {
        dtkWarn() << Q_FUNC_INFO << "Input variant is not of type" << QMetaType::typeName(qMetaTypeId<Derive>()) << ". Nothing is done.";

    } else {
        Derive i = v.value<Derive>();
        Derive& self = static_cast<Derive&>(*this);
        self = i;
        if (i.m_connection) {
            self.m_connection = i.m_connection;
            if (!self.m_connection->param_list.contains(&self)) {
                self.m_connection->param_list << &self;
            }
        } else {
            dtkWarn() << Q_FUNC_INFO << "Input parameter has no connection. Only copy of values is done.";
        }
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameter simple version implementation
// ///////////////////////////////////////////////////////////////////

template <typename T, typename Enable>
inline dtkCoreParameterSimple<T, Enable>::dtkCoreParameterSimple(const dtkCoreParameter *p) : dtkCoreParameterBase<dtkCoreParameterSimple>()
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

template <typename T, typename Enable>
inline dtkCoreParameterSimple<T, Enable>::dtkCoreParameterSimple(const QVariant& v) : dtkCoreParameterBase<dtkCoreParameterSimple>()
{
    if (v.canConvert<dtkCoreParameterSimple>()) {
        auto o(v.value<dtkCoreParameterSimple>());
        *this = o;

    } else if (v.canConvert<T>()) {
        m_value = v.value<T>();
    }
}

template <typename T, typename Enable>
inline dtkCoreParameterSimple<T, Enable>::dtkCoreParameterSimple(const dtkCoreParameterSimple& o) : dtkCoreParameterBase<dtkCoreParameterSimple>(o), m_value(o.m_value)
{

}

template <typename T, typename Enable>
inline dtkCoreParameterSimple<T, Enable>::dtkCoreParameterSimple(const QString& label, const T& t, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterSimple>(label, doc), m_value(t)
{

}

template <typename T, typename Enable>
inline auto dtkCoreParameterSimple<T, Enable>::operator = (const T& t) -> dtkCoreParameterSimple&
{
    m_value = t;
    this->sync();
    return *this;
}

template <typename T, typename Enable>
inline auto dtkCoreParameterSimple<T, Enable>::operator = (const dtkCoreParameter *p) -> dtkCoreParameterSimple&
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

template <typename T, typename Enable>
inline auto dtkCoreParameterSimple<T, Enable>::operator = (const QVariant& v) -> dtkCoreParameterSimple&
{
    if (v.canConvert<dtkCoreParameterSimple<T, Enable>>()) {
        *this = v.value<dtkCoreParameterSimple<T, Enable>>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_value = map["value"].value<T>();

    } else if (v.canConvert<T>()) {
        m_value = v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename Enable>
inline auto dtkCoreParameterSimple<T, Enable>::operator = (const dtkCoreParameterSimple& o) -> dtkCoreParameterSimple&
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_value = o.m_value;
        this->sync();
    }
    return *this;
}

template <typename T, typename Enable>
inline dtkCoreParameterSimple<T, Enable>::operator T() const
{
    return m_value;
}

template <typename T, typename Enable>
inline void dtkCoreParameterSimple<T, Enable>::setValue(const T& t)
{
    if (m_value != t) {
        m_value = t;
        this->sync();
    }
}

template <typename T, typename Enable>
inline void dtkCoreParameterSimple<T, Enable>::setValue(const QVariant &v)
{
    if (v.canConvert<dtkCoreParameterSimple>()) {
        *this = v.value<dtkCoreParameterSimple>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_value = map["value"].value<T>();

    } else if (v.canConvert<T>()) {
        m_value = v.value<T>();

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterSimple<T, Enable>>())
                  << ". Nothing is done.";
        this->syncFail();
        return;
    }
    this->sync();
}


template <typename T, typename Enable>
inline QVariantHash dtkCoreParameterSimple<T, Enable>::toVariantHash(void)
{
    QVariantHash hash;
    hash.insert("label", m_label);
    hash.insert("doc", m_doc);
    hash.insert("value", QVariant::fromValue(m_value));

    return hash;
}

template <typename T, typename Enable>
inline T dtkCoreParameterSimple<T, Enable>::value(void) const
{
    return m_value;
}

template <typename T>
inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterSimple<T>& p)
{
    s << p.label();
    s << p.value();
    s << p.documentation();

    return s;
}

template <typename T>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterSimple<T>& p)
{
    QString label; s >> label;
    T t; s >> t;
    QString doc; s >> doc;

    p = dtkCoreParameterSimple<T>(label, t, doc);
    return s;
}

template <typename T>
inline QDebug operator << (QDebug dbg, dtkCoreParameterSimple<T> p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label : " << p.label() << ", "
                  << "value : " << p.value() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameterNumeric for numerical types implementation
// ///////////////////////////////////////////////////////////////////

template <typename T, typename E>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const T& t) : dtkCoreParameterBase<dtkCoreParameterNumeric>(), m_val(t)
{
}

template <typename T, typename E> template < typename U, typename V>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const U *p) : dtkCoreParameterBase<dtkCoreParameterNumeric>()
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

template <typename T, typename E>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const QVariant& v) : dtkCoreParameterBase<dtkCoreParameterNumeric>()
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this = v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<T>()) {
        m_val = v.value<T>();
    }
}

template <typename T, typename E>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const dtkCoreParameterNumeric& o) : dtkCoreParameterBase<dtkCoreParameterNumeric>(o), m_val(o.m_val), m_bounds(o.m_bounds), m_decimals(o.m_decimals)
{

}

template <typename T, typename E>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const QString& label, const T& t, const T& min, const T& max, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterNumeric>(label, doc), m_val(t), m_bounds({min, max})
{
}

template <typename T, typename E> template <typename U, typename>
inline dtkCoreParameterNumeric<T, E>::dtkCoreParameterNumeric(const QString& label, const T& t, const T& min, const T& max, const int& decimals, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterNumeric>(label, doc), m_val(t), m_bounds({min, max}), m_decimals(decimals)
{
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator = (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val = t;
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator = (const U *p) -> dtk::is_core_parameter<U, dtkCoreParameterNumeric&>
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator = (const QVariant& v) -> dtkCoreParameterNumeric&
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this = v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_val = map["value"].value<T>();
        if(map.contains("min")) m_bounds[0] = map["min"].value<T>();
        else                    m_bounds[0] = std::numeric_limits<T>::lowest();

        if(map.contains("max")) m_bounds[1] = map["max"].value<T>();
        else                    m_bounds[1] = std::numeric_limits<T>::max();

        if (map.contains("decimals")) {
            m_decimals = map["decimals"].value<int>();
        }

    } else if (v.canConvert<T>()) {
        T t = v.value<T>();

        if (( t < m_bounds[0] ) || ( t > m_bounds[1] )) {
            dtkWarn() << Q_FUNC_INFO << "Value (" << t << ") not setted because out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
            this->syncFail();
            return *this;

        } else if (t != m_val) {
            m_val = t;
        }

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterNumeric>())
                  << ". Nothing is done.";
        this->syncFail();
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator = (const dtkCoreParameterNumeric& o) -> dtkCoreParameterNumeric&
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_val = o.m_val;
        m_bounds = o.m_bounds;
        m_decimals = o.m_decimals;
        this->sync();
    }
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator = (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val = (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator += (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val += t;
    this->sync();
    return *this;
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator += (const QVariant& v) -> dtkCoreParameterNumeric&
{
    if (v.canConvert<dtkCoreParameterNumeric<T, E>>()) {
        *this += v.value<dtkCoreParameterNumeric<T, E>>();

    } else if (v.canConvert<T>()) {
        m_val += v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator += (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val += (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator -= (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val -= t;
    this->sync();
    return *this;
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator -= (const QVariant& v) -> dtkCoreParameterNumeric&
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this -= v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<T>()) {
        m_val -= v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator -= (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val -= (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator *= (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val *= t;
    this->sync();
    return *this;
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator *= (const QVariant& v) -> dtkCoreParameterNumeric&
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this *= v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<T>()) {
        m_val *= v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator *= (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val *= (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator /= (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val /= t;
    this->sync();
    return *this;
}

template <typename T, typename E>
inline auto dtkCoreParameterNumeric<T, E>::operator /= (const QVariant& v) -> dtkCoreParameterNumeric&
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this /= v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<T>()) {
        m_val /= v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator /= (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, dtkCoreParameterNumeric&>
{
    m_val /= (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator %= (const U& t) -> std::enable_if_t<std::numeric_limits<U>::is_modulo && std::numeric_limits<T>::is_modulo, dtkCoreParameterNumeric&>
{
    m_val %= t;
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator %= (const QVariant& v) -> std::enable_if_t<std::numeric_limits<U>::is_modulo, dtkCoreParameterNumeric&>
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this %= v.value<dtkCoreParameterNumeric>();

    } else if (v.canConvert<T>()) {
        m_val %= v.value<T>();

    } else {
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator %= (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::numeric_limits<U>::is_modulo && std::numeric_limits<T>::is_modulo, dtkCoreParameterNumeric&>
{
    m_val %= (T)(o);
    this->sync();
    return *this;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator == (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, bool>
{
    return (m_val == t);
}

template <typename T, typename E>
inline bool dtkCoreParameterNumeric<T, E>::operator == (const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        return (*this == v.value<dtkCoreParameterNumeric>());

    } else if (v.canConvert<T>()) {
        return (m_val == v.value<T>());
    }
    return false;
}

template <typename T, typename E> template <typename U>
inline auto  dtkCoreParameterNumeric<T, E>::operator == (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, bool>
{
    return (m_val == (T)(o));
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator != (const U& t) -> std::enable_if_t<std::is_arithmetic<U>::value, bool>
{
    return (m_val != t);
}

template <typename T, typename E>
inline bool dtkCoreParameterNumeric<T, E>::operator != (const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        return (*this != v.value<dtkCoreParameterNumeric>());

    } else if (v.canConvert<T>()) {
        return (m_val != v.value<T>());
    }
    return true;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::operator != (const dtkCoreParameterNumeric<U>& o) -> std::enable_if_t<std::is_arithmetic<U>::value, bool>
{
    return (m_val != (T)(o));
}

template <typename T, typename E>
inline dtkCoreParameterNumeric<T, E>::operator T() const
{
    return m_val;
}

template <typename T, typename E>
inline void dtkCoreParameterNumeric<T, E>::setValue(const T& t)
{
    if (( t < m_bounds[0] ) || ( t > m_bounds[1] )) {
        dtkWarn() << Q_FUNC_INFO << "Value (" << t << ") not setted because out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
        this->syncFail();

    } else if (t != m_val) {
        m_val = t;
        this->sync();
    }
}

template <typename T, typename E>
inline void dtkCoreParameterNumeric<T, E>::setValue(const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterNumeric>()) {
        *this = v.value<dtkCoreParameterNumeric>();
        // value is copied, do not check bounds

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_val = map["value"].value<T>();

        if(map.contains("min")) m_bounds[0] = map["min"].value<T>();
        else                    m_bounds[0] = std::numeric_limits<T>::lowest();

        if(map.contains("max")) m_bounds[1] = map["max"].value<T>();
        else                    m_bounds[1] = std::numeric_limits<T>::max();

        if (map.contains("decimals")) {
            m_decimals = map["decimals"].value<int>();
        }

    } else if (v.canConvert<T>()) {
        T t = v.value<T>();

        if (( t < m_bounds[0] ) || ( t > m_bounds[1] )) {
            dtkWarn() << Q_FUNC_INFO << "Value (" << t << ") not setted because out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
            this->syncFail();
            return;

        } else if (t != m_val) {
            m_val = t;
        }

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterNumeric>())
                  << ". Nothing is done.";
        this->syncFail();
        return;
    }
    this->sync();
}

template <typename T, typename E>
inline QVariantHash dtkCoreParameterNumeric<T, E>::toVariantHash(void)
{
    QVariantHash hash;
    hash.insert("label", m_label);
    hash.insert("doc", m_doc);
    hash.insert("value", m_val);
    hash.insert("min", m_bounds[0]);
    hash.insert("max", m_bounds[1]);
    hash.insert("decimals", m_decimals);

    return hash;
}


template <typename T, typename E>
inline T dtkCoreParameterNumeric<T, E>::value(void) const
{
    return m_val;
}

template <typename T, typename E>
inline T dtkCoreParameterNumeric<T, E>::min(void) const
{
    return this->m_bounds[0];
}

template <typename T, typename E>
inline T dtkCoreParameterNumeric<T, E>::max(void) const
{
    return this->m_bounds[1];
}

template <typename T, typename E>
inline const std::array<T, 2>& dtkCoreParameterNumeric<T, E>::bounds(void) const
{
    return this->m_bounds;
}

template <typename T, typename E> template <typename U>
inline auto dtkCoreParameterNumeric<T, E>::setDecimals(const int& d) -> std::enable_if_t<std::is_floating_point<U>::value>
{
    this->m_decimals = d;
}

template <typename T, typename E>
inline int dtkCoreParameterNumeric<T, E>::decimals(void) const
{
    return this->m_decimals;
}

template <typename T>
inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterNumeric<T>& p)
{
    s << p.label();
    s << p.value();
    s << p.min();
    s << p.max();
    s << p.decimals();
    s << p.documentation();

    return s;
}

template <typename T, typename E>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterNumeric<T>& p)
{
    QString label; s >> label;
    T val; s >> val;
    T min; s >> min;
    T max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    p = dtkCoreParameterNumeric<T>(label, val, min, max, dec, doc);
    return s;
}

template <typename T, typename E, typename U>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterNumeric<T>& p)
{
    QString label; s >> label;
    T val; s >> val;
    T min; s >> min;
    T max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    p = dtkCoreParameterNumeric<T>(label, val, min, max, doc);
    return s;
}

inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterNumeric<char>& p)
{
    s << p.label();
    s << (qint8)p.value();
    s << (qint8)p.min();
    s << (qint8)p.max();
    s << p.decimals();
    s << p.documentation();

    return s;
}

inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterNumeric<char>& p)
{
    QString label; s >> label;
    qint8 val; s >> val;
    qint8 min; s >> min;
    qint8 max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    p = dtkCoreParameterNumeric<char>(label, (char)val, (char)min, (char)max, doc);
    return s;
}

template <typename T>
inline QDebug operator << (QDebug dbg, dtkCoreParameterNumeric<T> p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label : " << p.label() << ", "
                  << "value : " << p.value() << ", "
                  << "bounds : [" << p.bounds()[0] << ", " << p.bounds()[1] << "], "
                  << "decimals : " << p.decimals() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameterInList implementation
// ///////////////////////////////////////////////////////////////////

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const T& t) : dtkCoreParameterBase<dtkCoreParameterInList>()
{
    m_values << t;
    m_value_index = 0;
}

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const QVariant& v) : dtkCoreParameterBase<dtkCoreParameterInList>()
{
    if (v.canConvert<dtkCoreParameterInList<T>>()) {
        auto o(v.value<dtkCoreParameterInList<T>>());
        *this = o;

    } else if (v.canConvert<T>()) {
        m_values << v.value<T>();
        m_value_index = 0;
    }
}

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const dtkCoreParameterInList& o) : dtkCoreParameterBase<dtkCoreParameterInList>(o), m_values(o.m_values), m_value_index(o.m_value_index)
{

}

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const QString& label, const T& t, const QList<T>& values, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterInList>(label, doc), m_values(values)
{
    Q_ASSERT_X(!m_values.empty(), Q_FUNC_INFO, "Input list cannot be empty");
    m_value_index = m_values.indexOf(t);
}

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const QString& label, int index, const QList<T>& values, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterInList>(label, doc), m_values(values), m_value_index(index)
{
    Q_ASSERT_X(!m_values.empty(), Q_FUNC_INFO, "Input list cannot be empty");
    Q_ASSERT_X(((index > -1) && (index < values.size())), Q_FUNC_INFO, "Input index is out of range");
}

template <typename T>
inline dtkCoreParameterInList<T>::dtkCoreParameterInList(const QString& label, const QList<T>& values, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterInList>(label, doc), m_values(values), m_value_index(0)
{
    Q_ASSERT_X(!m_values.empty(), Q_FUNC_INFO, "Input list cannot be empty");
}

template <typename T>
inline dtkCoreParameterInList<T>& dtkCoreParameterInList<T>::operator = (const T& t)
{
    int index = m_values.indexOf(t);
    if (index < 0) {
        dtkWarn() << Q_FUNC_INFO << "Value index (" << index << ") is out of range [" << 0 << "," << m_values.size()-1 << "]";
        this->syncFail();

    } else {
        m_value_index = index;
        this->sync();
    }
    return *this;
}

template <typename T>
inline dtkCoreParameterInList<T>& dtkCoreParameterInList<T>::operator = (const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterInList>()) {
        *this = v.value<dtkCoreParameterInList>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_value_index = map["index"].value<int>();

        m_values.clear();
        auto list = map["values"].toList();
        for (auto item : list) {
            m_values << item.value<T>();
        }

    } else if (v.canConvert<T>()) {
        int index = m_values.indexOf(v.value<T>());
        if (index < 0) {
            dtkWarn() << Q_FUNC_INFO << "Value is not part of the admissible ones.";
            this->syncFail();
            return *this;

        } else {
            m_value_index = index;
        }

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterInList>())
                  << ". Nothing is done.";
        this->syncFail();
        return *this;
    }
    this->sync();
    return *this;
}

template <typename T>
inline dtkCoreParameterInList<T>& dtkCoreParameterInList<T>::operator = (const dtkCoreParameterInList& o)
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_values = o.m_values;
        m_value_index = o.m_value_index;
        this->sync();
    }
    return *this;
}

template <typename T>
inline dtkCoreParameterInList<T>::operator T() const
{
    return m_values.value(m_value_index);
}

template <typename T>
inline void dtkCoreParameterInList<T>::setValueIndex(int index)
{
    if (index < 0 || index > m_values.size()-1) {
        dtkWarn() << Q_FUNC_INFO << "Value index (" << index << ") is out of range [" << 0 << "," << m_values.size()-1 << "]";
        this->syncFail();

    } else {
        m_value_index = index;
        this->sync();
    }
}

template <typename T>
inline void dtkCoreParameterInList<T>::setValue(const T& t)
{
    int index = m_values.indexOf(t);
    if (index < 0) {
        dtkWarn() << Q_FUNC_INFO << "Value index (" << index << ") is out of range [" << 0 << "," << m_values.size()-1 << "]";
        this->syncFail();

    } else {
        m_value_index = index;
        this->sync();
    }
}

template <typename T>
inline void dtkCoreParameterInList<T>::setValues(const QList<T>& l)
{
    m_values = l;
    m_value_index = 0;
    this->sync();
}

template <typename T>
inline void dtkCoreParameterInList<T>::setValue(const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterInList>()) {
        *this = v.value<dtkCoreParameterInList>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();
        m_value_index = map["index"].value<int>();

        m_values.clear();
        auto list = map["values"].toList();
        for(auto item : list) {
            m_values << item.value<T>();
        }

    } else if (v.canConvert<T>()) {
        int index = m_values.indexOf(v.value<T>());
        if (index < 0) {
            dtkWarn() << Q_FUNC_INFO << "Value is not part of the admissible ones.";
            this->syncFail();
            return;

        } else {
            m_value_index = index;
        }

    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterInList>())
                  << ". Nothing is done.";
        this->syncFail();
        return;
    }
    this->sync();
}


template <typename T>
inline QVariantHash dtkCoreParameterInList<T>::toVariantHash(void)
{
    QVariantHash hash;
    hash.insert("label", m_label);
    hash.insert("doc", m_doc);
    hash.insert("index", m_value_index);
    hash.insert("values",  QVariant::fromValue(m_values));

    return hash;
}


template <typename T>
inline int dtkCoreParameterInList<T>::valueIndex(void) const
{
    return m_value_index;
}

template <typename T>
inline T dtkCoreParameterInList<T>::value(void) const
{
    return m_values.value(m_value_index);
}

template <typename T>
inline QList<T> dtkCoreParameterInList<T>::values(void) const
{
    return m_values;
}

template <typename T>
inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterInList<T>& p)
{
    s << p.label();
    s << p.valueIndex();
    s << p.values().size();
    for (auto&& val : p.values()) {
        s << val;
    }
    s << p.documentation();

    return s;
}

template <typename T>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterInList<T>& p)
{
    QString label; s >> label;
    int index; s >> index;
    int size; s >> size;
    QList<T> values;
    for (int i = 0; i < size; ++i) {
        T t; s >> t;
        values << t;
    }
    QString doc; s >> doc;

    p = dtkCoreParameterInList<T>(label, index, values, doc);
    return s;
}

template <typename T>
inline QDebug operator << (QDebug dbg, dtkCoreParameterInList<T> p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label" << p.label() << ", "
                  << "value_index" << p.valueIndex() << ", "
                  << "values [";
    for (int i = 0; i < p.values().size(); ++i) {
        if (i)
            dbg.nospace() << ", ";
        dbg.nospace() << p.values().at(i);
    }
    dbg.nospace() << "], "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(const std::array<T, 2>& t) : dtkCoreParameterBase<dtkCoreParameterRange>()
{
    if (t[0] <= t[1]) {
        //qDebug() << Q_FUNC_INFO << t[0] << t[1] << m_bounds[0] << m_bounds[1];
        m_val = t;

    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values are not increasing order. Nothing is done.";
    }
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(std::initializer_list<T> args) : dtkCoreParameterBase<dtkCoreParameterRange>()
{
    if (args.size() == 2 && *(args.begin()) <= *(args.end())) {
        m_val[0] = *(args.begin());
        m_val[1] = *(args.end());
    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values are not increasing order. Nothing is done.";
    }
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(const QVariant& v) : dtkCoreParameterBase<dtkCoreParameterRange>()
{
    if (v.canConvert<dtkCoreParameterRange>()) {
        auto o(v.value<dtkCoreParameterRange>());
        *this = o;

    } else if (v.canConvert<std::array<T, 2>>()) {
        auto t = v.value<std::array<T, 2>>();
        if (t[0] <= t[1]) {
            m_val = t;
        } else {
            dtkWarn() << Q_FUNC_INFO << "Input values are not increasing order. Nothing is done.";
        }
    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterRange>())
                  << ". Nothing is done.";
    }
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(const dtkCoreParameterRange& o) : dtkCoreParameterBase<dtkCoreParameterRange>(o), m_val(o.m_val), m_bounds(o.m_bounds), m_decimals(o.m_decimals)
{

}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(const QString& label, const std::array<T, 2>& t, const T& min, const T& max, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterRange>(label, doc)
{
    if (min <= t[0] && t[0] <= t[1] && t[1] <= max) {
        m_val = t;
        m_bounds = {min , max};

    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values and bounds are not correctly ordered. Nothing is done.";
    }
}

template <typename T, typename E> template <typename U, typename>
inline dtkCoreParameterRange<T, E>::dtkCoreParameterRange(const QString& label, const std::array<T, 2>& t, const T& min, const T& max, const int& decimals, const QString& doc) : dtkCoreParameterBase<dtkCoreParameterRange>(label, doc)
{
    if (min <= t[0] && t[0] <= t[1] && t[1] <= max) {
        m_val = t;
        m_bounds = {min , max};
        m_decimals = decimals;

    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values and bounds are not correctly ordered. Nothing is done.";
    }
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>& dtkCoreParameterRange<T, E>::operator = (const std::array<T, 2>& t)
{
    if (m_bounds[0] <= t[0] && t[0] <= t[1] && t[1] <= m_bounds[1]) {
        m_val = t;
        this->sync();

    } else {
        dtkWarn() << Q_FUNC_INFO << "Values (" << t[0] << "," << t[1] << ") not setted because not correctly ordered or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
        this->syncFail();
    }
    return *this;
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>& dtkCoreParameterRange<T, E>::operator = (std::initializer_list<T> args)
{
    if (args.size() == 2 && m_bounds[0] <= *(args.begin()) && *(args.begin()) <= *(args.end()) && *(args.end()) <= m_bounds[1]) {
        m_val[0] = *(args.begin());
        m_val[1] = *(args.end());
        this->sync();

    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values not setted because they are not the right number of values or they are not correctly ordered or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
        this->syncFail();
    }
    return *this;
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>& dtkCoreParameterRange<T, E>::operator = (const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterRange>()) {
        auto o(v.value<dtkCoreParameterRange>());
        *this = o;

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();

        T min, max;
        if (map.contains("min")) min = map["min"].value<T>();
        else                     min = std::numeric_limits<T>::lowest();

        if (map.contains("max")) max = map["max"].value<T>();
        else                     max = std::numeric_limits<T>::max();

        auto list = map["values"].toList();
        auto v_min = list[0].value<T>();
        auto v_max = list[1].value<T>();

        if (min <= v_min && v_min <= v_max && v_max <= max) {
            m_bounds = {min, max};
            m_val = {v_min, v_max};

        } else {
            dtkWarn() << Q_FUNC_INFO << "Values and bouns in VariantHash are not correctly ordred. Nothing is done";
            this->syncFail();
            return *this;
        }

    } else if (v.canConvert<std::array<T, 2>>()) {
        auto t = v.value<std::array<T, 2>>();
         if (m_bounds[0] <= t[0] && t[0] <= t[1] && t[1] <= m_bounds[1]) {
            m_val = t;

         } else {
             dtkWarn() << Q_FUNC_INFO << "Values (" << t[0] << "," << t[1] << ") not setted because not correctly ordred or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
             this->syncFail();
             return *this;
         }
    }
    this->sync();
    return *this;
}

template <typename T, typename E>
inline dtkCoreParameterRange<T, E>& dtkCoreParameterRange<T, E>::operator = (const dtkCoreParameterRange& o)
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_val = o.m_val;
        m_bounds = o.m_bounds;
        m_decimals = o.m_decimals;
        this->sync();
    }
    return *this;
}

template <typename T, typename E>
inline void dtkCoreParameterRange<T, E>::setValue(const std::array<T, 2>& t)
{
    if (m_bounds[0] <= t[0] && t[0] <= t[1] && t[1] <= m_bounds[1]) {
        m_val = t;
        this->sync();

    } else {
        dtkWarn() << Q_FUNC_INFO << "Values (" << t[0] << "," << t[1] << ") not setted because not correctly ordered or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
        this->syncFail();
    }
}

template <typename T, typename E>
inline void dtkCoreParameterRange<T, E>::setValue(std::initializer_list<T> args)
{
    if ( ( args.size() == 2 ) && ( m_bounds[0] <= *(args.begin()) ) && ( *(args.begin()) <= *(args.end()) ) && ( *(args.end()) <= m_bounds[1]) ) {
        m_val[0] = *(args.begin());
        m_val[1] = *(args.end());
        this->sync();

    } else {
        dtkWarn() << Q_FUNC_INFO << "Input values not setted because they are are not the right number of values or not correctly ordered or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
        this->syncFail();
    }
}

template <typename T, typename E>
inline void dtkCoreParameterRange<T, E>::setValue(const QVariant& v)
{
    if (v.canConvert<dtkCoreParameterRange>()) {
        *this = v.value<dtkCoreParameterRange>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();

        T min, max;
        if (map.contains("min")) min = map["min"].value<T>();
        else                     min = std::numeric_limits<T>::lowest();

        if (map.contains("max")) max = map["max"].value<T>();
        else                     max = std::numeric_limits<T>::max();

        if (map.contains("decimals")) {
            m_decimals = map["decimals"].value<int>();
        }

        auto list = map["values"].toList();
        auto v_min = list[0].value<T>();
        auto v_max = list[1].value<T>();

        if (min <= v_min && v_min <= v_max && v_max <= max) {
            m_bounds = {min, max};
            m_val = {v_min, v_max};

        } else {
            dtkWarn() << Q_FUNC_INFO << "Values and bouns in VariantHash are not correctly ordred. Nothing is done";
            this->syncFail();
            return;
        }

    } else if (v.canConvert<std::array<T, 2>>()) {
        auto t = v.value<std::array<T, 2>>();
         if (m_bounds[0] <= t[0] && t[0] <= t[1] && t[1] <= m_bounds[1]) {
            m_val = t;

         } else {
             dtkWarn() << Q_FUNC_INFO << "Values (" << t[0] << "," << t[1] << ") not setted because not correctly ordred or out of bounds [" << m_bounds[0] << "," << m_bounds[1] << "]";
             this->syncFail();
             return;
         }
    }
    this->sync();
}

template <typename T, typename E>
inline QVariantHash dtkCoreParameterRange<T, E>::toVariantHash(void)
{
    QVariantHash hash;
    hash.insert("label", m_label);
    hash.insert("doc", m_doc);
    hash.insert("values", QVariant::fromValue(m_val));
    hash.insert("min", m_bounds[0]);
    hash.insert("max", m_bounds[1]);
    hash.insert("decimals", m_decimals);

    return hash;
}

template <typename T, typename E>
inline const std::array<T, 2>& dtkCoreParameterRange<T, E>::value(void) const
{
    return m_val;
}

template <typename T, typename E>
inline T dtkCoreParameterRange<T, E>::min(void) const
{
    return m_bounds[0];
}

template <typename T, typename E>
inline T dtkCoreParameterRange<T, E>::max(void) const
{
    return m_bounds[1];
}

template <typename T, typename E>
inline const std::array<T, 2>& dtkCoreParameterRange<T, E>::bounds(void) const
{
    return m_bounds;
}

template <typename T, typename E>
template <typename U>
inline std::enable_if_t<std::is_floating_point<U>::value> dtkCoreParameterRange<T, E>::setDecimals(const int& decimals)
{
    m_decimals = decimals;
}

template <typename T, typename E>
inline int dtkCoreParameterRange<T, E>::decimals(void) const
{
    return m_decimals;
}

template <typename T>
inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterRange<T>& p)
{
    s << p.label();
    s << p.value()[0];
    s << p.value()[1];
    s << p.min();
    s << p.max();
    s << p.decimals();
    s << p.documentation();

    return s;
}

template <typename T, typename E>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterRange<T>& p)
{
    QString label; s >> label;
    T v_min; s >> v_min;
    T v_max; s >> v_max;
    T min; s >> min;
    T max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    std::array<T, 2> val = {v_min, v_max};

    p = dtkCoreParameterRange<T>(label, val, min, max, dec, doc);
    return s;
}

template <typename T, typename E, typename U>
inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterRange<T>& p)
{
    QString label; s >> label;
    T v_min; s >> v_min;
    T v_max; s >> v_max;
    T min; s >> min;
    T max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    std::array<T, 2> val = {v_min, v_max};

    p = dtkCoreParameterRange<T>(label, val, min, max, doc);
    return s;
}

inline QDataStream& operator << (QDataStream& s, const dtkCoreParameterRange<char>& p)
{
    s << p.label();
    s << (qint8)p.value()[0];
    s << (qint8)p.value()[1];
    s << (qint8)p.min();
    s << (qint8)p.max();
    s << p.decimals();
    s << p.documentation();

    return s;
}

inline QDataStream& operator >> (QDataStream& s, dtkCoreParameterRange<char>& p)
{
    QString label; s >> label;
    qint8 v_min; s >> v_min;
    qint8 v_max; s >> v_max;
    qint8 min; s >> min;
    qint8 max; s >> max;
    int dec; s >> dec;
    QString doc; s >> doc;

    std::array<char, 2> val = {(char)v_min, (char)v_max};

    p = dtkCoreParameterRange<char>(label, val, (char)min, (char)max, doc);
    return s;
}

template <typename T>
inline QDebug operator << (QDebug dbg, dtkCoreParameterRange<T> p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "values ["
                  << p.value()[0] << ","
                  << p.value()[1] << "], "
                  << "bounds ["
                  << p.min() << ","
                  << p.max() << "], "
                  << "decimals" << p.decimals() << ", "
                  << "documentation : " << p.documentation()
                  << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

//
// dtkCoreParameter.tpp ends here
