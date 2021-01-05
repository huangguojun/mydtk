/* dtkApplicationPrivate.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2016 - Nicolas Niclausse, Inria.
 * Created: 2016/04/25 08:53:17
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkCoreExport.h>

#include <QtCore>

class DTKCORE_EXPORT dtkApplicationPrivate
{
public:
    dtkApplicationPrivate(void);
    virtual ~dtkApplicationPrivate(void);

public:
    QSettings *settings;
    QCommandLineParser *parser;
    QCoreApplication *app;

public:
    virtual void initialize(void);

    void setApplication(QCoreApplication *app);
};
