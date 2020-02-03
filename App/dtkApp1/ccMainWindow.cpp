
#include "ccMainWindow.h"

#include <dtkWidgets>
#include <dtkVisualization>

#include <QtCore>
#include <QtGui>


ccMainWindow::ccMainWindow(QWidget *parent) : QMainWindow(parent)
{
 
    dtkWidgetsWidget *view = Q_NULLPTR;
    view = new dtkVisualizationView2D;

    
}


