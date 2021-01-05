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

class dtkThemesWidgetsMainWindow;
class QSize;

class DTKTHEMESWIDGETS_EXPORT dtkThemesWidgetsMainWindowButton : public QFrame
{
    Q_OBJECT

public:
    enum Kind { Maximize, Minimize, Close };

public:
    dtkThemesWidgetsMainWindowButton(Kind kind, QWidget *parent = nullptr);
    ~dtkThemesWidgetsMainWindowButton(void);

public:
    QSize sizeHint(void) const override;

protected:
    void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    class dtkThemesWidgetsMainWindowButtonPrivate *d;
};

//
// dtkThemesWidgetsMainWindowButton.h ends here
