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

#include "dtkWidgetsWorkspace.h"

#include "dtkWidgets.h"

dtkWidgetsWorkspace::dtkWidgetsWorkspace(QWidget *parent) : QWidget(parent) {}

namespace dtk {
namespace widgets {
DTK_DEFINE_CONCEPT(dtkWidgetsWorkspace, workspace, dtk::widgets);
}
} // namespace dtk

//
// dtkWidgetsWorkspace.cpp ends here
