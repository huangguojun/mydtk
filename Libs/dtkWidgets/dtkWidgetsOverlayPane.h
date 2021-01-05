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

#include <functional>

class DTKWIDGETS_EXPORT dtkWidgetsOverlayPane : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

public:
    dtkWidgetsOverlayPane(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPane(void);

signals:
    void entered(void);
    void left(void);

signals:
    void widthChanged(void);

public:
    int width(void);

public:
    void setWidth(int);
    void setBound(int);

public slots:
    void toggle(void);
    void toggle(std::function<void(void)> &);

public slots:
    void deactivate(void);

public:
    void addLayout(QLayout *);

public:
    void addWidget(QWidget *);

public slots:
    void clear(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    class dtkWidgetsOverlayPanePrivate *d;

private:
    friend class dtkWidgetsHUDItem;
};

//
// dtkWidgetsOverlayPane.h ends here
