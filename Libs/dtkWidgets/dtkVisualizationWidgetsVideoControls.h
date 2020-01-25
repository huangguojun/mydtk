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

#include <QtWidgets/QFrame>

class DTKWIDGETS_EXPORT dtkVisualizationWidgetsVideoControls : public QFrame
{
    Q_OBJECT

public:
    enum State {
        StoppedState = 0,
        PlayingState = 1,
        PausedState  = 2
    };

public:
     dtkVisualizationWidgetsVideoControls(QWidget *parent = nullptr);
    ~dtkVisualizationWidgetsVideoControls(void);

public:
    State state(void) const;

public slots:
    void reset(void);
    void toggle(void);

 public:
    void setBufferVisible(bool);

signals:
    void pause(void);
    void play(void);
    void restart(void);
    void seekTo(int);
    void fullscreen(bool);

public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void setCurrentFrame(qlonglong frame_id);
    void setCurrentBufferFrame(qlonglong frame_id);
    void setFrameNumber(qlonglong frame_count);

public slots:
    void setDuration(qlonglong duration);
    void setPosition(qlonglong position);
    void setBufferPosition(qlonglong position);

private:
    class dtkVisualizationWidgetsVideoControlsPrivate *d;
};

//
// dtkVisualizationWidgetsVideoControls.h ends here
