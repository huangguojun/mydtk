// Version: $Id: 79d9c81d98253f9de301a7950f40f65604c8a955 $
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

#include <dtkWidgetsExport.h>
#include <QEvent>

class dtkNotificationEvent;
// /////////////////////////////////////////////////////////////////
// Helper enumerations
// /////////////////////////////////////////////////////////////////

static const int dtkNotificationEventType = QEvent::User + 1003;

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

DTKWIDGETS_EXPORT void dtkNotify(const QString& message);
DTKWIDGETS_EXPORT void dtkNotify(const QString& message, int duration);
DTKWIDGETS_EXPORT void dtkNotify(dtkNotificationEvent *event);
DTKWIDGETS_EXPORT void dtkClearNotifications(void);

//
// dtkNotification.h ends here
