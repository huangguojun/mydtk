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

#include <dtkCore/dtkCoreParameters>

#include <QtCore>

class dtkWidgetsMenu;
class dtkWidgetsMenuBar;
class dtkWidgetsMenuBarContainer;
class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterMenuBarGenerator declaration
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsParameterMenuBarGenerator
{
public:
     dtkWidgetsParameterMenuBarGenerator(const QString& menu_file, const QString& definition_file);
    ~dtkWidgetsParameterMenuBarGenerator(void);

public:
    void populate(dtkWidgetsMenu *);
    void populate(dtkWidgetsMenuBar *);
    void populate(dtkWidgetsMenuBarContainer *);

public:
    dtkCoreParameters parameters(void) const;

protected:
    class dtkWidgetsParameterMenuBarGeneratorPrivate *d;
};

//
// dtkWidgetsParameterMenuBarGenerator.h ends here
