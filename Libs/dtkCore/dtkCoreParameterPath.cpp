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

#include "dtkCoreParameterPath.h"

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameterPath implementation
// ///////////////////////////////////////////////////////////////////

dtkCoreParameterPath::dtkCoreParameterPath(const dtkCoreParameter *p)
    : dtkCoreParameterBase<dtkCoreParameterPath>()
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return;
    }
    *this = p->variant();
}

dtkCoreParameterPath::dtkCoreParameterPath(const QVariant &v)
    : dtkCoreParameterBase<dtkCoreParameterPath>()
{
    if (v.canConvert<dtkCoreParameterPath>()) {
        auto o(v.value<dtkCoreParameterPath>());
        *this = o;

    } else if (v.canConvert<QString>()) {
        m_path = v.toString();
    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterPath>())
                  << ". Nothing is done.";
    }
}

dtkCoreParameterPath::dtkCoreParameterPath(const dtkCoreParameterPath &o)
    : dtkCoreParameterBase<dtkCoreParameterPath>(o), m_path(o.m_path), m_filters(o.m_filters)
{
}

dtkCoreParameterPath::dtkCoreParameterPath(const QString &label, const QString &path,
                                           const QStringList &filters, const QString &doc)
    : dtkCoreParameterBase<dtkCoreParameterPath>(label, doc), m_path(path), m_filters(filters)
{
}

dtkCoreParameterPath &dtkCoreParameterPath::operator=(const dtkCoreParameter *p)
{
    if (!p) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter is null. Nothing is done.";
        return *this;
    }
    return *this = p->variant();
}

dtkCoreParameterPath &dtkCoreParameterPath::operator=(const QVariant &v)
{
    if (v.canConvert<dtkCoreParameterPath>()) {
        *this = v.value<dtkCoreParameterPath>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();

        m_path = map["path"].toString();
        m_filters = map["filters"].toStringList();

    } else if (v.canConvert<QString>()) {
        m_path = v.toString();
    } else {
        return *this;
    }
    this->sync();
    return *this;
}

dtkCoreParameterPath &dtkCoreParameterPath::operator=(const dtkCoreParameterPath &o)
{
    if (this != &o) {
        m_label = o.m_label;
        m_doc = o.m_doc;
        m_path = o.m_path;
        m_filters = o.m_filters;
        this->sync();
    }
    return *this;
}

void dtkCoreParameterPath::setValue(const QString &path)
{
    m_path = path;
    this->sync();
}

void dtkCoreParameterPath::setValue(const QVariant &v)
{
    if (v.canConvert<dtkCoreParameterPath>()) {
        *this = v.value<dtkCoreParameterPath>();

    } else if (v.canConvert<QVariantHash>()) {
        auto map = v.toHash();

        m_label = map["label"].toString();
        m_doc = map["doc"].toString();

        m_path = map["path"].toString();
        m_filters = map["filters"].toStringList();

    } else if (v.canConvert<QString>()) {
        m_path = v.toString();
    } else {
        dtkWarn() << Q_FUNC_INFO << "QVariant type" << v.typeName()
                  << "is not compatible with current type"
                  << QMetaType::typeName(qMetaTypeId<dtkCoreParameterPath>())
                  << ". Nothing is done.";
        this->syncFail();
        return;
    }
    this->sync();
}

QVariantHash dtkCoreParameterPath::toVariantHash(void)
{
    QVariantHash hash;
    hash.insert("label", m_label);
    hash.insert("doc", m_doc);
    hash.insert("filters", m_filters);
    hash.insert("path", m_path);

    return hash;
}

QString dtkCoreParameterPath::path(void) const
{
    return m_path;
}

QStringList dtkCoreParameterPath::filters(void) const
{
    return m_filters;
}

QString dtkCoreParameterPath::dirName(void) const
{
    QFileInfo pathinfo = QFileInfo(m_path);
    return pathinfo.dir().path();
}

QString dtkCoreParameterPath::baseName(void) const
{
    QFileInfo pathinfo = QFileInfo(m_path);
    return pathinfo.baseName();
}

QDataStream &operator<<(QDataStream &s, const dtkCoreParameterPath &p)
{
    s << p.label();
    s << p.path();
    auto filters = p.filters();
    s << filters.size();
    for (int i = 0; i < filters.size(); ++i) {
        s << filters[i];
    }
    s << p.documentation();

    return s;
}

QDataStream &operator>>(QDataStream &s, dtkCoreParameterPath &p)
{
    QString label;
    s >> label;
    QString path;
    s >> path;
    int count;
    s >> count;
    QStringList filters;
    for (int i = 0; i < count; ++i) {
        QString filter;
        s >> filter;
        filters << filter;
    }
    QString doc;
    s >> doc;

    p = dtkCoreParameterPath(label, path, filters, doc);
    return s;
}

QDebug operator<<(QDebug dbg, dtkCoreParameterPath p)
{
    const bool old_setting = dbg.autoInsertSpaces();
    dbg.nospace() << p.variant().typeName() << " : { ";
    dbg.nospace() << "label " << p.label() << ", "
                  << "path " << p.path() << ", "
                  << "filters " << p.filters() << ", "
                  << "documentation : " << p.documentation() << " }";

    dbg.setAutoInsertSpaces(old_setting);
    return dbg.maybeSpace();
}

//
// dtkCoreParameterPath.cpp ends here
