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

class DTKVISUALIZATION_EXPORT dtkVisualizationViewVideo : public dtkWidgetsWidget
{
    Q_OBJECT

public:
    dtkVisualizationViewVideo(QWidget *parent = nullptr);
    ~dtkVisualizationViewVideo(void);

public:
    QWidget *widget(void) override;

public slots:
    void update(void) override;

public:
    void setTitle(const QString &);

public slots:
    void setImage(const QImage &);

private:
    class dtkVisualizationViewVideoPrivate *d;
};

//
// dtkVisualizationViewVideo.h ends here
