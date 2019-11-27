/* dtkAbstractData.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractData.h"
#include "dtkAbstractData_p.h"
#include "dtkAbstractDataFactory.h"
#include "dtkAbstractDataReader.h"
#include "dtkAbstractDataWriter.h"
#include "dtkAbstractDataConverter.h"
#include "dtkAbstractDataSerializer.h"
#include "dtkAbstractDataDeserializer.h"

#include <dtkLog/dtkLog>

#include <QPainter>

// /////////////////////////////////////////////////////////////////
// dtkAbstractData implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractData::dtkAbstractData(dtkAbstractData *parent) : dtkAbstractObject(*new dtkAbstractDataPrivate(this), parent)
{
    DTK_D(dtkAbstractData);

    d->numberOfChannels = 0;
}

dtkAbstractData::dtkAbstractData(const dtkAbstractData& other) : dtkAbstractObject(*new dtkAbstractDataPrivate(*other.d_func()), this)
{

}

dtkAbstractData::~dtkAbstractData(void)
{

}

//! Returns a new dtkAbstractData that is a copy of this.
/*!
 *  This method can be overloaded through the hierarchy enabling a
 *  deep copy of this. Note that, using covariance of returned type,
 *  the returned argument can be of the more derived type.
 *
 *  Example:
 *  \code
 *  class xyzData : public dtkAbstractData
 *  {
 *    ...
 *    xyzData *clone(void); // Covariant returned argument
 *    ...
 *  };
 *
 *  xyzData *xyzData::clone(void)
 *  {
 *     return new xyzData(*this);
 *  }
 *  \endcode
 */
dtkAbstractData *dtkAbstractData::clone(void)
{
    return new dtkAbstractData(*this);
}

dtkAbstractData& dtkAbstractData::operator = (const dtkAbstractData& other)
{
    this->copy(other);

    return (*this);
}

//! Enables to make a deep copy of the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the assignement operator which delegates
 *  the copy process. When re-implementing this method into a derived
 *  class of dtkAbstractData, one must called first the copy method
 *  of the parent to ensure that all the attributes are really copied.
 *
 *  Nevertheless, some caution must be taken to avoid slicing problem
 *  as shown in the following example.
 *
 *  Example:
 *  \code
 *  class xyzData : public dtkAbstractData
 *  {
 *    ...
 *    void copy(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  void xyzData::copy(const dtkAbstractObject& other)
 *  {
 *     // copy of the parent attributes
 *     dtkAbstractData::copy(other);
 *
 *     // copy of the xyzData attributes
 *     if (other.identifier() == this->identifier()) {
 *        // cast other into xyzData and do the copy
 *     } else {
 *        dtkWarn() << "other is not of same type than this, slicing is occuring.";
 *     }
 *  }
 *  \endcode
 */
void dtkAbstractData::copy(const dtkAbstractObject& other)
{
    dtkAbstractObject::copy(other);

    if (this->identifier() == other.identifier()) {

        const dtkAbstractData& data = reinterpret_cast<const dtkAbstractData&>(other);

        DTK_D(dtkAbstractData);

        d->readers       = data.d_func()->readers;
        d->writers       = data.d_func()->writers;
        d->converters    = data.d_func()->converters;
        d->serializers   = data.d_func()->serializers;
        d->deserializers = data.d_func()->deserializers;

        d->path  = data.d_func()->path;
        d->paths = data.d_func()->paths;

        d->numberOfChannels = data.d_func()->numberOfChannels;

        d->thumbnails = data.d_func()->thumbnails;

    } else {
        dtkWarn() << "Other is not of same type than this, slicing is occuring.";
    }
}

//! Comparison operator.
/*!
 *
 */
bool dtkAbstractData::operator == (const dtkAbstractData& other) const
{
    return this->isEqual(other);
}

//! Enables to make a deep comparison between all the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the comparator operator == which
 *  delegates the comparison. When re-implementing this method into a
 *  derived class of dtkAbstractData, one must called the isEqual
 *  method of the parent to ensure the comparison through all the
 *  attributes.
 *
 *  Example:
 *  \code
 *  class xyzData : public dtkAbstractData
 *  {
 *    ...
 *    bool isEqual(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  bool xyzData::isEqual(const dtkAbstractObject& other)
 *  {
 *     // comparison of the parent attributes
 *     if (!dtkAbstractData::isEqual(other))
 *        return false;
 *
 *     // comparison of the types.
 *     if (this->identifier() != other.identifier())
 *        return false;
 *
 *     // comparison of the xyzData attributes after casting of other.
 *     ...
 *
 *     // comparison of parent attributes
 *    if (!dtkAbstractData::isEqual(other))
 *       return false;
 *
 *    return true;
 *  }
 *  \endcode
 */
bool dtkAbstractData::isEqual(const dtkAbstractObject& other) const
{
    if (this == &other)
        return true;

    if (this->identifier() != other.identifier())
        return false;

    const dtkAbstractData& data = reinterpret_cast<const dtkAbstractData&>(other);

    DTK_D(const dtkAbstractData);

    if (d->readers          != data.d_func()->readers          ||
            d->writers          != data.d_func()->writers          ||
            d->converters       != data.d_func()->converters       ||
            d->serializers      != data.d_func()->serializers      ||
            d->deserializers    != data.d_func()->deserializers    ||
            d->path             != data.d_func()->path             ||
            d->paths            != data.d_func()->paths            ||
            d->numberOfChannels != data.d_func()->numberOfChannels ||
            d->thumbnails       != data.d_func()->thumbnails)
        return false;

    if (!dtkAbstractObject::isEqual(other))
        return false;

    return true;
}

void dtkAbstractData::addReader(const QString& reader)
{
    DTK_D(dtkAbstractData);

    d->readers.insert(reader, false);
}

void dtkAbstractData::addWriter(const QString& writer)
{
    DTK_D(dtkAbstractData);

    d->writers.insert(writer, false);
}

void dtkAbstractData::addConverter(const QString& converter)
{
    DTK_D(dtkAbstractData);
    d->converters.insert(converter, false);
}

void dtkAbstractData::addSerializer(const QString& serializer)
{
    DTK_D(dtkAbstractData);

    d->serializers.insert(serializer, false);
}

void dtkAbstractData::addDeserializer(const QString& deserializer)
{
    DTK_D(dtkAbstractData);

    d->deserializers.insert(deserializer, false);
}

void dtkAbstractData::enableReader(const QString& reader)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->readers.find(reader));

    if (it != d->readers.end())
        it.value() = true;
    else
        dtkDebug() << this->identifier() << " has no such reader: " << reader;
}

void dtkAbstractData::disableReader(const QString& reader)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->readers.find(reader));

    if (it != d->readers.end())
        it.value() = false;
}

void dtkAbstractData::enableWriter(const QString& writer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->writers.find(writer));

    if (it != d->writers.end())
        it.value() = true;
    else
        dtkDebug() << this->identifier() << " has no such writer: " << writer;
}

void dtkAbstractData::disableWriter(const QString& writer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->writers.find(writer));

    if (it != d->writers.end())
        it.value() = false;
}

void dtkAbstractData::enableConverter(const QString& converter)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->converters.find(converter));

    if (it != d->converters.end())
        it.value() = true;
    else
        dtkDebug() << this->identifier() << " has no such converter: " << converter;
}

void dtkAbstractData::disableConverter(const QString& converter)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->converters.find(converter));

    if (it != d->converters.end())
        it.value() = false;
}

void dtkAbstractData::enableSerializer(const QString& serializer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->serializers.find(serializer));

    if (it != d->serializers.end())
        it.value() = true;
    else
        dtkDebug() << this->identifier() << " has no such serializer: " << serializer;
}

void dtkAbstractData::disableSerializer(const QString& serializer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->serializers.find(serializer));

    if (it != d->serializers.end())
        it.value() = false;
}

void dtkAbstractData::enableDeserializer(const QString& deserializer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->deserializers.find(deserializer));

    if (it != d->deserializers.end())
        it.value() = true;
    else
        dtkDebug() << this->identifier() << " has no such deserializer: " << deserializer;
}

void dtkAbstractData::disableDeserializer(const QString& deserializer)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::iterator it(d->deserializers.find(deserializer));

    if (it != d->deserializers.end())
        it.value() = false;
}

dtkAbstractDataReader *dtkAbstractData::reader(const QString& type)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::const_iterator it(d->readers.find(type));

    if (it != d->readers.end() && (it.value() == true))
        return dtkAbstractDataFactory::instance()->reader(type);
    else
        return NULL;
}

dtkAbstractDataWriter *dtkAbstractData::writer(const QString& type)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::const_iterator it(d->writers.find(type));

    if (it != d->writers.end() && (it.value() == true))
        return dtkAbstractDataFactory::instance()->writer(type);
    else
        return NULL;
}

dtkAbstractDataConverter *dtkAbstractData::converter(const QString& type)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::const_iterator it(d->converters.find(type));

    if (it != d->converters.end() && (it.value() == true))
        return dtkAbstractDataFactory::instance()->converter(type);
    else
        return NULL;
}

dtkAbstractDataSerializer *dtkAbstractData::serializer(const QString& type)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::const_iterator it(d->serializers.find(type));

    if (it != d->serializers.end() && (it.value() == true))
        return dtkAbstractDataFactory::instance()->serializer(type);
    else
        return NULL;
}

dtkAbstractDataDeserializer *dtkAbstractData::deserializer(const QString& type)
{
    DTK_D(dtkAbstractData);

    QMap<QString, bool>::const_iterator it(d->deserializers.find(type));

    if (it != d->deserializers.end() && (it.value() == true))
        return dtkAbstractDataFactory::instance()->deserializer(type);
    else
        return NULL;
}


int dtkAbstractData::numberOfChannels(void)
{
    DTK_D(dtkAbstractData);

    return d->numberOfChannels;
}

void dtkAbstractData::setNumberOfChannels(int number)
{
    DTK_D(dtkAbstractData);

    d->numberOfChannels = number;
}

void dtkAbstractData::update(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

bool dtkAbstractData::read(const QString& file)
{
    DTK_D(dtkAbstractData);

    bool read = false;

    dtkAbstractDataFactory *factoryInstance = dtkAbstractDataFactory::instance();

    for (QMap<QString, bool>::const_iterator it(d->readers.begin()); it != d->readers.end(); ++it) {

        if (it.value()) {
            QScopedPointer<dtkAbstractDataReader> reader( factoryInstance->reader(it.key()));

            if (!reader)
                continue;

            reader->setData(this);

            if (reader->read(file)) {
                read = true;
                break;
            }
        }
    }

    if (read) {
        if (d->path.isEmpty())
            d->path = file;

        if (!d->paths.contains(file))
            d->paths << file;
    }

    return read;
}

bool dtkAbstractData::read(const QStringList& files)
{
    DTK_D(dtkAbstractData);

    bool read = false;

    dtkAbstractDataFactory *factoryInstance = dtkAbstractDataFactory::instance();

    for (QMap<QString, bool>::const_iterator it(d->readers.begin()); it != d->readers.end(); ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataReader> reader(factoryInstance->reader(it.key()));

            if (!reader)
                continue;

            reader->setData(this);

            if (reader->read(files)) {
                read = true;
                break;
            }
        }
    }

    if (read) {
        foreach (QString file, files) {
            if (!d->paths.contains(file))
                d->paths << file;
        }
    }

    return read;
}

bool dtkAbstractData::write(const QString& file)
{
    DTK_D(dtkAbstractData);

    bool written = false;

    dtkAbstractDataFactory *factoryInstance = dtkAbstractDataFactory::instance();

    for (QMap<QString, bool>::const_iterator it(d->writers.begin()); it != d->writers.end(); ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataWriter> writer(factoryInstance->writer(it.key()));

            if (!writer)
                continue;

            writer->setData(this);

            if (writer->write(file)) {
                written = true;
                break;
            }
        }
    }

    return written;
}

bool dtkAbstractData::write(const QStringList& files)
{
    DTK_D(dtkAbstractData);

    bool written = false;

    dtkAbstractDataFactory *factoryInstance = dtkAbstractDataFactory::instance();

    for (QMap<QString, bool>::const_iterator it(d->writers.begin()); it != d->writers.end(); ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataWriter> writer( factoryInstance->writer(it.key()));

            if (!writer)
                continue;

            writer->setData(this);

            if (writer->write(files)) {
                written = true;
                break;
            }
        }
    }

    return written;
}


dtkAbstractData *dtkAbstractData::convert(const QString& toType)
{
    DTK_D(dtkAbstractData);

    dtkAbstractData *conversion = NULL;

    for (QMap<QString, bool>::const_iterator it(d->converters.begin()); it != d->converters.end() && !conversion ; ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataConverter> converter( dtkAbstractDataFactory::instance()->converter(it.key()));

            if (converter && converter->canConvert(toType)) {
                converter->setData(this);
                conversion = converter->convert();

                if (conversion) {

                    foreach (QString metaDataKey, this->metaDataList())
                        conversion->addMetaData(metaDataKey, this->metaDataValues(metaDataKey));

                    foreach (QString propertyKey, this->propertyList())
                        conversion->addProperty(propertyKey, this->propertyValues(propertyKey));
                }
            }
        }
    }

    return conversion;
}

QByteArray *dtkAbstractData::serialize(void)
{
    DTK_D(dtkAbstractData);

    QByteArray *array = NULL;

    for (QMap<QString, bool>::const_iterator it(d->serializers.begin()); it != d->serializers.end() && array == NULL ; ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataSerializer> serializer( dtkAbstractDataFactory::instance()->serializer(it.key()));

            if (serializer) {

                array = serializer->serialize(this);

                if (array != NULL) {
                    break;
                }
            }
        }
    }

    return array;
}

dtkAbstractData *dtkAbstractData::deserialize(const QByteArray& array)
{
    DTK_D(dtkAbstractData);

    dtkAbstractData *deserialized = NULL;

    for (QMap<QString, bool>::const_iterator it(d->deserializers.begin()); it != d->deserializers.end() && !deserialized ; ++it) {

        if (it.value()) {

            QScopedPointer<dtkAbstractDataDeserializer> deserializer( dtkAbstractDataFactory::instance()->deserializer(it.key()));

            if (deserializer) {
                deserializer->setData(this);

                deserialized = deserializer->deserialize(array);

                if (!deserialized)
                    dtkDebug() << "deserializer failed, try another one ...";
                else
                    break;
            }
        }
    }

    return deserialized;
}

QString dtkAbstractData::path(void)
{
    DTK_D(dtkAbstractData);

    return d->path;
}

QStringList dtkAbstractData::paths(void)
{
    DTK_D(dtkAbstractData);

    return d->paths;
}

QImage& dtkAbstractData::thumbnail(void)
{
    DTK_D(dtkAbstractData);

    QImage *image = new QImage(128, 128, QImage::Format_RGB32);

    QPainter painter(image);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::gray);
    painter.fillRect(image->rect(), Qt::black);

    d->thumbnails << (*image);

    return (*image);
}

QList<QImage>& dtkAbstractData::thumbnails(void)
{
    DTK_D(dtkAbstractData);

    return d->thumbnails;
}

void *dtkAbstractData::output(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *dtkAbstractData::output(int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return NULL;
}

void *dtkAbstractData::data(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *dtkAbstractData::data(int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return NULL;
}

double dtkAbstractData::parameter(int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return -1;
}

void dtkAbstractData::setParameter(int parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(int parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(qlonglong parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(qlonglong parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(float parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(float parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(double parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(double parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(const QString& parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(const QString& parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(dtkAbstractData *parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(dtkAbstractData *parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setParameter(dtkVectorReal parameter)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
}

void dtkAbstractData::setParameter(dtkVectorReal parameter, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(parameter);
    Q_UNUSED(channel);
}

void dtkAbstractData::setData(void *data)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractData::setData(void *data, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

QVariant dtkAbstractData::toVariant(dtkAbstractData *data)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);

    return qVariantFromValue(*data);
}

dtkAbstractData *dtkAbstractData::fromVariant(const QVariant& v)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(v);

    return NULL;
}

QDebug operator<<(QDebug debug, const dtkAbstractData& data)
{
    debug.nospace() << data.identifier();

    return debug.space();
}

QDebug operator<<(QDebug debug, dtkAbstractData *data)
{
    debug.nospace() << data->identifier();

    return debug.space();
}
