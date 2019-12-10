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

#include "dtkIoSettings.h"

dtkIoSettings::dtkIoSettings(void) : QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "dtk-io")
{
    this->beginGroup("io");
    if(!this->allKeys().contains("plugins"))
        this->setValue("plugins", QString());
    this->sync();
    this->endGroup();
}

dtkIoSettings::~dtkIoSettings(void)
{

}

//
// dtkIosettings.cpp ends here
