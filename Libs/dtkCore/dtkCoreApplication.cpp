/* dtkCoreApplication.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2015 - Nicolas Niclausse, Inria.
 * Created: 2015/02/04 12:03:44
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkCoreApplication.h"
#include "dtkApplicationPrivate.h"

/*!
  \class dtkCoreApplication
  \inmodule dtkCore
  \brief This class is used by non-GUI applications to provide their event loop. It is based on QCoreApplication, and adds a few command line options to setup settings and dtkLog. For GUI applications, see dtkApplication in dtkWidgets.

  \code
   int main(int argc, char *argv[])
   {
    dtkCoreApplication application(argc, argv);

    application.setApplicationName("myApp");
    application.setApplicationVersion("1.0.0");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");

    QCommandLineParser *parser = application.parser();
    parser->setApplicationDescription("my first DTK based application.");

    QCommandLineOption myOption("myopt", "enable myopt");
    parser->addOption(myOption);

    application.initialize();

    if (!parser->isSet(myOption)) {
       // do something
    }

    return application.exec();
   }
   \endcode

   If you run this application, --help gives you:

   \code
Usage: ./bin/myApp [options]
my first DTK based application.

Options:
  --myopt                         enable myopt
  -h, --help                      Displays this help.
  -v, --version                   Displays version information.
  --settings <filename>           main settings file
  --verbose                       verbose plugin initialization
  --nw, --no-window               non GUI application (no window)
  --loglevel <level>              log level used by dtkLog (default is info),
                                  available: trace|debug|info|warn|error|fatal
  --logfile <filename | console>  log file used by dtkLog; default is:
                                  /user/nniclaus/home/.local/share/inria/myApp/m
                                  yApp.log
  --logfilemax <size>             log file max size  (in MB); default is: 3072
                                  (3GB)
\endcode

*/

/*! \fn dtkCoreApplication::dtkCoreApplication(int &argc, char **argv)

Constructs a dtkCoreApplication. Core applications are applications without a graphical user interface. Such applications are used at the console or as server processes.

The argc and argv arguments are processed by the application, and made available in a more convenient form by the arguments() function.

Warning: The data referred to by argc and argv must stay valid for the entire lifetime of the dtkCoreApplication object. In addition, argc must be greater than zero and argv must contain at least one valid character string.
*/

dtkCoreApplication::dtkCoreApplication(int& argc, char **argv): QCoreApplication(argc, argv)
{
    d = new dtkApplicationPrivate;
    d->setApplication(this);
}

/*! \fn dtkCoreApplication::~dtkCoreApplication(void)

Destroys the dtkCoreApplication object.

*/

dtkCoreApplication::~dtkCoreApplication(void)
{
    delete d;
    d = NULL;
}

/*! \fn Qsettings dtkCoreApplication::settings(void)

  Return the main QSettings.

*/

QSettings *dtkCoreApplication::settings(void)
{
    return d->settings;
}

/*! \fn QCommandLineParser *dtkCoreApplication::parser(void)

  Return the main QCommandLineParser used by the application. It can be used to app specific options for your application.

*/

QCommandLineParser *dtkCoreApplication::parser(void)
{
    return d->parser;
}

/*! \fn void dtkCoreApplication::initialize(void)

 Initialize the command line parser. Should be called once all the specific options of your application are added in the parser.

 \sa parser
*/

void dtkCoreApplication::initialize(void)
{
    d->initialize();
}
