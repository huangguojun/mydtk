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

class DTKWIDGETS_EXPORT dtkWidgetsParameterLongLongSpinBox
    : public dtkWidgetsParameterBase<dtk::d_int>
{
    Q_OBJECT

public:
    dtkWidgetsParameterLongLongSpinBox(QWidget *parent = nullptr);
    ~dtkWidgetsParameterLongLongSpinBox(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_int>::m_parameter;

    class dtkWidgetsParameterLongLongSpinBoxPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterLongLongSpinBoxCreator(void)
{
    return new dtkWidgetsParameterLongLongSpinBox();
}
//
// dtkWidgetsParameterLongLongSpinBox.h ends here
