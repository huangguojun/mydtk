/* main.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:14:10 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Sat Jun  2 01:45:46 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 13
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "ccMainWindow.h"

//#include <dtkCore/dtkGlobal.h>
//#include <dtkCore/dtkPluginManager.h>

#include <dtkLog/dtkLog.h>
#include <dtkLog/dtkLogger.h>

#include <QtCore>
#include <QtGui>
#include <dtkWidgets>

int main(int argc, char **argv)
{
    //QApplication application(argc, argv);
    dtkApplication *application = dtkApplication::create(argc, argv);

 //   dtkLogger::instance().setLevel(dtkLog::Debug);
  //  dtkLogger::instance().attachConsole();
   // dtkLogger::instance().attachFile(dtkLogPath(&application));

    /*
    dtkPluginManager::instance()->setVerboseLoading(true);
    dtkPluginManager::instance()->initialize();
    */

    ccMainWindow *window = new ccMainWindow;
    window->show();
    window->raise();

    int status = application->exec();

    delete window;

    //dtkPluginManager::instance()->uninitialize();

    return status;
}
