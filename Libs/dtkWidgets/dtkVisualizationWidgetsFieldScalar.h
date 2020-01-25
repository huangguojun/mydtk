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

#pragma once

#include <dtkWidgetsExport>

#include <QtWidgets/QWidget>

class vtkActor;
class vtkVolume;

class DTKWIDGETS_EXPORT dtkVisualizationWidgetsFieldScalar : public QWidget
{
    Q_OBJECT

public:
     dtkVisualizationWidgetsFieldScalar(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsFieldScalar(void);

public slots:
    void setActor(vtkActor *);
    void setVolume(vtkVolume *);

public slots:
    void update(void);

private:
    class dtkVisualizationWidgetsFieldScalarPrivate *d;
};

//
// dtkVisualizationWidgetsFieldScalar.h ends here
