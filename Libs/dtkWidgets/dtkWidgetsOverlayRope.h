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

class DTKWIDGETS_EXPORT dtkWidgetsOverlayRope : public QFrame
{
    Q_OBJECT

public:
    enum Mode {
        Dragging = 0x00,
        Collider = 0x01,
         Dynamic = 0x10
    };

public:
     dtkWidgetsOverlayRope(QWidget *parent = Q_NULLPTR);
    ~dtkWidgetsOverlayRope(void);

public slots:
    void toggle(void);

signals:
    void source(QWidget *);
    void target(QWidget *);

public:
    void setMode(Mode);

public:
    void setVerticesCount(quint32 count);
    void setVerticesInterval(quint32 distance);

public:
    void setIntegrationFactor(qreal factor);

protected:
    void paintEvent(QPaintEvent *event);

protected:
    void resizeEvent(QResizeEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    class dtkWidgetsOverlayRopePrivate *d;
};

//
// dtkWidgetsOverlayRope.h ends here
