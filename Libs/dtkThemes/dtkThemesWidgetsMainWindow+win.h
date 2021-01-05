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

#include <QtCore>
#include <QtWidgets>

class DTKTHEMESWIDGETS_EXPORT dtkThemesWidgetsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    dtkThemesWidgetsMainWindow(QWidget *parent = 0);
    ~dtkThemesWidgetsMainWindow(void);

public:
    void setCentralWidget(QWidget *widget);
    void touch(QWidget *widget = nullptr);

public:
    QWidget *titlebar(void);

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
// dtkThemesWidgetsMainWindow+win.h ends here
