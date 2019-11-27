/* @(#)dtkDistributedGuiApplication.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2016 - Nicolas Niclausse, Inria.
 * Created: 2016/04/25 13:06:26
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkDistributedGuiApplication.h"

#include <dtkDistributed/dtkDistributedApplicationPrivate.h>
#include <dtkDistributed/dtkDistributedCommunicator.h>

#include <dtkWidgets/dtkApplication.h>

#include <dtkLog>

/*!
  \class dtkDistributedGuiApplication
  \inmodule dtkWidgets
  \brief dtkDistributedGuiApplication ...

    \brief This class is used to start GUI applications that use the dtkDistributed framework. The distributed option (plugins, number of process/threads, ...) can be configured on the command line, and also setup settings and dtkLog. The rank 0 will be able to start the main loop and handle the GUI parts. It can also be used to start a non GUI appliation if the \tt{-nw} argument is given and the create method is called (it won't work if you use new directly).

    \code
int main(int argc, char **argv)
{
  dtkDistributedGuiApplication *app = dtkDistributedGuiApplication::create(argc, argv);
  app->setOrganizationName("inria");
  app->setOrganizationDomain("fr");
  app->setApplicationName("myApp");
  app->setApplicationVersion("0.1.0");

  QCommandLineParser *parser = app->parser();
  parser->setApplicationDescription("my dtk distributed app.");

  QCommandLineOption myOption("myopt", "enable myopt", "default val");
  parser->addOption(myOption);

  app->initialize();

  // ------------ check parameters
   if (!parser->isSet(myOption)) {
    qFatal("Error: input are not correctly set! you have to set : --myopt <val>") ;
    return 1;
  }

  // /////////////////////////////////////////////////////////////////
  // Launch the execution
  // ////////////////////////////////////////////////////////////////
  myRunnableClass torun;

  torun.myarg = parser->value(myOption);

  app->spawn();
  app->exec(&torun);
  app->unspawn(); // unspawn will quit the application
  return 0;
}
    \endcode

    \sa dtkDistributed

*/


/*! \fn dtkDistributedGuiApplication::dtkDistributedGuiApplication(int &argc, char **argv)

Constructs a dtkDistributedGuiApplication. Initializes the window system and constructs an application object with argc command line arguments in argv.

The argc and argv arguments are processed by the application, and made available in a more convenient form by the arguments() function.

Warning: The data referred to by \a argc and \a argv must stay valid for the entire lifetime of the dtkCoreApplication object. In addition, argc must be greater than zero and argv must contain at least one valid character string.
*/

dtkDistributedGuiApplication::dtkDistributedGuiApplication(int& argc, char **argv): QApplication(argc, argv)
{

    d = new dtkDistributedApplicationPrivate;
    d->spawned = false;
    d->nospawn = false;

}

/*! \fn dtkDistributedGuiApplication::~dtkDistributedGuiApplication(void)

Destroys the dtkDistributedGuiApplication object.

*/


/*! \fn dtkDistributedGuiApplication *create(int &argc, char *argv[])

  Helper function to create an instance of a dtkDistributedGuiApplication. If the \tt{--nw} option is set, it will use a \e{minimal} \tt{QT_QPA_PLATFORM}. \a argc and \a argv are the usual parameters of a QCoreApplication.

*/

dtkDistributedGuiApplication::~dtkDistributedGuiApplication(void)
{
    delete d;
    d = NULL;
}


/*! \fn void dtkDistributedGuiApplication::initialize(void)

 Initialize the command line parser. Should be called once all the specific options of your application are added in the parser.

 \sa parser
*/

void dtkDistributedGuiApplication::initialize(void)
{
    d->initialize();
}

/*! \fn QCommandLineParser *dtkDistributedGuiApplication::parser(void)

  Return the main QCommandLineParser used by the application. It can be used to app specific options for your application.

*/

QCommandLineParser *dtkDistributedGuiApplication::parser(void)
{
    return d->parser;
}

/*! \fn bool dtkDistributedGuiApplication::noGui(void)

  Return true if the application is not a GUI application (ie. the \tt{-nw} was used in the command line)

*/

bool dtkDistributedGuiApplication::noGui(void)
{
    return !(qApp && qobject_cast<QGuiApplication *>(qApp) && (QGuiApplication::platformName() != "minimal")) ;
}

/*! \fn void dtkDistributedGuiApplication::exec(QRunnable *task)

  execute the given QRunnable \a task on the spawned processes/threads. Must be called after spawn()

  \sa spawn
*/

void dtkDistributedGuiApplication::exec(QRunnable *task)
{
    d->policy.communicator()->exec(task);
}

/*! \fn void dtkDistributedGuiApplication::spawn (QMap <QString,QString> options)

  Spawn processes/threads on one or several hosts (depending on the implementation). This step can be bypassed if the --no-spawn argument is given. This can be useful when you are using an mpi communicator and your mpi implementation or scheduler does not handle the \a Mpi_comm_Spawn gracefully. Will you have to use an external tool to spawn the application (mpirun , srun , ...)

*/

void dtkDistributedGuiApplication::spawn(QMap<QString, QString> options)
{
    d->spawn();
}

/*! \fn void dtkDistributedGuiApplication::unspawn(void)

  unspawn processes/threads

*/

void dtkDistributedGuiApplication::unspawn(void)
{
    d->unspawn();
}

/*! \fn dtkDistributedPolicy *dtkDistributedGuiApplication::policy(void)

  Return the current policy
*/

dtkDistributedPolicy *dtkDistributedGuiApplication::policy(void)
{
    return &(d->policy);
}

/*! \fn dtkDistributedCommunicator *dtkDistributedGuiApplication::communicator(void)

  Return the main communicator.

  \sa dtkDistributedCommunicator

*/
dtkDistributedCommunicator *dtkDistributedGuiApplication::communicator(void)
{

    return d->policy.communicator();
}

/*! \fn bool dtkDistributedGuiApplication::isMaster(void)

  Return true if the calling process/thread is the master (rank 0)
*/
bool dtkDistributedGuiApplication::isMaster(void)
{
    return (d->policy.communicator()->rank() == 0);
}

