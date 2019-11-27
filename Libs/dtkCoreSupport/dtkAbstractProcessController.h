/* dtkAbstractProcessController.h ---
 *
 * Author: Julien Wintz
 * Created: Thu Aug 29 14:32:38 2013 (+0200)
 * Version:
 * Last-Updated: Thu Aug 29 17:01:35 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 21
 */

/* Change Log:
 *
 */

#pragma once

#include "dtkAbstractObject"

class dtkAbstractProcess;
class dtkAbstractProcessControllerPrivate;

class DTKCORESUPPORT_EXPORT dtkAbstractProcessController : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractProcessController(void);
    ~dtkAbstractProcessController(void);

public:
    dtkAbstractProcess *process(void);

public:
    void setProcess(dtkAbstractProcess *process);

public slots:
    void start(void);
    void stop(void);

private:
    dtkAbstractProcessControllerPrivate *d;
};
