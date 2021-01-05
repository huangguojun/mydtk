/* @(#)dtkDistributedGuiApplication.h ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2016 - Nicolas Niclausse, Inria.
 * Created: 2016/04/25 13:04:29
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QApplication>
#include <QtCore>

class dtkDistributedCommunicator;
class dtkDistributedPolicy;
class dtkDistributedApplicationPrivate;

class DTKWIDGETS_EXPORT dtkDistributedGuiApplication : public QApplication
{
public:
    dtkDistributedGuiApplication(int &argc, char **argv);
    virtual ~dtkDistributedGuiApplication(void);

public:
    virtual void initialize(void);
    virtual void exec(QRunnable *task);
    virtual void spawn(QMap<QString, QString> options = QMap<QString, QString>());
    virtual void unspawn(void);

public:
    virtual bool noGui(void);

public:
    QCommandLineParser *parser(void);

public:
    static dtkDistributedGuiApplication *create(int &argc, char *argv[])
    {
        for (int i = 0; i < argc; i++)
            if (!qstrcmp(argv[i], "-nw") || !qstrcmp(argv[i], "--nw")
                || !qstrcmp(argv[i], "-no-window") || !qstrcmp(argv[i], "--no-window")
                || !qstrcmp(argv[i], "-h") || !qstrcmp(argv[i], "--help")
                || !qstrcmp(argv[i], "--version")) {
                qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("minimal"));
            }

        return new dtkDistributedGuiApplication(argc, argv);
    }

public:
    bool isMaster(void);
    dtkDistributedCommunicator *communicator(void);
    dtkDistributedPolicy *policy(void);

private:
    dtkDistributedApplicationPrivate *d;
};
