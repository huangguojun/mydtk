// Version: $Id: a53fa52f3dba175a8f0ad523fb21024ab7294537 $
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

#include <cassert>
#include <cstdlib>
#include <iostream>

namespace dtk {

inline void assert_yes(bool cond, const char *msg)
{
    if (!cond)
        std::cerr << msg << std::endl;
    assert(cond);
}

inline void assert_no(bool, const char *) {}
} // namespace dtk

#ifndef NDEBUG
#    define DTK_ASSERT(cond, msg)                                                                  \
        dtk::assert_yes(cond, msg);                                                                \
        assert(cond);
#else
#    define DTK_ASSERT(cond, msg) dtk::assert_no(cond, msg);
#endif

//
// dtkAssert.h ends here
