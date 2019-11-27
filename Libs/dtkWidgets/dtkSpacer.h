// Version: $Id: df1ff148a1ea542ac66eff318879f243c3948634 $
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

#include <QWidget>

#include <dtkWidgetsExport.h>

class DTKWIDGETS_EXPORT dtkSpacer : public QWidget
{
    Q_OBJECT

public:
     dtkSpacer(QWidget *parent = 0, int width = 0, int height = 0);
    ~dtkSpacer(void);

public:
    QSize sizeHint (void) const;
};

//
// dtkSpacer.h ends here
