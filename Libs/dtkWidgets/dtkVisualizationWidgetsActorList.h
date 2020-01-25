#pragma once

#include <dtkWidgetsExport>

#include <QtWidgets/QFrame>

class vtkRenderer;

class DTKWIDGETS_EXPORT dtkVisualizationWidgetsActorList : public QFrame
{
    Q_OBJECT

public:
     dtkVisualizationWidgetsActorList(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsActorList(void);

public slots:
    void update(void);

public:
    void setRenderer(vtkRenderer *);

private:
    class dtkVisualizationWidgetsActorListPrivate *d;
};
