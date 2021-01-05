// Version: $Id: 6819be11b55f305b3b25a8017dae22487f697181 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <QtCore>

namespace dtkLog {
enum Level {
    Trace = 0x000,
    Debug = 0x001,
    Info = 0x010,
    Warn = 0x011,
    Error = 0x100,
    Fatal = 0x101
};
}

#include "dtkLogEngine.h"
#include "dtkLogger.h"

// /////////////////////////////////////////////////////////////////
// Trace level stream
// /////////////////////////////////////////////////////////////////

#define dtkTrace()                                                                                 \
    if (dtkLogger::instance().level() > dtkLog::Trace)                                             \
        ;                                                                                          \
    else                                                                                           \
        dtkLogEngine(dtkLog::Trace).stream()

// /////////////////////////////////////////////////////////////////
// Debug level stream
// /////////////////////////////////////////////////////////////////

#define dtkDebug()                                                                                 \
    if (dtkLogger::instance().level() > dtkLog::Debug)                                             \
        ;                                                                                          \
    else                                                                                           \
        dtkLogEngine(dtkLog::Debug).stream()

// /////////////////////////////////////////////////////////////////
// Info level stream
// /////////////////////////////////////////////////////////////////

#define dtkInfo()                                                                                  \
    if (dtkLogger::instance().level() > dtkLog::Info)                                              \
        ;                                                                                          \
    else                                                                                           \
        dtkLogEngine(dtkLog::Info).stream()

// /////////////////////////////////////////////////////////////////
// Warn level stream
// /////////////////////////////////////////////////////////////////

#define dtkWarn()                                                                                  \
    if (dtkLogger::instance().level() > dtkLog::Warn)                                              \
        ;                                                                                          \
    else                                                                                           \
        dtkLogEngine(dtkLog::Warn).stream()

// /////////////////////////////////////////////////////////////////
// Error level stream
// /////////////////////////////////////////////////////////////////

#define dtkError()                                                                                 \
    if (dtkLogger::instance().level() > dtkLog::Error)                                             \
        ;                                                                                          \
    else                                                                                           \
        dtkLogEngine(dtkLog::Error).stream()

// /////////////////////////////////////////////////////////////////
// Fatal level stream
// /////////////////////////////////////////////////////////////////

#define dtkFatal() dtkLogEngine(dtkLog::Fatal).stream()

// ///////////////////////////////////////////////////////////////////
// Custom level stream
// ///////////////////////////////////////////////////////////////////

#define dtkLog(level) dtkLogEngine(level, true).stream()

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

QString DTKLOG_EXPORT dtkLogPath(QCoreApplication *application);

//
// dtkLog.h ends here
