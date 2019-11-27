// Version: $Id: dfb484125b8d53002386d36549508b6ddc590a47 $
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
#include "dtkLogger.h"
#include "dtkLogger_p.h"
#include "dtkLogEngine.h"
#include "dtkLogEngine_p.h"

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

QString dtkLogLevel2String(dtkLog::Level level)
{
    switch (level) {
    case dtkLog::Trace: return "TRACE"; break;

    case dtkLog::Debug: return "DEBUG"; break;

    case dtkLog::Info:  return "INFO "; break;

    case dtkLog::Warn:  return "WARN "; break;

    case dtkLog::Error: return "ERROR"; break;

    case dtkLog::Fatal: return "FATAL"; break;

    default:
        return "UNKNOWN";
        break;
    };
}

// /////////////////////////////////////////////////////////////////
// dtkLogEnginePrivate
// /////////////////////////////////////////////////////////////////

void dtkLogEnginePrivate::write(void)
{
    const QString message = QString("%1 - %2 - %3")
                            .arg(qPrintable(dtkLogLevel2String(this->level)))
                            .arg(QDateTime::currentDateTime().toString())
                            .arg(this->buffer);

    QMutexLocker lock(&(dtkLogger::instance().d->mutex));

    if (!this->custom)
        dtkLogger::instance().write(message);
    else
        dtkLogger::instance().write(message, level);

    if (this->level ==  dtkLog::Fatal)
        qFatal("Fatal error occured, please check log");
}

// /////////////////////////////////////////////////////////////////
// dtkLogEngine
// /////////////////////////////////////////////////////////////////

dtkLogEngine::dtkLogEngine(dtkLog::Level level, bool custom) : d(new dtkLogEnginePrivate)
{
    d->level = level;
    d->custom = custom;
}

dtkLogEngine::~dtkLogEngine(void)
{
    d->write();

    delete d;

    d = NULL;
}

QDebug dtkLogEngine::stream(void)
{
    return d->stream;
}

//
// dtkLogEngine.cpp ends here
