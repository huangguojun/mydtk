/* dtkAbstractDataReader.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Feb 24 21:58:48 2009 (+0100)
 * Version: $Id: ee720d02490c18db783e26f2c75470e2f28141b5 $
 * Last-Updated: Tue Apr 24 10:41:29 2012 (+0200)
 *           By: tkloczko
 *     Update #: 55
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTDATAREADER_H
#define DTKABSTRACTDATAREADER_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractDataReaderPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractDataReader interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractDataReader : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractDataReader(void);
    dtkAbstractDataReader(const dtkAbstractDataReader &other);
    virtual ~dtkAbstractDataReader(void);

public:
    virtual QString description(void) const = 0;
    virtual QStringList handled(void) const = 0;

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
    virtual bool canRead(const QString &file);
    virtual bool canRead(const QStringList &files);

    virtual bool read(const QString &file);
    virtual bool read(const QStringList &files);

    virtual bool readInformation(const QString &path);
    virtual bool readInformation(const QStringList &paths);

    virtual void setProgress(int value);

protected:
    const QString &file(void) const;
    const QStringList &files(void) const;

private:
    DTK_DECLARE_PRIVATE(dtkAbstractDataReader);
};

#endif
