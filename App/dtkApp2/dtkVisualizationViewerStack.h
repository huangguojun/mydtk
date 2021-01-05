
#pragma once

#include <QtWidgets/QStackedWidget>

class dtkWidgetsLayout;
class dtkWidgetsWidget;

class dtkVisualizationViewerStack : public QStackedWidget
{
    Q_OBJECT

public:
    dtkVisualizationViewerStack(QWidget *parent = nullptr);
    ~dtkVisualizationViewerStack(void);

public:
    dtkWidgetsLayout *layout(void);

public:
    static dtkWidgetsWidget *create(const QString &name);

protected:
    void keyPressEvent(QKeyEvent *);

private:
    void importDataSet(const QString &);

private:
    class dtkVisualizationViewerStackPrivate *d;
};

//
// dtkVisualizationViewerStack.h ends here
