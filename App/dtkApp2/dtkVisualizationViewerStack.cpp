
#include "dtkVisualizationViewerStack.h"

#include <dtkVisualization>
#include <dtkWidgets>

class dtkVisualizationViewerStackPrivate
{
public:
    dtkWidgetsLayout *layout;
};

dtkVisualizationViewerStack::dtkVisualizationViewerStack(QWidget *parent)
    : QStackedWidget(parent), d(new dtkVisualizationViewerStackPrivate)
{
    d->layout = new dtkWidgetsLayout(this);
    d->layout->setCreator(create);
    this->addWidget(d->layout);
}

dtkVisualizationViewerStack::~dtkVisualizationViewerStack(void)
{
    delete d;
}

dtkWidgetsLayout *dtkVisualizationViewerStack::layout(void)
{
    return d->layout;
}

dtkWidgetsWidget *dtkVisualizationViewerStack::create(const QString &type)
{

    dtkWidgetsWidget *view = Q_NULLPTR;

    if (type == "Plot2D") {
        view = new dtkVisualizationPlot2D;
    } else if (type == "Plot3D") {
        view = new dtkVisualizationPlot3D;
    } else if (type == "View2D") {
        view = new dtkVisualizationView2D;
    } else if (type == "View3D") {
        view = new dtkVisualizationView3D;
    } else if (type == "ViewVideoPlayer") {
        view = new dtkVisualizationViewVideoPlayer;
    }

    /*
    dtkWidgetsMenuBar *menu_bar = dtkApp->window()->menubar();


    dtkWidgetsMenu *menu = nullptr;
    menu = menu_bar->menu("View " + view->objectName());

    if( menu) {
        if(type != "Plot2D") {
            menu->clear(); // dont clear plot 2d menu
        }
        menu_bar->touch();
    } else {
        menu_bar->addMenu(fa::desktop, "View " + view->objectName());
        menu_bar->touch();
    }
    */

    // connect(view, &dtkWidgetsWidget::focused, [=] () {
    //         qDebug() << "focus :" << view->objectName();
    //     });

    // connect(view, &dtkWidgetsWidget::unfocused, [=] () {
    //         qDebug() << "unfocus :" << view->objectName();
    //     });

    return view;
}

void dtkVisualizationViewerStack::keyPressEvent(QKeyEvent *event)
{
    event->ignore();

    QStackedWidget::keyPressEvent(event);
}

//
// dtkVisualizationViewerStack.cpp ends here
