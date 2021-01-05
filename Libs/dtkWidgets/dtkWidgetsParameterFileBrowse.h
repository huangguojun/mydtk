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

#include <dtkCore/dtkCoreParameter>
#include <dtkCore/dtkCoreParameterPath>

class DTKWIDGETS_EXPORT dtkWidgetsParameterFileBrowse : public dtkWidgetsParameterBase<dtk::d_path>
{
    Q_OBJECT

public:
    dtkWidgetsParameterFileBrowse(QWidget *parent = nullptr);
    ~dtkWidgetsParameterFileBrowse(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_path>::m_parameter;

    class dtkWidgetsParameterFileBrowsePrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterFileBrowseCreator(void)
{
    return new dtkWidgetsParameterFileBrowse();
}
//
// dtkWidgetsParameterFileBrowse.h ends here
