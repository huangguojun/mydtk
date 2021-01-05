/* dtkScriptInterpreter.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Thu Jan 10 14:09:18 2013 (+0100)
 * Version: $Id: 69caf644dae9275feadd259f04e02791fd96503d $
 * Last-Updated: Thu Jan 10 14:11:46 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 15
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkScriptInterpreter.h"

// /////////////////////////////////////////////////////////////////
// dtkScriptInterpreterPrivate
// /////////////////////////////////////////////////////////////////

class dtkScriptInterpreterPrivate
{
public:
};

// /////////////////////////////////////////////////////////////////
// dtkScriptInterpreter
// /////////////////////////////////////////////////////////////////

dtkScriptInterpreter::dtkScriptInterpreter(QObject *parent)
    : QObject(parent), d(new dtkScriptInterpreterPrivate)
{
}

dtkScriptInterpreter::~dtkScriptInterpreter(void)
{
    delete d;

    d = NULL;
}
