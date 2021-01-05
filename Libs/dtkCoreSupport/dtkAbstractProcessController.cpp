/* dtkAbstractProcessController.cpp ---
 *
 * Author: Julien Wintz
 * Created: Thu Aug 29 14:37:38 2013 (+0200)
 * Version:
 * Last-Updated: Thu Aug 29 17:10:46 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 71
 */

/* Change Log:
 *
 */

#include "dtkAbstractProcessController.h"
#include "dtkAbstractProcess.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
// dtkAbstractProcessControllerPrivate
// ///////////////////////////////////////////////////////////////////

class dtkAbstractProcessControllerPrivate : public QThread
{
public:
    void run(void);

public:
    dtkAbstractProcess *process;
};

void dtkAbstractProcessControllerPrivate::run(void)
{
    if (this->process)
        this->process->run();
}

// ///////////////////////////////////////////////////////////////////
// dtkAbstractProcessController
// ///////////////////////////////////////////////////////////////////

dtkAbstractProcessController::dtkAbstractProcessController(void)
    : dtkAbstractObject(), d(new dtkAbstractProcessControllerPrivate)
{
    d->process = NULL;
}

dtkAbstractProcessController::~dtkAbstractProcessController(void)
{
    d->quit();
    d->wait();

    delete d;
}

dtkAbstractProcess *dtkAbstractProcessController::process(void)
{
    return d->process;
}

void dtkAbstractProcessController::setProcess(dtkAbstractProcess *process)
{
    d->process = process;
}

void dtkAbstractProcessController::start(void)
{
    d->start();
}

void dtkAbstractProcessController::stop(void)
{
    d->terminate();
}
