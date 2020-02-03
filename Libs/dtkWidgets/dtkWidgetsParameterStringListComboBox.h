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

class  DTKWIDGETS_EXPORT dtkWidgetsParameterStringListComboBox : public dtkWidgetsParameterBase<dtk::d_inliststring>
{
    Q_OBJECT

public:
     dtkWidgetsParameterStringListComboBox(QWidget* parent = 0);
    ~dtkWidgetsParameterStringListComboBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_inliststring>::m_parameter;

    class dtkWidgetsParameterStringListComboBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterStringListComboBoxCreator(void)
{
    return new dtkWidgetsParameterStringListComboBox();
}
//
// dtkWidgetsParameterStringListComboBox.h ends here
