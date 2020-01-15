// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <QtGui>

#include <QVTKWidget.h>
//#include <vtkGenericOpenGLRenderWindow.h>

#include <dtkCore>
//#include <dtkCoreRuntime>

//#include <dtkThemes/dtkThemesEngine>

#include <dtkWidgets>

#include "dtkVisualizationViewer.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    //dtk::core::registerParameters();
    //dtk::widgets::initialize();
 //   dtkThemesEngine::instance()->apply();

   // QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
  //  vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
  //  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("dtkVisualizationViewer");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");

    dtkWidgetsLayoutItem::Actions actions;
  
    actions.insert("Plot2D", "Plot 2D");
    actions.insert("Plot3D", "Plot 3D");
    actions.insert("View2D", "View 2D");
    actions.insert("View3D", "View 3D");
    actions.insert("ViewVideoPlayer", "Video player");

    dtkWidgetsLayoutItem::setActions(actions);

    
    dtkVisualizationViewer *viewer = new dtkVisualizationViewer;

    //application->setWindow(viewer);
    viewer->show();

    application->initialize();

    int status = application->exec();

   // dtkWidgetsController::instance()->clear();

    return status;
}

// ///////////////////////////////////////////////////////////////////

//
// main.cpp ends here
