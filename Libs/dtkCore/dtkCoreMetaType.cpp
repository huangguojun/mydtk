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
#include "dtkCoreMetaContainerSequential.h"

#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// dtkCoreMetaType function implementations
// ///////////////////////////////////////////////////////////////////

namespace dtk {
// Description of the content of a QVariant
QString description(const QVariant &v)
{
    QString str;
    QDebug dbg(&str);
    dbg.setAutoInsertSpaces(false);

    if (v.canConvert<dtkCoreMetaContainerSequential>()) {
        dtkCoreMetaContainerSequential mc = v.value<dtkCoreMetaContainerSequential>();
        dbg << mc;
    } else {
        const uint typeId = v.userType();
        bool userStream = false;
        bool canConvertToString = false;

        if (typeId >= QMetaType::User) {
            userStream = QMetaType::debugStream(dbg, v.constData(), typeId);
            canConvertToString = v.canConvert<QString>();
        }

        if (!userStream && canConvertToString) {
            dbg << v.toString();

        } else if (typeId == QMetaType::QVariantMap || typeId == QMetaType::QVariantHash) {
            QAssociativeIterable iterable = v.value<QAssociativeIterable>();
            QAssociativeIterable::const_iterator it = iterable.begin();
            const QAssociativeIterable::const_iterator end = iterable.end();

            for (; it != end; ++it) {
                dbg << dtk::description(it.key()) << ": " << dtk::description(it.value());
                dbg << '\n';
            }

        } else if (!userStream) {
            dbg << qSetRealNumberPrecision(15) << v;
            QString cleanStr = str.trimmed();
            cleanStr.chop(1);
            // remove "QVariant(typename, " from the string:
            int count = 11 + QString(v.typeName()).size();
            str = cleanStr.remove(0, count);
        }
    }

    return str;
}

// Clones the content of a QVariant and stores it into another QVariant
QVariant cloneContent(const QVariant &v)
{
    if (v.canConvert<dtkCoreMetaContainerSequential>()) {

        QString type_name = v.typeName();

        if (type_name.endsWith("*")) {
            type_name.chop(1);
        }

        int c_id = QMetaType::type(qPrintable(type_name));

        if (c_id != QMetaType::UnknownType) {
            const void *c = *static_cast<const void *const *>(v.data());
            void *ptr = QMetaType::create(c_id, c);

            c_id = QMetaType::type(qPrintable(type_name + "*"));
            return QVariant(c_id, &ptr, 1);
        } else {
            dtkTrace() << Q_FUNC_INFO << "Type" << type_name
                       << "has not be registered to QMetaType System.";
        }

    } else {

        int type_id = v.userType();
        QMetaType::TypeFlags flags = QMetaType(type_id).flags();

        if (int(flags & QMetaType::PointerToQObject)) {

            const QObject *o = *static_cast<const QObject *const *>(v.data());

            if (o) {
                const QMetaObject *mo = o->metaObject();
                QString class_name(mo->className());
                int class_type = QMetaType::type(qPrintable(class_name));

                while (class_type == QMetaType::UnknownType) {
                    mo = mo->superClass();

                    if (!mo)
                        break;

                    class_name = mo->className();
                    class_type = QMetaType::type(qPrintable(class_name));
                }

                if (class_type != QMetaType::UnknownType) {
                    void *ptr = QMetaType::create(class_type, o);

                    class_type = QMetaType::type(qPrintable(class_name + "*"));
                    return QVariant(class_type, &ptr, 1);
                } else {
                    class_name = o->metaObject()->className();
                    dtkTrace() << Q_FUNC_INFO << "Type" << class_name
                               << "has not be registered to QMetaType System. If they "
                                  "exist, parent classes are neither registered.";
                }
            }

        } else if (type_id < QMetaType::User) { // built-in type

            return v;

        } else {

            QString type_name = v.typeName();

            if (!type_name.endsWith("*")) { // Not a pointer
                return v;
            }

            type_name.chop(1);
            int type_id = QMetaType::type(qPrintable(type_name));

            if (type_id != QMetaType::UnknownType) {
                const void *data = *static_cast<const void *const *>(v.data());
                void *ptr = QMetaType::create(type_id, data);

                type_id = QMetaType::type(qPrintable(type_name + "*"));
                return QVariant(type_id, &ptr, 1);
            } else {
                type_name = v.typeName();
                type_id = QMetaType::type(qPrintable(type_name));

                if (type_id != QMetaType::UnknownType) {
                    void *ptr = nullptr;
                    return QVariant(type_id, &ptr, 1);
                } else {
                    dtkTrace() << Q_FUNC_INFO << ": Type" << type_name
                               << "has not be registered to QMetaType System. "
                                  "Maybe it is not registered or maybe it is a "
                                  "non copyable class.";
                }
            }
        }
    }

    return QVariant();
}

// Creates an empty container into a QVariant. The container is of
// the same type than the container contained in the input QVaraint.
QVariant createEmptyContainer(const QVariant &v)
{
    if (v.canConvert<dtkCoreMetaContainerSequential>()) {

        QString type_name = v.typeName();

        if (type_name.endsWith("*")) {
            type_name.chop(1);
        }

        int c_id = QMetaType::type(qPrintable(type_name));

        if (c_id != QMetaType::UnknownType) {
            void *ptr = QMetaType::create(c_id);
            c_id = QMetaType::type(qPrintable(type_name + "*"));
            QVariant res = QVariant(c_id, &ptr, 1);

            if (res.canConvert<dtkCoreMetaContainerSequential>()) {
                return res;
            } else {
                dtkTrace() << Q_FUNC_INFO << "Type" << res.typeName()
                           << "is not compatible with dtkMetaContainer.";
            }

        } else {
            dtkTrace() << Q_FUNC_INFO << "Type" << type_name
                       << "has not be registered to QMetaType System.";
        }
    }
    return QVariant();
}

// Destroys the pointer contained in the input QVariant
bool destroyPointer(QVariant &v)
{
    QString type_name = QString(v.typeName());
    bool ok = false;

    if (type_name.endsWith("*")) {
        void *ptr = *static_cast<void **>(v.data());

        if (ptr) {
            type_name.chop(1);
            int type_id = QMetaType::type(qPrintable(type_name));
            QMetaType::destroy(type_id, ptr);
            v.clear();
            ok = true;
        }
    } else {
        dtkTrace() << Q_FUNC_INFO << "Type" << type_name << "is not of pointer type.";
    }

    return ok;
}

} // namespace dtk

//
// dtkCoreMetaType.cpp ends here
