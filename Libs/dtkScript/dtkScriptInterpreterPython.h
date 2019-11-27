// Version: $Id: 87a6f0ba9feaf44b1d7048d8ae6c3fbf9de20e79 $
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

#include <dtkScriptExport>

#include "dtkScriptInterpreter.h"

class DTKSCRIPT_EXPORT dtkScriptInterpreterPython : public dtkScriptInterpreter
{
    Q_OBJECT

public:
     static dtkScriptInterpreterPython *instance();

protected:
     static dtkScriptInterpreterPython *s_instance;

public slots:
    QString interpret(const QString& command, int *stat) override;
    void init(bool redirect_io = false, const QString& settings_file = QString("dtk-script") ) override ;

public:
    void allowThreads(void);     // need to be called from main thread
    void endAllowThreads(void);  // need to be called from main thread
    void childAcquireLock(void); // need to be called from child thread
    void childReleaseLock(void); // need to be called from child thread

public:
    void release(void);

private:
     dtkScriptInterpreterPython(void);
     ~dtkScriptInterpreterPython(void);

private:
    class dtkScriptInterpreterPythonPrivate *d;
};

//
// dtkScriptInterpreterPython.h ends here
