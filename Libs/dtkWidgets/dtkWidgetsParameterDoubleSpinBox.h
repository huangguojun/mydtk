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

class DTKWIDGETS_EXPORT dtkWidgetsParameterDoubleSpinBox : public dtkWidgetsParameterBase<dtk::d_real>
{
    Q_OBJECT

public:
     dtkWidgetsParameterDoubleSpinBox(QWidget* parent = 0);
    ~dtkWidgetsParameterDoubleSpinBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_real>::m_parameter;

    class dtkWidgetsParameterDoubleSpinBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterDoubleSpinBoxCreator(void)
{
    return new dtkWidgetsParameterDoubleSpinBox();
}
//
// dtkWidgetsParameterDoubleSpinBox.h ends here
