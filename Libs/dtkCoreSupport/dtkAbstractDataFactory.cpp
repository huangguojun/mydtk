/* dtkAbstractDataFactory.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 15:54:10 2008 (+0100)
 * Version: $Id: 5ab31e4489e1cab00424397d2902d0be83a566e1 $
 * Last-Updated: lun. févr.  3 14:09:46 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 292
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractDataFactory.h"
#include "dtkAbstractData.h"
#include "dtkAbstractDataConverter.h"
#include "dtkAbstractDataDeserializer.h"
#include "dtkAbstractDataReader.h"
#include "dtkAbstractDataSerializer.h"
#include "dtkAbstractDataWriter.h"
#include "dtkSmartPointer.h"

// /////////////////////////////////////////////////////////////////
// Anonymous namespace declarations
// /////////////////////////////////////////////////////////////////

namespace {

typedef QHash<QString, QList<dtkAbstractData *>> dtkAbstractDataHash;

struct dtkAbstractDataReaderInfo
{
    dtkAbstractDataReaderInfo() : creator(NULL) {}
    dtkAbstractDataReaderInfo(const QStringList &_handled,
                              dtkAbstractDataFactory::dtkAbstractDataReaderCreator _creator)
        : handled(_handled), creator(_creator)
    {
    }
    QStringList handled;
    dtkAbstractDataFactory::dtkAbstractDataReaderCreator creator;
};

struct dtkAbstractDataWriterInfo
{
    dtkAbstractDataWriterInfo() : creator(NULL) {}
    dtkAbstractDataWriterInfo(const QStringList &_handled,
                              dtkAbstractDataFactory::dtkAbstractDataWriterCreator _creator)
        : handled(_handled), creator(_creator)
    {
    }
    QStringList handled;
    dtkAbstractDataFactory::dtkAbstractDataWriterCreator creator;
};

struct dtkAbstractDataConverterInfo
{
    dtkAbstractDataConverterInfo() : creator(NULL) {}
    dtkAbstractDataConverterInfo(const QStringList &_fromTypes, const QString &_toType,
                                 dtkAbstractDataFactory::dtkAbstractDataConverterCreator _creator)
        : fromTypes(_fromTypes), toType(_toType), creator(_creator)
    {
    }
    QStringList fromTypes;
    QString toType;
    dtkAbstractDataFactory::dtkAbstractDataConverterCreator creator;
};

struct dtkAbstractDataSerializerInfo
{
    dtkAbstractDataSerializerInfo() : creator(NULL) {}
    dtkAbstractDataSerializerInfo(const QStringList &_handled,
                                  dtkAbstractDataFactory::dtkAbstractDataSerializerCreator _creator)
        : handled(_handled), creator(_creator)
    {
    }
    QStringList handled;
    dtkAbstractDataFactory::dtkAbstractDataSerializerCreator creator;
};

struct dtkAbstractDataDeserializerInfo
{
    dtkAbstractDataDeserializerInfo() : creator(NULL) {}
    dtkAbstractDataDeserializerInfo(
            const QStringList &_handled,
            dtkAbstractDataFactory::dtkAbstractDataDeserializerCreator _creator)
        : handled(_handled), creator(_creator)
    {
    }
    QStringList handled;
    dtkAbstractDataFactory::dtkAbstractDataDeserializerCreator creator;
};

typedef QHash<QString, dtkAbstractDataFactory::dtkAbstractDataCreator> dtkAbstractDataCreatorHash;
typedef QHash<QString, dtkAbstractDataReaderInfo> dtkAbstractDataReaderCreatorHash;
typedef QHash<QString, dtkAbstractDataWriterInfo> dtkAbstractDataWriterCreatorHash;
typedef QHash<QString, dtkAbstractDataConverterInfo> dtkAbstractDataConverterCreatorHash;
typedef QHash<QString, dtkAbstractDataSerializerInfo> dtkAbstractDataSerializerCreatorHash;
typedef QHash<QString, dtkAbstractDataDeserializerInfo> dtkAbstractDataDeserializerCreatorHash;

typedef QPair<QString, QStringList> dtkAbstractDataTypeHandler;
typedef QPair<QString, QPair<QStringList, QString>> dtkAbstractDataConverterTypeHandler;

typedef QHash<QString, QString> dtkAbstractDataInterfacesHash;
} // namespace

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataFactoryPrivate
// /////////////////////////////////////////////////////////////////

class dtkAbstractDataFactoryPrivate
{
public:
    dtkAbstractDataCreatorHash creators;
    dtkAbstractDataReaderCreatorHash readers;
    dtkAbstractDataWriterCreatorHash writers;
    dtkAbstractDataConverterCreatorHash converters;
    dtkAbstractDataSerializerCreatorHash serializers;
    dtkAbstractDataDeserializerCreatorHash deserializers;
    dtkAbstractDataInterfacesHash interfaces;

    dtkAbstractDataFactory::dtkAbstractDataFactoryReaderPriorityMap reader_priorities;
    dtkAbstractDataFactory::dtkAbstractDataFactoryWriterPriorityMap writer_priorities;
    dtkAbstractDataFactory::dtkAbstractDataFactoryConverterPriorityMap converter_priorities;
};

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataFactory
// /////////////////////////////////////////////////////////////////

DTKCORESUPPORT_EXPORT dtkAbstractDataFactory *dtkAbstractDataFactory::instance(void)
{
    if (!s_instance)
        s_instance = new dtkAbstractDataFactory;

    return s_instance;
}

dtkAbstractData *dtkAbstractDataFactory::create(const QString &type)
{
    static int count = 0;

    if (!d->creators.contains(type))
        return NULL;

    dtkAbstractData *data = d->creators[type]();

    for (dtkAbstractDataReaderCreatorHash::const_iterator it(d->readers.begin());
         it != d->readers.end(); ++it) {
        if (it.value().handled.contains(type))
            data->addReader(it.key());
    }

    for (dtkAbstractDataWriterCreatorHash::const_iterator it(d->writers.begin());
         it != d->writers.end(); ++it) {
        if (it.value().handled.contains(type))
            data->addWriter(it.key());
    }

    for (dtkAbstractDataConverterCreatorHash::const_iterator it(d->converters.begin());
         it != d->converters.end(); ++it) {
        if (it.value().fromTypes.contains(type)) {
            data->addConverter(it.key());
            data->enableConverter(it.key());
        }
    }

    for (dtkAbstractDataSerializerCreatorHash::const_iterator it(d->serializers.begin());
         it != d->serializers.end(); ++it) {
        if (it.value().handled.contains(type)) {
            data->addSerializer(it.key());
            data->enableSerializer(it.key());
        }
    }

    for (dtkAbstractDataDeserializerCreatorHash::const_iterator it(d->deserializers.begin());
         it != d->deserializers.end(); ++it) {
        if (it.value().handled.contains(type)) {
            data->addDeserializer(it.key());
            data->enableDeserializer(it.key());
        }
    }

    data->setObjectName(QString("%1%2").arg(data->metaObject()->className()).arg(count++));

    emit created(data, type);

    return data;
}

dtkSmartPointer<dtkAbstractData> dtkAbstractDataFactory::createSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractData> data = this->create(type);
    return data;
}

dtkAbstractDataReader *dtkAbstractDataFactory::reader(const QString &type)
{
    dtkAbstractDataReaderCreatorHash::const_iterator it(d->readers.find(type));

    if (it == d->readers.end())
        return NULL;
    else
        return it->creator();
}

dtkSmartPointer<dtkAbstractDataReader>
dtkAbstractDataFactory::readerSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractDataReader> reader = this->reader(type);
    return reader;
}

dtkAbstractDataWriter *dtkAbstractDataFactory::writer(const QString &type)
{
    dtkAbstractDataWriterCreatorHash::const_iterator it(d->writers.find(type));

    if (it == d->writers.end())
        return NULL;
    else
        return it->creator();
}

dtkSmartPointer<dtkAbstractDataWriter>
dtkAbstractDataFactory::writerSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractDataWriter> writer = this->writer(type);
    return writer;
}

dtkAbstractDataConverter *dtkAbstractDataFactory::converter(const QString &type)
{
    dtkAbstractDataConverterCreatorHash::const_iterator it(d->converters.find(type));

    if (it == d->converters.end())
        return NULL;
    else
        return it->creator();
}

dtkSmartPointer<dtkAbstractDataConverter>
dtkAbstractDataFactory::converterSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractDataConverter> converter = this->converter(type);
    return converter;
}

dtkAbstractDataSerializer *dtkAbstractDataFactory::serializer(const QString &type)
{
    dtkAbstractDataSerializerCreatorHash::const_iterator it(d->serializers.find(type));

    if (it == d->serializers.end())
        return NULL;
    else
        return it->creator();
}

dtkSmartPointer<dtkAbstractDataSerializer>
dtkAbstractDataFactory::serializerSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractDataSerializer> serializer;
    serializer.takePointer(this->serializer(type));
    return serializer;
}

dtkAbstractDataDeserializer *dtkAbstractDataFactory::deserializer(const QString &type)
{
    dtkAbstractDataDeserializerCreatorHash::const_iterator it(d->deserializers.find(type));

    if (it == d->deserializers.end())
        return NULL;
    else
        return it->creator();
}

dtkSmartPointer<dtkAbstractDataDeserializer>
dtkAbstractDataFactory::deserializerSmartPointer(const QString &type)
{
    dtkSmartPointer<dtkAbstractDataDeserializer> deserializer;
    deserializer.takePointer(this->deserializer(type));
    return deserializer;
}

void dtkAbstractDataFactory::setReaderPriorities(
        const dtkAbstractDataFactoryReaderPriorityMap &priorities)
{
    d->reader_priorities = priorities;
}

void dtkAbstractDataFactory::setWriterPriorities(
        const dtkAbstractDataFactoryWriterPriorityMap &priorities)
{
    d->writer_priorities = priorities;
}

void dtkAbstractDataFactory::setConverterPriorities(
        const dtkAbstractDataFactoryConverterPriorityMap &priorities)
{
    d->converter_priorities = priorities;
}

const dtkAbstractDataFactory::dtkAbstractDataFactoryReaderPriorityMap &
dtkAbstractDataFactory::readerPriorities(void) const
{
    return d->reader_priorities;
}

const dtkAbstractDataFactory::dtkAbstractDataFactoryWriterPriorityMap &
dtkAbstractDataFactory::writerPriorities(void) const
{
    return d->writer_priorities;
}

const dtkAbstractDataFactory::dtkAbstractDataFactoryConverterPriorityMap &
dtkAbstractDataFactory::converterPriorities(void) const
{
    return d->converter_priorities;
}

QStringList dtkAbstractDataFactory::implementations(void)
{
    return d->interfaces.values();
}

QStringList dtkAbstractDataFactory::implementations(const QString &interface)
{
    QStringList implementations;

    if (d->interfaces.keys().contains(interface))
        implementations << d->interfaces.values(interface);
    else
        qDebug() << "There is no available implementation of " << interface;

    return implementations;
}

QStringList dtkAbstractDataFactory::interfaces(void)
{
    return d->interfaces.keys();
}

bool dtkAbstractDataFactory::registerDataType(const QString &type, dtkAbstractDataCreator func)
{
    if (!d->creators.contains(type)) {
        d->creators.insert(type, func);
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataType(const QString &type, dtkAbstractDataCreator func,
                                              const QString &interface)
{
    if (!d->creators.contains(type)) {
        d->creators.insert(type, func);
        d->interfaces.insertMulti(interface, type);
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataReaderType(const QString &type, const QStringList &handled,
                                                    dtkAbstractDataReaderCreator func)
{
    if (!d->readers.contains(type)) {
        d->readers.insert(type, dtkAbstractDataReaderInfo(handled, func));
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataWriterType(const QString &type, const QStringList &handled,
                                                    dtkAbstractDataWriterCreator func)
{
    if (!d->writers.contains(type)) {
        d->writers.insert(type, dtkAbstractDataWriterInfo(handled, func));
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataConverterType(const QString &type,
                                                       const QStringList &fromTypes,
                                                       const QString &toType,
                                                       dtkAbstractDataConverterCreator func)
{
    if (!d->converters.contains(type)) {
        d->converters.insert(type, dtkAbstractDataConverterInfo(fromTypes, toType, func));
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataSerializerType(const QString &type,
                                                        const QStringList &handled,
                                                        dtkAbstractDataSerializerCreator func)
{
    if (!d->serializers.contains(type)) {
        d->serializers.insert(type, dtkAbstractDataSerializerInfo(handled, func));
        return true;
    }

    return false;
}

bool dtkAbstractDataFactory::registerDataDeserializerType(const QString &type,
                                                          const QStringList &handled,
                                                          dtkAbstractDataDeserializerCreator func)
{
    if (!d->deserializers.contains(type)) {
        d->deserializers.insert(type, dtkAbstractDataDeserializerInfo(handled, func));
        return true;
    }

    return false;
}

unsigned int dtkAbstractDataFactory::count(const QString &type) const
{
    DTK_UNUSED(type);
    return d->creators.keys().count();
}

unsigned int dtkAbstractDataFactory::countReaders(const QString &type) const
{
    DTK_UNUSED(type);
    return d->readers.keys().count();
}

unsigned int dtkAbstractDataFactory::countWriters(const QString &type) const
{
    DTK_UNUSED(type);
    return d->writers.keys().count();
}

unsigned int dtkAbstractDataFactory::countConverters(const QString &type) const
{
    DTK_UNUSED(type);
    return d->converters.keys().count();
}

unsigned int dtkAbstractDataFactory::countSerializers(const QString &type) const
{
    DTK_UNUSED(type);
    return d->serializers.keys().count();
}

unsigned int dtkAbstractDataFactory::countDeserializers(const QString &type) const
{
    DTK_UNUSED(type);
    return d->deserializers.keys().count();
}

QList<QString> dtkAbstractDataFactory::creators(void) const
{
    return d->creators.keys();
}

QList<QString> dtkAbstractDataFactory::readers(void) const
{
    if (d->reader_priorities.isEmpty())
        return d->readers.keys();

    const QStringList priorities = d->reader_priorities.values();

    QMap<int, QString> readers;

    foreach (QString reader, d->readers.keys())
        if (priorities.contains(reader))
            readers.insert(d->reader_priorities.key(reader), reader);

    return readers.values();
}

QList<QString> dtkAbstractDataFactory::writers(void) const
{
    if (d->writer_priorities.isEmpty())
        return d->writers.keys();

    const QStringList priorities = d->writer_priorities.values();

    QMap<int, QString> writers;

    foreach (QString writer, d->writers.keys())
        if (priorities.contains(writer))
            writers.insert(d->writer_priorities.key(writer), writer);

    return writers.values();
}

QList<QString> dtkAbstractDataFactory::converters(void) const
{
    if (d->converter_priorities.isEmpty())
        return d->converters.keys();

    const QStringList priorities = d->converter_priorities.values();

    QMap<int, QString> converters;

    foreach (QString converter, d->converters.keys())
        if (priorities.contains(converter))
            converters.insert(d->converter_priorities.key(converter), converter);

    return converters.values();
}

QList<QString> dtkAbstractDataFactory::serializers(void) const
{
    return d->serializers.keys();
}

QList<QString> dtkAbstractDataFactory::deserializers(void) const
{
    return d->deserializers.keys();
}

dtkAbstractDataFactory::dtkAbstractDataFactory(void)
    : dtkAbstractFactory(), d(new dtkAbstractDataFactoryPrivate)
{
}

dtkAbstractDataFactory::~dtkAbstractDataFactory(void)
{
    delete d;

    d = NULL;
}

dtkAbstractDataFactory *dtkAbstractDataFactory::s_instance = NULL;
