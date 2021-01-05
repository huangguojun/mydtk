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

#include "dtkWidgetsParameter.h"

#include <dtkCore/dtkCoreParameterPath>

class DTKWIDGETS_EXPORT dtkWidgetsParameterDirBrowse : public dtkWidgetsParameterBase<dtk::d_path>
{
    Q_OBJECT

public:
    dtkWidgetsParameterDirBrowse(QWidget *parent = 0);
    ~dtkWidgetsParameterDirBrowse(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_path>::m_parameter;

    class dtkWidgetsParameterDirBrowsePrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterDirBrowseCreator(void)
{
    return new dtkWidgetsParameterDirBrowse();
}
//
// dtkWidgetsParameterDirBrowse.h ends here
