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

class DTKWIDGETS_EXPORT dtkWidgetsParameterBoolPushButton
    : public dtkWidgetsParameterBase<dtk::d_bool>
{
    Q_OBJECT

public:
    dtkWidgetsParameterBoolPushButton(QWidget *parent = 0);
    ~dtkWidgetsParameterBoolPushButton(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_bool>::m_parameter;

    class dtkWidgetsParameterBoolPushButtonPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterBoolPushButtonCreator(void)
{
    return new dtkWidgetsParameterBoolPushButton();
}
//
// dtkWidgetsParameterBoolPushButton.h ends here
