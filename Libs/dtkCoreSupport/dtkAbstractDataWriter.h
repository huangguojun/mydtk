/* dtkAbstractDataWriter.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 21:58:48 2009 (+0100)
 * Version: $Id: e905d2f7c0a410a32c5c4751cf942870b10b21bc $
 * Last-Updated: Tue Apr 24 10:47:09 2012 (+0200)
 *           By: tkloczko
 *     Update #: 40
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATAWRITER_H
#define DTKABSTRACTDATAWRITER_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractDataWriterPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataWriter interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataWriter : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractDataWriter(void);
    dtkAbstractDataWriter(const dtkAbstractDataWriter &other);
    virtual ~dtkAbstractDataWriter(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList handled(void) const = 0;

    bool enabled(void) const;
    void enable(void);
    void disable(void);

    dtkAbstractData *data(void) const;

    virtual void setData(dtkAbstractData *data);

    virtual QStringList supportedFileExtensions(void) const;

signals:
    void started(const QString &message);
    void progressed(int step);
    void finished(void);

public slots:
    virtual bool canWrite(const QString &file);
    virtual bool canWrite(const QStringList &files);

    virtual bool write(const QString &file);
    virtual bool write(const QStringList &files);

    virtual void setProgress(int value);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractDataWriter);
};

#endif
