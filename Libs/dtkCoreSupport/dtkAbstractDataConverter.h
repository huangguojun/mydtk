/* dtkAbstractDataConverter.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 21:58:48 2009 (+0100)
 * Version: $Id: a667a69f281ac1588551af8ba6f9c36780897fb5 $
 * Last-Updated: Tue Apr 24 10:52:26 2012 (+0200)
 *           By: tkloczko
 *     Update #: 49
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATACONVERTER_H
#define DTKABSTRACTDATACONVERTER_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractDataConverterPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataConverter interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataConverter : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractDataConverter(void);
    dtkAbstractDataConverter(const dtkAbstractDataConverter &other);
    virtual ~dtkAbstractDataConverter(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList fromTypes(void) const = 0;
    virtual QString toType(void) const = 0;

    bool enabled(void) const;
    void enable(void);
    void disable(void);

    dtkAbstractData *data(void) const;

    virtual void setData(dtkAbstractData *data);

signals:
    void started(const QString &message);
    void progressed(int step);
    void finished(void);

public slots:
    virtual bool canConvert(const QString &toType);

    virtual dtkAbstractData *convert(void);

    virtual void setProgress(int value);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractDataConverter);
};

#endif
