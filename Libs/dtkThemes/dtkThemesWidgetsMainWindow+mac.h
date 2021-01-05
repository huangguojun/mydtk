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

#include <dtkThemesWidgetsExport>

#include <QtWidgets>

class QObject;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKTHEMESWIDGETS_EXPORT dtkThemesWidgetsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    dtkThemesWidgetsMainWindow(QWidget *parent = 0);
    ~dtkThemesWidgetsMainWindow(void);

public:
    void setDraggableAreaHeight(int height = 0);

public slots:
    void setCentralWidget(QWidget *);
    void touch(QWidget *widget = nullptr);

public:
    QWidget *titlebar(void);

public:
    void setCloseButtonVisible(bool = true);
    void setMinButtonVisible(bool = true);
    void setZoomButtonVisible(bool = true);

protected:
    void initialize(void);

protected:
    bool eventFilter(QObject *, QEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    class dtkThemesWidgetsMainWindowPrivate *d;
};

//
// dtkThemesWidgetsMainWindow+mac.h ends here
