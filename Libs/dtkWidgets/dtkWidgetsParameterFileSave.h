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

class  DTKWIDGETS_EXPORT dtkWidgetsParameterFileSave : public dtkWidgetsParameterBase<dtk::d_path>
{
    Q_OBJECT

public:
     dtkWidgetsParameterFileSave(QWidget* parent = nullptr);
    ~dtkWidgetsParameterFileSave(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_path>::m_parameter;

    class dtkWidgetsParameterFileSavePrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterFileSaveCreator(void)
{
    return new dtkWidgetsParameterFileSave();
}
//
// dtkWidgetsParameterFileSave.h ends here
