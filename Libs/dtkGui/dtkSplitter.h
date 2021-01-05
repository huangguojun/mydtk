// Version: $Id: e4d2e9145f90e9bc8a59d397c4ac9c684ffbbb8f $
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

#include <QSplitter>

#include <dtkCoreSupport/dtkGlobal.h>

#include <dtkGuiSupportExport.h>

class dtkSplitterPrivate;

class DTKGUISUPPORT_EXPORT dtkSplitter : public QSplitter
{
    Q_OBJECT

public:
    dtkSplitter(QWidget *parent = 0, bool slim = true);
    ~dtkSplitter(void);

    QSplitterHandle *createHandle(void);

private:
    dtkSplitterPrivate *d;
};

//
// dtkSplitter.h ends here
