/* @(#)dtkCpuid.cpp ---
 *
 * Author: Nicolas Niclausse
 * Copyright (C) 2012 - Nicolas Niclausse, Inria.
 * Created: 2012/04/25 13:18:27
 * Version: $Id: e3d5b3ddb4a6e0d567d0cb11b53bc3b99e246d05 $
 * Last-Updated: lun. févr.  3 14:21:46 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 55
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <dtkConfig.h>

#include <dtkCoreSupport/dtkGlobal.h>

#include "dtkCpuid.h"

#if defined(Q_OS_WIN) && !defined(__MINGW32__)
#    include <limits.h>
#    if defined(Q_CC_MSVC)
typedef unsigned __int32 uint32_t;
#        include <intrin.h>
#    endif
#else
#    include <stdint.h>
#endif

class dtkCpuidPrivate
{
public:
    uint32_t regs[4];
};

dtkCpuid::dtkCpuid(void) : d(new dtkCpuidPrivate) {}

dtkCpuid::~dtkCpuid(void)
{
    delete d;
}

void dtkCpuid::load(unsigned i)
{
#if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
    __cpuid((int *)d->regs, (int)i);
#elif defined(DTK_BUILD_64)
    asm volatile("cpuid"
                 : "=a"(d->regs[0]), "=b"(d->regs[1]), "=c"(d->regs[2]), "=d"(d->regs[3])
                 : "a"(i), "c"(0));
#endif
}

QString dtkCpuid::vendor(void)
{
    this->load(0); // Get CPU vendor

    QString vendor;
    vendor += QString::fromUtf8((const char *)&d->regs[1], 4);
    vendor += QString::fromUtf8((const char *)&d->regs[3], 4);
    vendor += QString::fromUtf8((const char *)&d->regs[2], 4);
    return vendor;
}
