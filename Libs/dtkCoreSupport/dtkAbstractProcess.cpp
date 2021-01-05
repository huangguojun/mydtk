/* dtkAbstractProcess.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 16:01:09 2008 (+0100)
 * Version: $Id: 264cb32896ffae838f33270c574db57882b8f635 $
 * Last-Updated: lun. févr.  3 16:46:42 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 115
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractProcess.h"
#include "dtkAbstractProcess_p.h"

// /////////////////////////////////////////////////////////////////
// dtkAbstractProcess implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess::dtkAbstractProcess(dtkAbstractProcess *parent)
    : dtkAbstractObject(*new dtkAbstractProcessPrivate(this), parent)
{
}

dtkAbstractProcess::dtkAbstractProcess(const dtkAbstractProcess &other)
    : dtkAbstractObject(*new dtkAbstractProcessPrivate(*other.d_func()), other)
{
}

dtkAbstractProcess::~dtkAbstractProcess(void) {}

//! Returns a new dtkAbstractProcess that is a copy of this.
/*!
 *  This method can be overloaded through the hierarchy enabling a
 *  deep copy of this. Note that, using covariance of returned type,
 *  the returned argument can be of the more derived type.
 *
 *  Example:
 *  \code
 *  class xyzProcess : public dtkAbstractProcess
 *  {
 *    ...
 *    xyzProcess *clone(void); // Covariant returned argument
 *    ...
 *  };
 *
 *  xyzProcess *xyzProcess::clone(void)
 *  {
 *     return new xyzProcess(*this);
 *  }
 *  \endcode
 */
dtkAbstractProcess *dtkAbstractProcess::clone(void)
{
    return new dtkAbstractProcess(*this);
}

dtkAbstractProcess &dtkAbstractProcess::operator=(const dtkAbstractProcess &other)
{
    this->copy(other);

    return (*this);
}

//! Enables to make a deep copy of the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the assignement operator which delegates
 *  the copy process. When re-implementing this method into a derived
 *  class of dtkAbstractProcess, one must called first the copy method
 *  of the parent to ensure that all the attributes are really copied.
 *
 *  Nevertheless, some caution must be taken to avoid slicing problem
 *  as shown in the following example.
 *
 *  Example:
 *  \code
 *  class xyzProcess : public dtkAbstractProcess
 *  {
 *    ...
 *    void copy(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  void xyzProcess::copy(const dtkAbstractObject& other)
 *  {
 *     // copy of the parent attributes
 *     dtkAbstractProcess::copy(other);
 *
 *     // copy of the xyzProcess attributes
 *     if (other.identifier() == this->identifier()) {
 *        // cast other into xyzProcess and do the copy
 *     } else {
 *        dtkWarn() << "other is not of same type than this, slicing is
 * occuring.";
 *     }
 *  }
 *  \endcode
 */
void dtkAbstractProcess::copy(const dtkAbstractObject &other)
{
    dtkAbstractObject::copy(other);
}

QDebug operator<<(QDebug debug, const dtkAbstractProcess &process)
{
    debug.nospace() << process.identifier();

    return debug.space();
}

QDebug operator<<(QDebug debug, dtkAbstractProcess *process)
{
    debug.nospace() << process->identifier();

    return debug.space();
}

int dtkAbstractProcess::run(void)
{
    int retval = this->update();

    if (retval == 0)
        emit success();
    else
        emit failure();

    return retval;
}

void dtkAbstractProcess::cancel(void)
{
    onCanceled();
    emit canceled();
}

int dtkAbstractProcess::update(void)
{
    // DTK_DEFAULT_IMPLEMENTATION;

    return DTK_FAILURE;
}

void dtkAbstractProcess::onCanceled(void)
{
    // DTK_DEFAULT_IMPLEMENTATION;
}

bool dtkAbstractProcess::read(const QString &file)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(file);

    return false;
}

bool dtkAbstractProcess::read(const QStringList &files)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(files);

    return false;
}

bool dtkAbstractProcess::write(const QString &file)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(file);

    return false;
}

bool dtkAbstractProcess::write(const QStringList &files)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(files);

    return false;
}

void dtkAbstractProcess::setParameter(int data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(int data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(int data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(int *data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(int *data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(int *data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(qlonglong data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(qlonglong data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(qlonglong data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(qlonglong *data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(qlonglong *data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(qlonglong *data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(double data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(double data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(double data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(double *data, int count)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(count);
}

void dtkAbstractProcess::setParameter(double *data, int count, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(count);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(double *data, int count, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(count);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setParameter(dtkAbstractObject *data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setParameter(dtkAbstractObject *data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setParameter(dtkAbstractObject *data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setInput(dtkAbstractData *data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setInput(dtkAbstractData *data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setInput(dtkAbstractData *data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

void dtkAbstractProcess::setData(void *data)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
}

void dtkAbstractProcess::setData(void *data, int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractProcess::setData(void *data, int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
    Q_UNUSED(frame);
}

dtkAbstractData *dtkAbstractProcess::output(void)
{
    // DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

dtkAbstractData *dtkAbstractProcess::output(int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return NULL;
}

dtkAbstractData *dtkAbstractProcess::output(int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);
    Q_UNUSED(frame);

    return NULL;
}

void *dtkAbstractProcess::data(void)
{
    // DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *dtkAbstractProcess::data(int channel)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);

    return NULL;
}

void *dtkAbstractProcess::data(int channel, int frame)
{
    // DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(channel);
    Q_UNUSED(frame);

    return NULL;
}

int dtkAbstractProcess::channelCount(void)
{
    return 0;
}
