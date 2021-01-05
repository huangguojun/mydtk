// Version: $Id:
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

#include "dtkIoExport.h"

#include <QtCore>

class DTKIO_EXPORT dtkIoSettings : public QSettings
{
public:
    dtkIoSettings(void);
    ~dtkIoSettings(void);
};

//
// dtkIoSettings.h ends here
