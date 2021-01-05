// Version: $Id: dd50a94e43b605ba70bbd6f0e9b982b7628a9df3 $
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

#include <QtCore>

class DTKWIDGETS_EXPORT dtkNotifiable
{
public:
    virtual void clear(void) = 0;

public:
    virtual void next(void) = 0;
    virtual void previous(void) = 0;

public:
    virtual void dismiss(void) = 0;
    virtual void dismissible(bool dismissible) = 0;

public:
    virtual void setPersistentCount(int count) = 0;
    virtual void setNonPersistentCount(int count) = 0;

public:
    virtual void display(const QString &message) = 0;
};

//
// dtkNotifiable.h ends here
