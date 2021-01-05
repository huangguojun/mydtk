// Version: $Id: cf5ce8ccf82cc783af755e5f9a5e7a49c5d9c66a $
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

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkWidgetsTagCloudDesc;
class dtkWidgetsTagCloudList;
class dtkWidgetsTagCloudViewPrivate;

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudView : public QStackedWidget
{
    Q_OBJECT

public:
    enum Direction { Left2Right, Right2Left, Top2Bottom, Bottom2Top, Automatic };

public:
    dtkWidgetsTagCloudView(QWidget *parent = 0);
    ~dtkWidgetsTagCloudView(void);

public:
    dtkWidgetsTagCloudList *list(void);
    dtkWidgetsTagCloudDesc *desc(void);

public:
    void setDark(void);
    void setDoom(void);

public slots:
    void onItemClicked(const QString &description);

protected slots:
    void setSpeed(int speed);
    void setAnimation(QEasingCurve::Type type);
    void setVerticalMode(bool vertical = true);
    void setWrap(bool wrap);

protected slots:
    void slideInNext(void);
    void slideInPrev(void);
    void slideInIdx(int idx, Direction direction = Automatic);

signals:
    void animationFinished(void);

protected slots:
    void animationDoneSlot(void);

protected:
    void slideInWgt(QWidget *widget, Direction direction = Automatic);

protected:
    dtkWidgetsTagCloudViewPrivate *d;
};

//
// dtkWidgetsTagCloudView.h ends here
