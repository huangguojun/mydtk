// Version: $Id: 9164bcd98e5f279038ceb11edd36ae2fc4a1e2f4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkNotification.h"
#include "dtkNotificationEvent.h"
#include "dtkNotificationQueue.h"

#include <QtWidgets>

void dtkNotify(const QString &message)
{
    dtkNotificationEvent *event = new dtkNotificationEvent;
    event->setType(dtkNotificationEvent::Persistent);
    event->setMessage(message);

    dtkNotify(event);
}

void dtkNotify(const QString &message, int duration)
{
    dtkNotificationEvent *event = new dtkNotificationEvent;
    event->setType(dtkNotificationEvent::NonPersistent);
    event->setMessage(message);
    event->setDuration(duration);

    dtkNotify(event);
}

void dtkNotify(dtkNotificationEvent *event)
{
    QApplication::postEvent(dtkNotificationQueue::instance(), event);
}

void dtkClearNotifications(void)
{
    dtkNotificationQueue::instance()->clear();
}

//
// dtkNotification.cpp ends here
