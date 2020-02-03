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

class DTKWIDGETS_EXPORT dtkWidgetsParameterIntSlider : public dtkWidgetsParameterBase<dtk::d_int>
{
    Q_OBJECT

public:
     dtkWidgetsParameterIntSlider(QWidget* parent = nullptr);
    ~dtkWidgetsParameterIntSlider(void);

public:
    bool connect(dtkCoreParameter *) override;

private:
    using dtkWidgetsParameterBase<dtk::d_int>::m_parameter;

    class dtkWidgetsParameterIntSliderPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterIntSliderCreator(void)
{
    return new dtkWidgetsParameterIntSlider();
}
//
// dtkWidgetsParameterIntSlider.h ends here
