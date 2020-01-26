#include <QtGui>
#include <QVTKWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <dtkCore>
//#include <dtkCoreRuntime>
#include <dtkThemes/dtkThemesEngine>
#include <dtkWidgets>
#include "dtkVisualizationViewer.h"


int main(int argc, char **argv)
{
    dtkThemesEngine::instance()->apply();
    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    dtkApplication *application = dtkApplication::create(argc, argv);
    
    dtkWidgetsLayoutItem::Actions actions;
  
    actions.insert("Plot2D", "Plot 2D");
    actions.insert("Plot3D", "Plot 3D");
    actions.insert("View2D", "View 2D");
    actions.insert("View3D", "View 3D");
    actions.insert("ViewVideoPlayer", "Video player");

    dtkWidgetsLayoutItem::setActions(actions);
    
    dtkVisualizationViewer *viewer = new dtkVisualizationViewer;
    viewer->show();
    application->initialize();
    return application->exec();
}

