/* dtkAbstractProcessFactory.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 15:48:10 2008 (+0100)
 * Version: $Id: 6643fb5f731146d18cd77d99eeb8ffc7340f667f $
 * Last-Updated: Thu Sep 27 16:11:54 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 67
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTPROCESSFACTORY_H
#define DTKABSTRACTPROCESSFACTORY_H

#include <QtCore>

#include "dtkAbstractFactory.h"

class dtkAbstractProcess;
class dtkAbstractProcessFactoryPrivate;
template<class T> class dtkSmartPointer;

class DTKCORESUPPORT_EXPORT dtkAbstractProcessFactory : public dtkAbstractFactory
{
    Q_OBJECT

public:
    typedef dtkAbstractProcess *(*dtkAbstractProcessCreator)(void);

public:
    static dtkAbstractProcessFactory *instance(void);
    bool registerProcessType(const QString& type, dtkAbstractProcessCreator func);
    bool registerProcessType(const QString& type, dtkAbstractProcessCreator func, const QString& itf);

    unsigned int size(const QString& type);

    bool exists(const QString& type);

    QStringList creators(void) const;

    QStringList implementations(void);
    QStringList implementations(const QString& itf);
    QStringList interfaces(void);

    dtkSmartPointer<dtkAbstractProcess> createSmartPointer(const QString& type);

signals:
    void created(dtkAbstractProcess *process, const QString& type);

public slots:
    dtkAbstractProcess *create(const QString& type);

protected:
    dtkAbstractProcessFactory(void);
    ~dtkAbstractProcessFactory(void);

private:
    static dtkAbstractProcessFactory   *s_instance;

private:
    dtkAbstractProcessFactoryPrivate *d;
};

#endif
