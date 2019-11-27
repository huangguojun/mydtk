// Version: $Id: a3997e6b1600ea62b311cddb3f0a5c2e90d61b5b $
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

#include <dtkWidgetsExport.h>
#include <dtkLog>

#include <QtCore>
#include <QtWidgets>

class dtkApplicationPrivate;

class DTKWIDGETS_EXPORT dtkApplication: public QApplication
{
public:
             dtkApplication(int& argc, char **argv);
    virtual ~dtkApplication(void);

public:
    virtual void initialize(void);

public:
    virtual bool noGui(void);

public:
    static dtkApplication *create(int& argc, char *argv[]) {
        for (int i = 0; i < argc; i++) {
            if (!qstrcmp(argv[i], "-nw") || !qstrcmp(argv[i], "--nw") ||  !qstrcmp(argv[i], "-no-window") || !qstrcmp(argv[i], "--no-window") || !qstrcmp(argv[i], "-h") || !qstrcmp(argv[i], "--help") || !qstrcmp(argv[i], "--version")) {
                qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("minimal"));
            }
        }

        return new dtkApplication(argc, argv);
    }

public:
    QCommandLineParser  *parser(void);
    QSettings         *settings(void);

protected:
    dtkApplicationPrivate *d;
};

//
// dtkApplication.h ends here
