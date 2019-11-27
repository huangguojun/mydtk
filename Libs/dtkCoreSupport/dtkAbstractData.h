/* dtkAbstractData.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 16:00:26 2008 (+0100)
 * Version: $Id: f6417e33862ab8e76a41a179ddd3ee6cbc95c697 $
 *           By: Nicolas Niclausse
 *     Update #: 296
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATA_H
#define DTKABSTRACTDATA_H

#include "dtkAbstractObject.h"

#include <dtkMathSupport/dtkVector.h>

#include <QtCore>
#include <QImage>

class dtkAbstractDataReader;
class dtkAbstractDataWriter;
class dtkAbstractDataConverter;
class dtkAbstractDataSerializer;
class dtkAbstractDataDeserializer;
class dtkAbstractDataPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractData interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractData : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractData(      dtkAbstractData *parent = 0);
    dtkAbstractData(const dtkAbstractData& other);
    virtual ~dtkAbstractData(void);

public:
    virtual dtkAbstractData *clone(void);

public:
    dtkAbstractData& operator = (const dtkAbstractData& other);

protected:
    virtual void copy(const dtkAbstractObject& other);

public:
    bool operator == (const dtkAbstractData& other) const;

protected:
    virtual bool isEqual(const dtkAbstractObject& other) const;

public:
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractData& data);
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug,       dtkAbstractData *data);

    virtual void draw(void) {}

public slots:
    virtual bool read(const QString& file);
    virtual bool read(const QStringList& files);

    virtual bool write(const QString& file);
    virtual bool write(const QStringList& files);

    virtual dtkAbstractData *convert(const QString& toType);
    virtual QByteArray    *serialize(void);
    virtual dtkAbstractData *deserialize(const QByteArray& array);

    virtual void *output(void);
    virtual void *output(int channel);

    virtual void *data(void);
    virtual void *data(int channel);

    virtual double parameter(int channel);

    virtual int numberOfChannels(void);

    virtual void setParameter(int parameter);
    virtual void setParameter(int parameter, int channel);

    virtual void setParameter(qlonglong parameter);
    virtual void setParameter(qlonglong parameter, int channel);

    virtual void setParameter(float parameter);
    virtual void setParameter(float parameter, int channel);

    virtual void setParameter(double parameter);
    virtual void setParameter(double parameter, int channel);

    virtual void setParameter(const QString& parameter);
    virtual void setParameter(const QString& parameter, int channel);

    virtual void setParameter(dtkAbstractData *parameter);
    virtual void setParameter(dtkAbstractData *parameter, int channel);

    virtual void setParameter(dtkVectorReal parameter);
    virtual void setParameter(dtkVectorReal parameter, int channel);

    virtual void setData(void *data);
    virtual void setData(void *data, int channel);

    virtual void setNumberOfChannels(int number);

    virtual void update(void);

    void addReader      (const QString& reader);
    void addWriter      (const QString& writer);
    void addConverter   (const QString& converter);
    void addSerializer  (const QString& serializer);
    void addDeserializer(const QString& deserializer);

    void  enableReader(const QString& reader);
    void disableReader(const QString& reader);

    void  enableWriter(const QString& writer);
    void disableWriter(const QString& writer);

    void  enableConverter(const QString& converter);
    void disableConverter(const QString& converter);

    void  enableSerializer(const QString& serializer);
    void disableSerializer(const QString& serializer);

    void  enableDeserializer(const QString& deserializer);
    void disableDeserializer(const QString& deserializer);

    dtkAbstractDataReader       *reader   (const QString& type);
    dtkAbstractDataWriter       *writer   (const QString& type);
    dtkAbstractDataConverter    *converter(const QString& type);
    dtkAbstractDataSerializer   *serializer(const QString& type);
    dtkAbstractDataDeserializer *deserializer(const QString& type);

    QString     path(void);
    QStringList paths(void);

    virtual       QImage& thumbnail(void) ;
    virtual QList<QImage>& thumbnails(void);

public:
    virtual QVariant toVariant(dtkAbstractData *data);
    virtual dtkAbstractData *fromVariant(const QVariant& v);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractData);
};

DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractData& data);
DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug,       dtkAbstractData *data);

Q_DECLARE_METATYPE(dtkAbstractData)
Q_DECLARE_METATYPE(dtkAbstractData *)

#endif
