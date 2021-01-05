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

#include <dtkFonts/dtkFontAwesome>

#include <QtWidgets/QFrame>

class dtkWidgetsHUDInfo;
class dtkWidgetsHUDItem;

class DTKWIDGETS_EXPORT dtkWidgetsHUD : public QFrame
{
public:
    dtkWidgetsHUD(QWidget *parent = nullptr);
    ~dtkWidgetsHUD(void);

public:
    dtkWidgetsHUDInfo *addInfo(const QString &text);
    dtkWidgetsHUDItem *addItem(fa::icon icon);

public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    class dtkWidgetsHUDPrivate *d;
};

//
// dtkWidgetsHUD.h ends here
