/* dtkScriptInterpreterTcl.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 26 16:19:44 2008 (+0100)
 * Version: $Id: 2d0cda92d97ceb60b2b463567a1047041ef8f30e $
 * Last-Updated: Thu Jan 10 14:26:21 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 316
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkScriptInterpreterTcl.h"

#include <tcl.h>

// /////////////////////////////////////////////////////////////////
// dtkScriptInterpreterTclPrivate
// /////////////////////////////////////////////////////////////////

class dtkScriptInterpreterTclPrivate
{
public:
    Tcl_Interp *interpreter;
};

// /////////////////////////////////////////////////////////////////
// dtkScriptInterpreterTcl
// /////////////////////////////////////////////////////////////////

void InitInterpreterChannels(Tcl_Interp *interpreter);

dtkScriptInterpreterTcl::dtkScriptInterpreterTcl(QObject *parent)
    : dtkScriptInterpreter(parent), d(new dtkScriptInterpreterTclPrivate)
{
    d->interpreter = Tcl_CreateInterp();

    InitInterpreterChannels(d->interpreter);

    Tcl_Init(d->interpreter);
}

dtkScriptInterpreterTcl::~dtkScriptInterpreterTcl(void)
{
    Tcl_DeleteInterp(d->interpreter);

    delete d;

    d = NULL;
}

QString dtkScriptInterpreterTcl::interpret(const QString &command, int *stat)
{
    int res = Tcl_Eval(d->interpreter, command.toUtf8().constData());

    switch (res) {
    case TCL_OK:
        *stat = Status_Ok;
        break;

    case TCL_ERROR:
        *stat = Status_Error;
        break;

    case TCL_RETURN:
        *stat = Status_Return;
        break;

    case TCL_BREAK:
        *stat = Status_Break;
        break;

    case TCL_CONTINUE:
        *stat = Status_Continue;
        break;

    default:
        break;
    }

    return QString(Tcl_GetString(Tcl_GetObjResult(d->interpreter)));
}

// /////////////////////////////////////////////////////////////////
// Tcl output channel redirection
// /////////////////////////////////////////////////////////////////

TCL_DECLARE_MUTEX(interpreterMutex)

static int InterpreterInput _ANSI_ARGS_((ClientData instanceData, char *buf, int toRead,
                                         int *errorCode));
static int InterpreterOutput _ANSI_ARGS_((ClientData instanceData, const char *buf, int toWrite,
                                          int *errorCode));
static int InterpreterClose _ANSI_ARGS_((ClientData instanceData, Tcl_Interp *interp));
static void InterpreterWatch _ANSI_ARGS_((ClientData instanceData, int mask));
static int InterpreterHandle _ANSI_ARGS_((ClientData instanceData, int direction,
                                          ClientData *handlePtr));

static Tcl_ChannelType interpreterChannelType = {
    (char *)"interpreter", // Type name.
    NULL, // Always non-blocking.
    InterpreterClose, // Close proc.
    InterpreterInput, // Input proc.
    InterpreterOutput, // Output proc.
    NULL, // Seek proc.
    NULL, // Set option proc.
    NULL, // Get option proc.
    InterpreterWatch, // Watch for events on interpreter.
    InterpreterHandle // Get a handle from the device.
};

#ifdef __WIN32__

#    include <windows.h>

static int ShouldUseInterpreterChannel(int type)
{
    DWORD handleId;
    DCB dcb;
    DWORD interpreterParams;
    DWORD fileType;
    int mode;
    char *bufMode;
    HANDLE handle;

    switch (type) {
    case TCL_STDIN:
        handleId = STD_INPUT_HANDLE;
        mode = TCL_READABLE;
        bufMode = "line";
        break;

    case TCL_STDOUT:
        handleId = STD_OUTPUT_HANDLE;
        mode = TCL_WRITABLE;
        bufMode = "line";
        break;

    case TCL_STDERR:
        handleId = STD_ERROR_HANDLE;
        mode = TCL_WRITABLE;
        bufMode = "none";
        break;

    default:
        return 0;
        break;
    }

    handle = GetStdHandle(handleId);

    if ((handle == INVALID_HANDLE_VALUE) || (handle == 0)) {
        return 1;
    }

    fileType = GetFileType(handle);

    if (fileType == FILE_TYPE_CHAR) {
        dcb.DCBlength = sizeof(DCB);

        if (!GetConsoleMode(handle, &interpreterParams) && !GetCommState(handle, &dcb)) {
            return 1;
        }
    } else if (fileType == FILE_TYPE_UNKNOWN) {
        return 1;
    } else if (Tcl_GetStdChannel(type) == NULL) {
        return 1;
    }

    return 0;
}

#else
#    define ShouldUseInterpreterChannel(chan) (1)
#endif

void InitInterpreterChannels(Tcl_Interp *interp)
{
    Tcl_Channel interpreterChannel;

    Tcl_MutexLock(&interpreterMutex);

    static int interpreterInitialized = 0;

    if (!interpreterInitialized) {

        interpreterInitialized = 1;

        if (ShouldUseInterpreterChannel(TCL_STDIN)) {
            interpreterChannel = Tcl_CreateChannel(&interpreterChannelType, "interpreter0",
                                                   (ClientData)TCL_STDIN, TCL_READABLE);

            if (interpreterChannel != NULL) {
                Tcl_SetChannelOption(NULL, interpreterChannel, "-translation", "lf");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-buffering", "none");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-encoding", "utf-8");
            }

            Tcl_SetStdChannel(interpreterChannel, TCL_STDIN);
        }

        if (ShouldUseInterpreterChannel(TCL_STDOUT)) {
            interpreterChannel = Tcl_CreateChannel(&interpreterChannelType, "interpreter1",
                                                   (ClientData)TCL_STDOUT, TCL_WRITABLE);

            if (interpreterChannel != NULL) {
                Tcl_SetChannelOption(NULL, interpreterChannel, "-translation", "lf");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-buffering", "none");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-encoding", "utf-8");
            }

            Tcl_SetStdChannel(interpreterChannel, TCL_STDOUT);
        }

        if (ShouldUseInterpreterChannel(TCL_STDERR)) {
            interpreterChannel = Tcl_CreateChannel(&interpreterChannelType, "interpreter2",
                                                   (ClientData)TCL_STDERR, TCL_WRITABLE);

            if (interpreterChannel != NULL) {
                Tcl_SetChannelOption(NULL, interpreterChannel, "-translation", "lf");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-buffering", "none");
                Tcl_SetChannelOption(NULL, interpreterChannel, "-encoding", "utf-8");
            }

            Tcl_SetStdChannel(interpreterChannel, TCL_STDERR);
        }
    }

    Tcl_MutexUnlock(&interpreterMutex);
}

#include <iostream>

static int InterpreterOutput(ClientData instanceData, const char *buf, int toWrite, int *errorCode)
{
    *errorCode = 0;

    Tcl_SetErrno(0);

    std::cout << QString(buf).simplified().toUtf8().constData() << std::endl;

    return toWrite;
}

static int InterpreterInput(ClientData instanceData, char *buf, int bufSize, int *errorCode)
{
    return 0;
}

static int InterpreterClose(ClientData instanceData, Tcl_Interp *interp)
{
    return 0;
}

static void InterpreterWatch(ClientData instanceData, int mask) {}

static int InterpreterHandle(ClientData instanceData, int direction, ClientData *handlePtr)
{
    return TCL_ERROR;
}
