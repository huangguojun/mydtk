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

class DTKWIDGETS_EXPORT dtkWidgetsParameterBoolCheckBox : public dtkWidgetsParameterBase<dtk::d_bool>
{
    Q_OBJECT

public:
     dtkWidgetsParameterBoolCheckBox(QWidget* parent = 0);
    ~dtkWidgetsParameterBoolCheckBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_bool>::m_parameter;

    class dtkWidgetsParameterBoolCheckBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterBoolCheckBoxCreator(void)
{
    return new dtkWidgetsParameterBoolCheckBox();
}
//
// dtkWidgetsParameterBoolCheckBox.h ends here
