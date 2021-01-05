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

#pragma once

#include <dtkCoreExport>

#include "dtkCoreParameter.h"

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameterPath declaration
// ///////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCoreParameterPath : public dtkCoreParameterBase<dtkCoreParameterPath>
{
public:
    dtkCoreParameterPath(void) = default;
    ~dtkCoreParameterPath(void) = default;

    dtkCoreParameterPath(const dtkCoreParameter *);
    dtkCoreParameterPath(const QVariant &);
    dtkCoreParameterPath(const dtkCoreParameterPath &);

    dtkCoreParameterPath(const QString &label, const QString &path, const QStringList &filters,
                         const QString & = QString());

    dtkCoreParameterPath &operator=(const dtkCoreParameter *);
    dtkCoreParameterPath &operator=(const QVariant &);
    dtkCoreParameterPath &operator=(const dtkCoreParameterPath &);

    void setValue(const QString &);
    void setValue(const QVariant &) override;
    QVariantHash toVariantHash(void) override;

    QString path(void) const;
    QStringList filters(void) const;

    QString dirName(void) const;
    QString baseName(void) const;

private:
    using dtkCoreParameter::m_doc;
    using dtkCoreParameter::m_label;

    QString m_path;
    QStringList m_filters;
};

// ///////////////////////////////////////////////////////////////////

DTKCORE_EXPORT QDataStream &operator<<(QDataStream &, const dtkCoreParameterPath &);
DTKCORE_EXPORT QDataStream &operator>>(QDataStream &, dtkCoreParameterPath &);

DTKCORE_EXPORT QDebug operator<<(QDebug, dtkCoreParameterPath);

// ///////////////////////////////////////////////////////////////////
// Alias and QMetaType
// ///////////////////////////////////////////////////////////////////

namespace dtk {

using d_path = dtkCoreParameterPath;
}

Q_DECLARE_METATYPE(dtk::d_path);
Q_DECLARE_METATYPE(dtk::d_path *);

//
// dtkCoreParameterPath.h ends here
