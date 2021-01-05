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

#include "dtkApplicationPrivate.h"
#include <dtkLog>

dtkApplicationPrivate::dtkApplicationPrivate(void)
{
    settings = NULL;
    app = NULL;
    parser = new QCommandLineParser;
    parser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
}

dtkApplicationPrivate::~dtkApplicationPrivate(void)
{
    if (settings)
        delete settings;
}

void dtkApplicationPrivate::setApplication(QCoreApplication *q)
{
    app = q;
}

void dtkApplicationPrivate::initialize(void)
{
    // unset QT_PLUGIN_PATH, otherwise, kde plugins can change the qmetatype ids
    qputenv("QT_PLUGIN_PATH", "1");
    qputenv("LC_ALL", "C");
    QLocale::setDefault(QLocale::c());

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    // to avoid troubles with floats separators ('.' and not ',')
    setlocale(LC_NUMERIC, "C");
#endif

    parser->addHelpOption();
    parser->addVersionOption();

    QCommandLineOption settingsOption("settings", "main settings file", "filename");
    parser->addOption(settingsOption);

    QCommandLineOption verboseOption(
            "verbose", QCoreApplication::translate("main", "verbose plugin initialization"));
    parser->addOption(verboseOption);

    QCommandLineOption nonguiOption(
            QStringList() << "nw"
                          << "no-window",
            QCoreApplication::translate("main", "non GUI application (no window)"));
    parser->addOption(nonguiOption);

    QString verbosity = "info";
    QCommandLineOption loglevelOption("loglevel",
                                      "log level used by dtkLog (default is info), available: "
                                      "trace|debug|info|warn|error|fatal",
                                      "level", verbosity);
    parser->addOption(loglevelOption);

    QCommandLineOption logfileOption(
            "logfile",
            qPrintable(QString("log file used by dtkLog; default is: ").append(dtkLogPath(app))),
            "filename | console", dtkLogPath(app));
    parser->addOption(logfileOption);

    QCommandLineOption logfileMaxSizeOption(
            "logfilemax", "log file max size  (in MB); default is: 3072 (3GB)", "size");
    parser->addOption(logfileMaxSizeOption);

    parser->process(*app);

    if (parser->isSet(settingsOption)) {
        settings = new QSettings(parser->value(settingsOption), QSettings::IniFormat);
    } else {
        settings = new QSettings(app->organizationName(), app->applicationName());
    }

    if (settings->contains("log_level")) {
        dtkLogger::instance().setLevel(settings->value("log_level").toString());
    } else {
        if (parser->isSet(loglevelOption)) {
            verbosity = parser->value(loglevelOption);
        }

        dtkLogger::instance().setLevel(verbosity);
    }

    qlonglong max_size = 1024L * 1024L * 1024L;

    if (parser->isSet(logfileMaxSizeOption)) {
        max_size = parser->value(logfileMaxSizeOption).toLongLong() * 1024 * 1024;
    }

    if (parser->isSet(logfileOption)) {
        if (parser->value(logfileOption) == "console") {
            dtkLogger::instance().attachConsole();
        } else {
            dtkLogger::instance().attachFile(parser->value(logfileOption), max_size);
        }
    } else {
        dtkLogger::instance().attachFile(dtkLogPath(app), max_size);
    }
}
