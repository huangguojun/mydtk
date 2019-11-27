// Version: $Id: 7205dad96655b3391791437c4456a08e9b424f3e $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkLog.h"
#include <dtkLogExport.h>

#include <QtCore>

/*!
  \namespace dtkLog

  \brief Contains main enumeration and helper functions used throughout the Log module.

  \inmodule dtkLog

  Log messages are redirected to a stream that is connected to one or many destinations.

  The logging layer should be configured as such:
\code
QString log_file = dtkLogPath(qApp);

dtkLogger::instance()->attachConsole();
dtkLogger::instance()->attachFile(log_file);

dtkLogger::setLevel(dtkLog::Info);

dtkDebug() << "Debug messaage"; // will not output as dtkLog::Debug < dtkLog::Info
dtkWarn()  <<  "Warn messaage"; // will     output as dtkLog::Warn >= dtkLog::Info
\endcode

  In this example, the console is attached to the logger, and a file
  which path is retrieved using the \c dtkLogPath function using Qt
  standard path in conjunction with the application name that has to
  be set correctly.

 */

/*!
 * \enum dtkLog::Level
 *
 * This enum type specifies a corner in a rectangle:
 *
 * \value Trace
 *        ...
 * \value Debug
 *        ...
 * \value Info
 *        ...
 * \value Warn
 *        ...
 * \value Error
 *        ...
 * \value Fatal
 *        ...
 */

/*!
 * \macro dtkTrace()
 * \relates dtkLog
 *
 * Provides a stream to send Trace level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \macro dtkDebug()
 * \relates dtkLog
 *
 * Provides a stream to send Debug level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \macro dtkInfo()
 * \relates dtkLog
 *
 * Provides a stream to send Info level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \macro dtkWarn()
 * \relates dtkLog
 *
 * Provides a stream to send Warn level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \macro dtkError()
 * \relates dtkLog
 *
 * Provides a stream to send Error level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \macro dtkFatal()
 * \relates dtkLog
 *
 * Provides a stream to send Fatal level messages.
 *
 * The output will be carried on depending on the dtkLogger instance log Level.
 */

/*!
 * \relates dtkLog
 *
 * Convenience method to return the path to an \a application's log file
 * using the standard data location path.
 */
QString dtkLogPath(QCoreApplication *application)
{
    return QDir(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first()).filePath(QString("%1.log").arg(application->applicationName()));
}

//
// dtkLog.cpp ends here
