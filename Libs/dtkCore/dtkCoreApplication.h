/* dtkCoreApplication.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2015 - Nicolas Niclausse, Inria.
 * Created: 2015/02/04 12:01:21
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkCoreExport.h>
#include <dtkLog>

#include <QCoreApplication>

class dtkApplicationPrivate;

class DTKCORE_EXPORT dtkCoreApplication : public QCoreApplication
{
public:
    dtkCoreApplication(int &argc, char **argv);
    virtual ~dtkCoreApplication(void);

public:
    virtual void initialize(void);

public:
    static dtkCoreApplication *create(int &argc, char *argv[])
    {
        return new dtkCoreApplication(argc, argv);
    }

public:
    QCommandLineParser *parser(void);
    QSettings *settings(void);

protected:
    dtkApplicationPrivate *d;
};
