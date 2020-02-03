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

#include "dtkWidgetsOverlayPaneItem.h"

class DTKWIDGETS_EXPORT dtkWidgetsOverlayPaneSlider : public dtkWidgetsOverlayPaneItem
{
    Q_OBJECT

public:
     dtkWidgetsOverlayPaneSlider(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneSlider(void);

signals:
    void indexVisible(int);
    void finished(void);
    void inTransition(bool);

#pragma mark - Slide management

public:
    void addSlide(fa::icon, QWidget *);
    void addSlide(fa::icon, QWidget *, const QString&);

public:
    void remSlide(QWidget *);

public:
    void setBound(int);

public:
    void slideTo(int);
    void slideTo(int, std::function<void ()>&);
    void slideToNext(void);
    void slideToPrevious(void);
    void slideToPrevious(std::function<void ()>&);

public:
    void setCurrentIndex(int);
    void setCurrentIndex(int, std::function<void ()>&);

public:
    void enableSpying(bool);

public:
    void decr(void);
    void print(void);
    void reset(void);
    
public:
    bool is_in_transition = false; // this is a dirty flag for parents to check whether they can move the menu or not (if it's transitionning, we forbid that)
    void informInTransition(bool);

private:
    class dtkWidgetsOverlayPaneSliderPrivate *d;
};

//
// dtkWidgetsOverlayPaneSlider.h ends here
