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

#include <dtkThemesExport>

#include <QtWidgets>

class QObject;

class DTKTHEMES_EXPORT dtkThemesWidgetsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     dtkThemesWidgetsMainWindow(QWidget *parent = 0);
    ~dtkThemesWidgetsMainWindow(void);

public slots:
    void setCentralWidget(QWidget *);
    void touch(QWidget *widget = nullptr);

public:
    QWidget *titlebar(void);

protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *)   override;

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    class dtkThemesWidgetsMainWindowPrivate *d;
};

//
// dtkThemesWidgetsMainWindow+linux.h ends here
