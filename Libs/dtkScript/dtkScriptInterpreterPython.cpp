// Version: $Id: cc841cb57516d2083b800887ee442d07e3f0e132 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:


#include <Python.h>

#include "dtkScriptInterpreterPython.h"

#include <dtkLog/dtkLog.h>
#include <dtkLog/dtkLogger.h>

#include <QtCore>


// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

static const QString dtkScriptInterpreterPythonRedirector_declare =
    "import sys\n"
    "\n"
    "class Redirector:\n"
    "    def __init__(self):\n"
    "        self.data = ''\n"
    "    def write(self, stuff):\n"
    "        self.data+= stuff\n"
    "    def flush(self):\n"
    "        pass\n"
    "    def isatty(self):\n"
    "        return False\n"
    "    def reset(self):\n"
    "        self.data = ''\n";

static const QString dtkScriptInterpreterPythonRedirector_define =
    "redirector = Redirector()\n"
    "sys.stdout = redirector\n"
    "sys.stderr = redirector\n";

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class dtkScriptInterpreterPythonPrivate
{
public:
    QString buffer;
    bool redirect_io = false;

public:
    PyThreadState *thread_state = nullptr;
    long thread_with_gil = -1;
    PyGILState_STATE gilState;

public:
    PyObject *module = nullptr;

};


// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkScriptInterpreterPython *dtkScriptInterpreterPython::instance(void)
{
    if (!s_instance) {
        s_instance = new dtkScriptInterpreterPython;
    }
    return s_instance;
}

dtkScriptInterpreterPython *dtkScriptInterpreterPython::s_instance = nullptr;


dtkScriptInterpreterPython::dtkScriptInterpreterPython(void) : dtkScriptInterpreter(), d(new dtkScriptInterpreterPythonPrivate)
{
    Py_Initialize();
}

dtkScriptInterpreterPython::~dtkScriptInterpreterPython(void)
{
    delete d;
    d = nullptr;
}

void dtkScriptInterpreterPython::init(bool redirect_io, const QString& settings_file)
{
    d->redirect_io = redirect_io;

    if (redirect_io) {
        PyRun_SimpleString(dtkScriptInterpreterPythonRedirector_declare.toUtf8().constData());
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", settings_file);
    settings.beginGroup("modules");
    QString paths = settings.value("path").toString();
    settings.endGroup();

    PyRun_SimpleString("import sys");

    auto path_list = paths.split(":", QString::SkipEmptyParts);
    for (QString path : path_list) {
        PyRun_SimpleString(qPrintable(QString("sys.path.append('%1')").arg(path)));
    }

    settings.beginGroup("init");
    QString init = settings.value("script").toString();
    settings.endGroup();

    if (!init.isEmpty()) {
        PyRun_SimpleString(qPrintable(QString("execfile('%1')").arg(init)));
    } else {
        dtkInfo() << Q_FUNC_INFO << "No init function";
    }
    if (redirect_io) {
        PyRun_SimpleString(dtkScriptInterpreterPythonRedirector_define.toUtf8().constData());
    }
}

void dtkScriptInterpreterPython::allowThreads(void)
{
    //init python threads
    if(!PyEval_ThreadsInitialized()) {
        qDebug() << "init threads";
        PyEval_InitThreads();  // in 3.7 not needed anymore
    }

    d->thread_state = PyEval_SaveThread();
}
void dtkScriptInterpreterPython::endAllowThreads(void)
{
    if(d->thread_state)
        PyEval_RestoreThread(d->thread_state);
    d->thread_state = nullptr;
}

void dtkScriptInterpreterPython::childAcquireLock(void)
{
    d->thread_with_gil = long(QThread::currentThreadId());
    d->gilState = PyGILState_Ensure();
}

void dtkScriptInterpreterPython::childReleaseLock(void)
{
    long current_thread = long(QThread::currentThreadId());
    if(current_thread == d->thread_with_gil) {
        PyGILState_Release(d->gilState);
        d->thread_with_gil = -1;
    }
}

void dtkScriptInterpreterPython::release(void)
{
    Py_Finalize();
}

QString dtkScriptInterpreterPython::interpret(const QString& command, int *stat)
{
    QString statement = command;

    if (command.endsWith(":")) {
        if (!d->buffer.isEmpty()) {
            d->buffer.append("\n");
        }
        d->buffer.append(command);
        return QString();
    }

    if (!command.isEmpty() && command.startsWith(" ")) {
        if (!d->buffer.isEmpty()) {
            d->buffer.append("\n");
        }
        d->buffer.append(command);
        return QString();
    }

    if (command.isEmpty() && !d->buffer.isEmpty()) {
        if (!d->buffer.isEmpty()) {
            d->buffer.append("\n");
        }
        statement = d->buffer;
        d->buffer.clear();
    }

    if (statement.isEmpty()) {
        return QString();
    }

    if (!d->module)
        d->module = PyImport_AddModule("__main__");

    switch(PyRun_SimpleString(qPrintable(statement))) {
    case  0: *stat = Status_Ok;    break;
    case -1: *stat = Status_Error; break;
    default: break;
    }

    if (d->redirect_io) {
        PyObject *redtor = PyObject_GetAttrString(d->module, "redirector");
        if (redtor) {
            PyObject *lresult = PyObject_GetAttrString(redtor, "data");
            char *method = (char *)"reset";
            PyObject_CallMethod(redtor, method, nullptr);

            QString s =  QString(PyString_AsString(lresult)).simplified();
            Py_DECREF(redtor);
            return s;
        } else  {
            qDebug() << "no redirector found";
            return QString();
        }
    }

    PyErr_Print();
    return QString();
}

//
// dtkScriptInterpreterPython.cpp ends here
