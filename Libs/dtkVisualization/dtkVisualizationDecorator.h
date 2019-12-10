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

#include <dtkVisualizationExport>

#include <dtkCore>

#include <QtCore/QObject>

class dtkVisualizationCanvas;
class vtkTransform;
class QWidget;

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecorator declaration
// ///////////////////////////////////////////////////////////////////

class DTKVISUALIZATION_EXPORT dtkVisualizationDecorator : public QObject
{
    Q_OBJECT

public:
     dtkVisualizationDecorator(void) = default;
    ~dtkVisualizationDecorator(void) = default;

public:
    virtual void touch(void) = 0;
    void setObjectName(const QString &);

public:
    void show(void);
    void hide(void);

protected:
    void draw(void);
    void saveSettings(const QString &, const QVariant&);

protected:
    virtual void restoreSettings(void) {};

public:
    virtual void setVisibility(bool visible) = 0;

    virtual bool isDecorating(void) = 0;

    virtual void setData(const QVariant&) = 0;

    virtual void setTransform(vtkTransform *);

    virtual void setCanvas(dtkVisualizationCanvas *) = 0;
    virtual void unsetCanvas(void) = 0;

    virtual QVariant data(void) const = 0;
    virtual dtkVisualizationCanvas *canvas(void) const = 0;

    virtual QList<QWidget *> inspectors(void) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (dtkVisualizationDecorator *)
DTK_DECLARE_PLUGIN        (dtkVisualizationDecorator, DTKVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(dtkVisualizationDecorator, DTKVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(dtkVisualizationDecorator, DTKVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to dtkVisualization layer
// /////////////////////////////////////////////////////////////////

namespace dtk { namespace visualization {
    DTK_DECLARE_CONCEPT(dtkVisualizationDecorator, DTKVISUALIZATION_EXPORT, decorator);
} }

//
// dtkVisualizationDecorator.h ends here
