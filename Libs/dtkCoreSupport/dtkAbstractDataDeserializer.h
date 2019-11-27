/* dtkAbstractDataDeserializer.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2011 - Nicolas Niclausse, Inria.
 * Created: lun. oct. 17 13:20:01 2011 (+0200)
 * Version: $Id: 803e3e9a0d0d49ed1ca0de9c86265070937e980f $
 * Last-Updated: mar. mai 29 09:10:28 2012 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 23
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATADESERIALIZER_H
#define DTKABSTRACTDATADESERIALIZER_H


#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractDataDeserializerPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataDeserializer interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataDeserializer : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractDataDeserializer(void);
    dtkAbstractDataDeserializer(const dtkAbstractDataDeserializer& other);
    virtual ~dtkAbstractDataDeserializer(void);

public:
    virtual QStringList handled(void) const = 0;

    bool enabled(void) const;
    void enable(void);
    void disable(void);

    dtkAbstractData *data(void) const;
    virtual void setData(dtkAbstractData *data);

signals:
    void started(const QString& message);
    void progressed(int step);
    void finished(void);

public slots:
    virtual dtkAbstractData *deserialize(const QByteArray& array);

    virtual void setProgress(int value);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractDataDeserializer);
};

#endif
