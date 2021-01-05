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

class DTKWIDGETS_EXPORT dtkWidgetsParameterIntSpinBox : public dtkWidgetsParameterBase<dtk::d_int>
{
    Q_OBJECT

public:
    dtkWidgetsParameterIntSpinBox(QWidget *parent = 0);
    ~dtkWidgetsParameterIntSpinBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_int>::m_parameter;

    class dtkWidgetsParameterIntSpinBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterIntSpinBoxCreator(void)
{
    return new dtkWidgetsParameterIntSpinBox();
}
//
// dtkWidgetsParameterIntSpinBox.h ends here
