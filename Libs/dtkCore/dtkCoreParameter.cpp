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

#include "dtkCoreParameter.h"
#include "dtkCoreParameterPath.h"

#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// helper functions
// ///////////////////////////////////////////////////////////////////

namespace dtk {
namespace core {

void registerParameters(void)
{
    /*
    qRegisterMetaType<dtk::d_uchar>();
    qRegisterMetaType<dtk::d_char>();
    qRegisterMetaType<dtk::d_uint>();
    qRegisterMetaType<dtk::d_int>();
    qRegisterMetaType<dtk::d_real>();
    qRegisterMetaType<dtk::d_bool>();
    qRegisterMetaType<dtk::d_string>();

    qRegisterMetaType<dtk::d_uchar*>();
    qRegisterMetaType<dtk::d_char*>();
    qRegisterMetaType<dtk::d_uint*>();
    qRegisterMetaType<dtk::d_int*>();
    qRegisterMetaType<dtk::d_real*>();
    qRegisterMetaType<dtk::d_bool*>();
    qRegisterMetaType<dtk::d_string*>();

    qRegisterMetaTypeStreamOperators<dtk::d_uchar>("dtk::d_uchar");
    qRegisterMetaTypeStreamOperators<dtk::d_char>("dtk::d_char");
    qRegisterMetaTypeStreamOperators<dtk::d_uint>("dtk::d_uint");
    qRegisterMetaTypeStreamOperators<dtk::d_int>("dtk::d_int");
    qRegisterMetaTypeStreamOperators<dtk::d_real>("dtk::d_real");
    qRegisterMetaTypeStreamOperators<dtk::d_bool>("dtk::d_bool");
    qRegisterMetaTypeStreamOperators<dtk::d_string>("dtk::d_string");

    QMetaType::registerDebugStreamOperator<dtk::d_uchar>();
    QMetaType::registerDebugStreamOperator<dtk::d_char>();
    QMetaType::registerDebugStreamOperator<dtk::d_uint>();
    QMetaType::registerDebugStreamOperator<dtk::d_int>();
    QMetaType::registerDebugStreamOperator<dtk::d_real>();
    QMetaType::registerDebugStreamOperator<dtk::d_bool>();
    QMetaType::registerDebugStreamOperator<dtk::d_string>();


    qRegisterMetaType<dtk::d_inliststring>();
    qRegisterMetaType<dtk::d_inliststring*>();
    qRegisterMetaType<dtk::d_inlistreal>();
    qRegisterMetaType<dtk::d_inlistreal*>();
    qRegisterMetaType<dtk::d_inlistint>();
    qRegisterMetaType<dtk::d_inlistint*>();

    qRegisterMetaTypeStreamOperators<dtk::d_inliststring>("dtk::d_inliststring");
    qRegisterMetaTypeStreamOperators<dtk::d_inlistreal>("dtk::d_inlistreal");
    qRegisterMetaTypeStreamOperators<dtk::d_inlistint>("dtk::d_inlistint");

    QMetaType::registerDebugStreamOperator<dtk::d_inliststring>();
    QMetaType::registerDebugStreamOperator<dtk::d_inlistreal>();
    QMetaType::registerDebugStreamOperator<dtk::d_inlistint>();


    qRegisterMetaType<dtk::d_range_uchar>();
    qRegisterMetaType<dtk::d_range_uchar*>();
    qRegisterMetaType<dtk::d_range_uchar::range>();
    qRegisterMetaType<dtk::d_range_char>();
    qRegisterMetaType<dtk::d_range_char*>();
    qRegisterMetaType<dtk::d_range_char::range>();
    qRegisterMetaType<dtk::d_range_uint>();
    qRegisterMetaType<dtk::d_range_uint*>();
    qRegisterMetaType<dtk::d_range_uint::range>();
    qRegisterMetaType<dtk::d_range_int>();
    qRegisterMetaType<dtk::d_range_int*>();
    qRegisterMetaType<dtk::d_range_int::range>();
    qRegisterMetaType<dtk::d_range_real>();
    qRegisterMetaType<dtk::d_range_real*>();
    qRegisterMetaType<dtk::d_range_real::range>();

    qRegisterMetaTypeStreamOperators<dtk::d_range_uchar>("dtk::d_range_uchar");
    qRegisterMetaTypeStreamOperators<dtk::d_range_char>("dtk::d_range_char");
    qRegisterMetaTypeStreamOperators<dtk::d_range_uint>("dtk::d_range_uint");
    qRegisterMetaTypeStreamOperators<dtk::d_range_int>("dtk::d_range_int");
    qRegisterMetaTypeStreamOperators<dtk::d_range_real>("dtk::d_range_real");

    QMetaType::registerDebugStreamOperator<dtk::d_range_uchar>();
    QMetaType::registerDebugStreamOperator<dtk::d_range_char>();
    QMetaType::registerDebugStreamOperator<dtk::d_range_uint>();
    QMetaType::registerDebugStreamOperator<dtk::d_range_int>();
    QMetaType::registerDebugStreamOperator<dtk::d_range_real>();


    qRegisterMetaType<dtk::d_path>();
    qRegisterMetaType<dtk::d_path*>();

    qRegisterMetaTypeStreamOperators<dtk::d_path>("dtk::d_path");

    QMetaType::registerDebugStreamOperator<dtk::d_path>();
    */
}

dtkCoreParameters readParameters(const QString &filename)
{
    /*
    QFile definition_file(filename);

    dtkCoreParameters parameters;
    dtkCoreParameters dummy;

    if(!definition_file.open(QFile::ReadOnly)) {
        dtkWarn() << Q_FUNC_INFO << "input file" << filename << "cannot be
opened in ReadOnly mode."; return dummy;
    }

    QJsonParseError definition_error;
    QJsonDocument definition_document =
QJsonDocument::fromJson(definition_file.readAll(), &definition_error);

    if(definition_error.error != QJsonParseError::NoError) {
        dtkWarn() << Q_FUNC_INFO << "Error :" << definition_error.errorString()
<< "parsing" << filename << "offset :" << definition_error.offset << "."; return
dummy;
    }

    QJsonObject definition_object = definition_document.object();
    if(!definition_object.contains("contents")) {
        dtkWarn() << Q_FUNC_INFO << "The nature parameters file should contain a
first json object with key 'contents'. The file cannot be processed any
further."; return dummy;
    }

    QJsonValue definition_contents = definition_object["contents"];
    QJsonObject definitions;
    if (definition_contents.isObject()) {
        definitions = definition_contents.toObject();
    } else {
        dtkWarn() << Q_FUNC_INFO << "The first object with key 'contents' must
be an object.";
    }

    QStringList keys = definitions.keys();
    for (auto it = definitions.begin(); it != definitions.end(); ++it) {
        QString name = keys.takeFirst();
        if (it->isObject()) {
            QJsonObject content_object = it->toObject();

            QString type = content_object["type"].toString();

            QVariantHash map = content_object.toVariantHash();
            //map.insert("type", type);

            dtkCoreParameter *parameter = dtkCoreParameter::create(map);
            if (!parameter) {
                dtkWarn() << Q_FUNC_INFO << "fail to create parameter" << type
<< map; return dummy;
            }
            parameters.insert(name, parameter);

        } else {
            dtkWarn() << Q_FUNC_INFO << "'contents' sections are expected to
contain objects only. Non object entry is ignored.";
        }
    }
    return parameters;
}*/
}
} // namespace core

// ///////////////////////////////////////////////////////////////////
// dtkCoreParameter implementation
// ///////////////////////////////////////////////////////////////////

dtkCoreParameter::dtkCoreParameter(const QString &label, const QString &doc)
    : m_label(label), m_doc(doc)
{
}

dtkCoreParameter::dtkCoreParameter(const dtkCoreParameter &o) : m_label(o.m_label), m_doc(o.m_doc)
{
    if (o.m_enable_share_connection) {
        m_connection = o.m_connection;
    }
}

void dtkCoreParameter::setLabel(const QString &label)
{
    this->m_label = label;
}

QString dtkCoreParameter::label(void) const
{
    return this->m_label;
}

void dtkCoreParameter::setDocumentation(const QString &doc)
{
    this->m_doc = doc;
}

QString dtkCoreParameter::documentation(void) const
{
    return this->m_doc;
}

void dtkCoreParameter::block(bool b)
{
    if (m_connection)
        m_connection->blockSignals(b);
}

void dtkCoreParameter::sync(void)
{
    if (m_connection) {
        emit m_connection->valueChanged(this->variant());
    }
}

void dtkCoreParameter::disconnect(void)
{
    if (m_connection) {
        if (m_connection.use_count() > 1) {
            m_connection->param_list.removeAll(this);
            m_connection = connection(nullptr);
        } else {
            for (auto c : m_connection->value_list) {
                if (c) {
                    m_connection->disconnect(c);
                }
            }
            m_connection->value_list.clear();
        }
    }
}

void dtkCoreParameter::syncFail(void)
{
    if (m_connection)
        emit m_connection->invalidValue();
}

void dtkCoreParameter::disconnectFail(void)
{
    if (m_connection) {
        if (m_connection.use_count() > 1) {
            m_connection = connection(nullptr);
        } else {
            for (auto c : m_connection->invalid_list) {
                if (c) {
                    m_connection->disconnect(c);
                }
            }
            m_connection->invalid_list.clear();
        }
    }
}

void dtkCoreParameter::shareValue(QVariant v)
{
    for (dtkCoreParameter *p : this->m_connection->param_list) {
        p->setValue(v);
    }
}

bool dtkCoreParameter::shareConnectionWith(dtkCoreParameter *source)
{
    m_enable_share_connection = false;
    if (!source || !source->m_connection) {
        dtkWarn() << Q_FUNC_INFO << "Input parameter has no connection. Nothing is done.";
        return false;

    } else {
        if (this != source) {
            if (m_connection != source->m_connection) {
                m_connection = source->m_connection;
            }
            return true;
        }
        return true;
    }
}

dtkCoreParameter *dtkCoreParameter::create(const QVariantHash &map)
{
    if (map.empty()) {
        qDebug() << Q_FUNC_INFO
                 << "Input variant does not contain a VariantHash. Nullptr is "
                    "returned.";
        return nullptr;
    }

    QString p_type_name = map["type"].toString();
    int p_type_id = QMetaType::type(qPrintable(p_type_name));

    if (p_type_id == QMetaType::UnknownType) {
        qDebug() << Q_FUNC_INFO << "Cannot create parameter of type" << p_type_name
                 << ". Nullptr is returned.";
        return nullptr;
    }

    void *ptr = QMetaType::create(p_type_id);
    if (!ptr) {
        qDebug() << Q_FUNC_INFO << "Parameter of type" << p_type_name
                 << "cannot be created. Nullptr is returned.";
        return nullptr;
    }
    p_type_id = QMetaType::type(qPrintable(p_type_name + "*"));
    if (p_type_id == QMetaType::UnknownType) {
        qDebug() << Q_FUNC_INFO << "Cannot embed pointer to a parameter of type" << p_type_name
                 << "into a QVariant. Please check that the type has been "
                    "registered at runtime to QMetaType system. Nullptr is "
                    "returned.";
        return nullptr;
    }
    QVariant pv = QVariant(p_type_id, &ptr, 1);
    dtkCoreParameter *p = pv.value<dtkCoreParameter *>();

    if (!p) {
        qDebug() << Q_FUNC_INFO
                 << "Cannot convert QVariant into dtkCoreParameter. Nullptr is "
                    "returned.";
        return nullptr;
    }

    p->setValue(QVariant(map));
    return p;
}

void dtkCoreParameter::setAdvanced(bool adv)
{
    m_advanced = adv;
    emit advancedChanged(m_advanced);
}

bool dtkCoreParameter::advanced(void)
{
    return m_advanced;
}

//
// dtkCoreParameter.cpp ends here
