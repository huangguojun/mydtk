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

class DTKWIDGETS_EXPORT dtkWidgetsParameterScientificSpinBox
    : public dtkWidgetsParameterBase<dtk::d_real>
{
    Q_OBJECT

public:
    dtkWidgetsParameterScientificSpinBox(QWidget *parent = nullptr);
    ~dtkWidgetsParameterScientificSpinBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_real>::m_parameter;

    class dtkWidgetsParameterScientificSpinBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterScientificSpinBoxCreator(void)
{
    return new dtkWidgetsParameterScientificSpinBox();
}
//
// dtkWidgetsParameterScientificSpinBox.h ends here
