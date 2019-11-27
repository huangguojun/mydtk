// Version: $Id: 40ecb99ed408f4378dc257ab3e9156a3e2edd990 $
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

#include "dtkLog.h"
#include <dtkLogExport.h>

#include <QtCore>
#include <iostream>

#define DEFAULT_MAX_FILE_SIZE 1073741824L

class dtkLogModel;

class DTKLOG_EXPORT dtkLogger
{
public:
    static dtkLogger& instance(void);

public:
    dtkLog::Level       level(void) const;
    QString       levelString(void) const;

public:
    void setLevel(dtkLog::Level level);
    void setLevel(QString level);

public:
    void attachConsole(void);
    void attachConsole(dtkLog::Level level);
    void detachConsole(void);

public:
    void attachFile(const QString& path, qlonglong max_file_size = DEFAULT_MAX_FILE_SIZE);
    void detachFile(const QString& path);

public:
    void attachModel(dtkLogModel *model);
    void detachModel(dtkLogModel *model);

public:
    void redirectCout(dtkLog::Level level = dtkLog::Info);
    void redirectCerr(dtkLog::Level level = dtkLog::Error);

private:
     dtkLogger(void);
     dtkLogger(const dtkLogger& other);
    ~dtkLogger(void);

private:
    void write(const QString& message);
    void write(const QString& message, dtkLog::Level level);

private:
    dtkLogger& operator=(const dtkLogger& other);

private:
    class dtkLoggerPrivate *d;

private:
    friend class dtkLogEngine;
    friend class dtkLogEnginePrivate;
};

//
// dtkLogger.h ends here
