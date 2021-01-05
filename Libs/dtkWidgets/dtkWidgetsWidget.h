// Version: $Id: 50159f08c3a859ff318f11ce2513587924cf4e46 $
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

#include <QtWidgets/QWidget>

class DTKWIDGETS_EXPORT dtkWidgetsWidget : public QWidget
{
    Q_OBJECT

public:
    dtkWidgetsWidget(QWidget *parent = nullptr);
    ~dtkWidgetsWidget(void) = default;

signals:
    void focused(void);
    void unfocused(void);

public:
    virtual QWidget *widget(void);
    virtual QWidget *inspector(void);

public slots:
    virtual void update(void);

protected:
    void mousePressEvent(QMouseEvent *);
    void showEvent(QShowEvent *);
};

//
// dtkWidgetsWidget.h ends here
