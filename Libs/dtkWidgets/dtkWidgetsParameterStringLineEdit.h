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

class DTKWIDGETS_EXPORT dtkWidgetsParameterStringLineEdit
    : public dtkWidgetsParameterBase<dtk::d_string>
{
    Q_OBJECT

public:
    dtkWidgetsParameterStringLineEdit(QWidget *parent = 0);
    ~dtkWidgetsParameterStringLineEdit(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_string>::m_parameter;

    class dtkWidgetsParameterStringLineEditPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterStringLineEditCreator(void)
{
    return new dtkWidgetsParameterStringLineEdit();
}
//
// dtkWidgetsParameterStringLineEdit.h ends here
