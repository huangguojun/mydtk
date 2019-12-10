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

#include "dtkVisualizationViewVideoGL.h"

class dtkWidgetsHUD;
class dtkWidgetsOverlayPane;

class DTKVISUALIZATION_EXPORT dtkVisualizationViewVideoPlayer: public dtkVisualizationViewVideoGL
{
    Q_OBJECT

public:
     dtkVisualizationViewVideoPlayer(QWidget *parent = nullptr);
    ~dtkVisualizationViewVideoPlayer(void);

public slots:
    void open(void);

public:
    dtkWidgetsOverlayPane *overlay(void);
    dtkWidgetsHUD *hud(void) ;

protected:
    void dragEnterEvent(QDragEnterEvent *) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dragMoveEvent(QDragMoveEvent *) override;
    void dropEvent(QDropEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

private:
    class dtkVisualizationViewVideoPlayerPrivate *d;
};

//
// dtkVisualizationViewVideoPlayer.h ends here
