/* dtkCoreSettings.h ---
 *
 * Author: Julien Wintz
 * Created: Fri Feb  8 16:51:14 2013 (+0100)
 * Version:
 * Last-Updated: Mon Feb 11 14:19:25 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 4
 */

/* Change Log:
 *
 */

#pragma once

#include <QtCore>

class dtkCoreSettings : public QSettings
{
public:
    explicit dtkCoreSettings(void);
    virtual ~dtkCoreSettings(void);
};
