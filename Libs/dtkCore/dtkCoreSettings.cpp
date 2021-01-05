/* dtkCoreSettings.cpp ---
 *
 * Author: Julien Wintz
 * Created: Fri Feb  8 16:53:22 2013 (+0100)
 * Version:
 * Last-Updated: Mon Mar 25 15:01:39 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 15
 */

/* Change Log:
 *
 */

#include "dtkCoreSettings.h"

dtkCoreSettings::dtkCoreSettings(void)
    : QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "dtk-core")
{
}

dtkCoreSettings::~dtkCoreSettings(void) {}
