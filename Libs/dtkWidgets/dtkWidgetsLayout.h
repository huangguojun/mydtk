// Version: $Id: 185593ebdaf6b60f083a43cfae897be4f82c504e $
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

class dtkWidgetsLayoutItem;
class dtkWidgetsLayoutPrivate;
class dtkWidgetsWidget;
class QString;

class DTKWIDGETS_EXPORT dtkWidgetsLayout : public QFrame
{
    Q_OBJECT

public:
    typedef dtkWidgetsWidget *(*creator)(const QString& name);

public:
     dtkWidgetsLayout(QWidget *parent = 0);
    ~dtkWidgetsLayout(void);

signals:
    void   focused(dtkWidgetsWidget *view);
    void unfocused(dtkWidgetsWidget *view);

public:
    dtkWidgetsLayoutItem *root(void);
    dtkWidgetsLayoutItem *current(void);

public:
    void setCreator(creator creator);
    void setCurrent(dtkWidgetsLayoutItem *item);

public slots:
    void clear(void);

public slots:
    void create(void);
    void create(const QString&);

protected:
    void keyPressEvent(QKeyEvent *);

private:
    dtkWidgetsLayoutPrivate *d;
};

//
// dtkWidgetsLayout.h ends here
