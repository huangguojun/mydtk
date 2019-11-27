/* dtkAbstractDataSerializer.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2011 - Nicolas Niclausse, Inria.
 * Created: lun. oct. 17 13:20:01 2011 (+0200)
 * Version: $Id: 76492cec435b4b7de6318e139ce06f66b8217f3a $
 * Last-Updated: Tue Apr 24 11:11:02 2012 (+0200)
 *           By: tkloczko
 *     Update #: 43
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATASERIALIZER_H
#define DTKABSTRACTDATASERIALIZER_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractDataSerializerPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataSerializer interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataSerializer : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractDataSerializer(void);
    dtkAbstractDataSerializer(const dtkAbstractDataSerializer& other);
    virtual ~dtkAbstractDataSerializer(void);

public:
    virtual QStringList handled(void) const = 0;

    bool enabled(void) const;
    void enable(void);
    void disable(void);

    QByteArray *data(void) const;

signals:
    void started(const QString& message);
    void progressed(int step);
    void finished(void);

public slots:
    virtual QByteArray *serialize(void);
    virtual QByteArray *serialize(dtkAbstractData *data);

public slots:
    virtual void setProgress(int value);

protected:
    virtual void setData(QByteArray *array);


private:
    DTK_DECLARE_PRIVATE(dtkAbstractDataSerializer);
};

#endif
