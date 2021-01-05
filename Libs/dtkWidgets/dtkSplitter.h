// Version: $Id: a4d11ed039774d4fcec66c9d091b5ba8a254d9d8 $
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

#include <QtWidgets>

#include <dtkWidgetsExport.h>

class DTKWIDGETS_EXPORT dtkSplitter : public QSplitter
{
    Q_OBJECT

public:
    dtkSplitter(QWidget *parent = 0, bool slim = true);
    ~dtkSplitter(void);

public:
    QSplitterHandle *createHandle(void);

private:
    class dtkSplitterPrivate *d;
};

//
// dtkSplitter.h ends here
