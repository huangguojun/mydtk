/* dtkSignalBlocker.h ---
 *
 * Author: John Stark
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Fri Jun 24 16:19:07 2011 (+0200)
 * Version: $Id: 8683d7972bcc259942e423a08dc96b178d95fc4b $
 * Last-Updated: Fri Jun 24 16:20:35 2011 (+0200)
 *           By: Julien Wintz
 *     Update #: 16
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKSIGNALBLOCKER_H
#define DTKSIGNALBLOCKER_H

#include <QObject>

class dtkSignalBlocker
{
public:
    explicit dtkSignalBlocker(QObject *o) : m_object(o) {
        m_object->blockSignals(true);
    }

    ~dtkSignalBlocker(void) {
        m_object->blockSignals(false);
    }

    void blockSignals(bool v) {
        m_object->blockSignals(v);
    }

private:
    QObject *m_object;

private:
    dtkSignalBlocker(const dtkSignalBlocker&);
    dtkSignalBlocker& operator=(const dtkSignalBlocker&);
};

#endif
