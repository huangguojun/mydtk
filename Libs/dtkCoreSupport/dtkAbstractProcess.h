/* dtkAbstractProcess.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 16:00:26 2008 (+0100)
 * Version: $Id: eca2568c9c3c4f4895360768ce70b7bb30870280 $
 * Last-Updated: Mon Oct 22 14:54:38 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 148
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTPROCESS_H
#define DTKABSTRACTPROCESS_H

#include "dtkAbstractObject.h"

class dtkAbstractData;
class dtkAbstractProcessPrivate;

// /////////////////////////////////////////////////////////////////
// dtkAbstractProcess interface
// /////////////////////////////////////////////////////////////////

class DTKCORESUPPORT_EXPORT dtkAbstractProcess : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractProcess(      dtkAbstractProcess *parent = 0);
    dtkAbstractProcess(const dtkAbstractProcess& other);
    virtual ~dtkAbstractProcess(void);

public:
    virtual dtkAbstractProcess *clone(void);

public:
    dtkAbstractProcess& operator = (const dtkAbstractProcess& other);

protected:
    virtual void copy(const dtkAbstractObject& other);

public:
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractProcess& process);
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug,       dtkAbstractProcess *process);

signals:
    void started(const QString& message);
    void elapsed(const QString& duration);
    void progressed(const QString& message);
    void progressed(int step);
    void success(void);
    void failure(void);
    void finished(void);
    void canceled(void);

public slots:
    void cancel(void);

    int run(void);

    virtual  int update(void);

    virtual void onCanceled (void);

    virtual bool read(const QString& file);
    virtual bool read(const QStringList& files);

    virtual bool write(const QString& file);
    virtual bool write(const QStringList& files);

    virtual void setParameter(int data);
    virtual void setParameter(int data, int channel);
    virtual void setParameter(int data, int channel, int frame);

    virtual void setParameter(int *data);
    virtual void setParameter(int *data, int channel);
    virtual void setParameter(int *data, int channel, int frame);

    virtual void setParameter(qlonglong data);
    virtual void setParameter(qlonglong data, int channel);
    virtual void setParameter(qlonglong data, int channel, int frame);

    virtual void setParameter(qlonglong *data);
    virtual void setParameter(qlonglong *data, int channel);
    virtual void setParameter(qlonglong *data, int channel, int frame);

    virtual void setParameter(double  data);
    virtual void setParameter(double  data, int channel);
    virtual void setParameter(double  data, int channel, int frame);

    virtual void setParameter(double *data, int count);
    virtual void setParameter(double *data, int count, int channel);
    virtual void setParameter(double *data, int count, int channel, int frame);

    virtual void setParameter(dtkAbstractObject  *data);
    virtual void setParameter(dtkAbstractObject  *data, int channel);
    virtual void setParameter(dtkAbstractObject  *data, int channel, int frame);

    virtual void setInput(dtkAbstractData *data);
    virtual void setInput(dtkAbstractData *data, int channel);
    virtual void setInput(dtkAbstractData *data, int channel, int frame);

    virtual void setData(void *data);
    virtual void setData(void *data, int channel);
    virtual void setData(void *data, int channel, int frame);

    virtual dtkAbstractData *output(void);
    virtual dtkAbstractData *output(int channel);
    virtual dtkAbstractData *output(int channel, int frame);

    virtual void *data (void);
    virtual void *data (int channel);
    virtual void *data (int channel, int frame);

    virtual int channelCount(void);

private:
    DTK_DECLARE_PRIVATE(dtkAbstractProcess);
};

DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractProcess& process);
DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug,       dtkAbstractProcess *process);

#endif
