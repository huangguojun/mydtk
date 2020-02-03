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

class DTKWIDGETS_EXPORT dtkWidgetsHUDItem : public QFrame
{
    Q_OBJECT

public:
     dtkWidgetsHUDItem(fa::icon icon, QWidget *parent = nullptr);
    ~dtkWidgetsHUDItem(void);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    class dtkWidgetsHUDItemPrivate *d;
};

//
// dtkWidgetsHUDItem.h ends here
