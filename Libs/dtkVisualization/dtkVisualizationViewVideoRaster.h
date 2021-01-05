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

class DTKVISUALIZATION_EXPORT dtkVisualizationViewVideoRaster : public dtkWidgetsWidget
{
    Q_OBJECT

public:
    dtkVisualizationViewVideoRaster(QWidget *parent = nullptr);
    ~dtkVisualizationViewVideoRaster(void);

public:
    QWidget *widget(void) override;

public slots:
    void update(void) override;

public:
    void setTitle(const QString &);

public slots:
    void setImage(const QImage &);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    class dtkVisualizationViewVideoRasterPrivate *d;
};

//
// dtkVisualizationViewVideoRaster.h ends here
