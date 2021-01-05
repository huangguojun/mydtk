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

#include <dtkWidgets/dtkWidgetsWidget>

class DTKVISUALIZATION_EXPORT dtkVisualizationViewVideoGL : public dtkWidgetsWidget
{
    Q_OBJECT

public:
    enum Version { GLSL120, GLSL150 };

public:
    dtkVisualizationViewVideoGL(QWidget *parent = nullptr);
    ~dtkVisualizationViewVideoGL(void);

public:
    QWidget *widget(void) override;

public slots:
    void update(void) override;

public slots:
    void setImage(const QImage &);
    void setTitle(const QString &);

public:
    float ratioBuffer(void) const;
    float ratioWidget(void) const;

public:
    void setVersion(int major, int minor, bool debug = false);

private:
    Version version = GLSL120;

private:
    class dtkVisualizationViewVideoGLPrivate *d;

private:
    friend class dtkVisualizationViewVideoGLPrivate;
};

//
// dtkVisualizationViewVideoGL.h ends here
