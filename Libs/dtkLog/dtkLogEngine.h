// Version: $Id: 8708f3feb0d79dc91fdefd611a3b39a75db361a7 $
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

#include <QtDebug>

class dtkLogEnginePrivate;

class DTKLOG_EXPORT dtkLogEngine
{
public:
    dtkLogEngine(dtkLog::Level level, bool custom = false);
    ~dtkLogEngine(void);

public:
    QDebug stream(void);

private:
    dtkLogEnginePrivate *d;
};

//
// dtkLogEngine.h ends here
