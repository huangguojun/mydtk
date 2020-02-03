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

class DTKWIDGETS_EXPORT dtkWidgetsParameterDoubleSlider : public dtkWidgetsParameterBase<dtk::d_real>
{
    Q_OBJECT

public:
     dtkWidgetsParameterDoubleSlider(QWidget* parent = nullptr);
    ~dtkWidgetsParameterDoubleSlider(void);

public:
    bool connect(dtkCoreParameter *) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    using dtkWidgetsParameterBase<dtk::d_real>::m_parameter;

    class dtkWidgetsParameterDoubleSliderPrivate *d;
};

// ///////////////////////////////////////////////////////////////////

inline dtkWidgetsParameter *dtkWidgetsParameterDoubleSliderCreator(void)
{
    return new dtkWidgetsParameterDoubleSlider();
}
//
// dtkWidgetsParameterDoubleSlider.h ends here
