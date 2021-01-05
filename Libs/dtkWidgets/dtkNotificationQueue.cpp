// Version: $Id: 51820d39ea4e12928af095b4d80d2da9029093b8 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkNotificationQueue.h"
#include "dtkNotifiable.h"
#include "dtkNotification.h"
#include "dtkNotificationEvent.h"

#define DTK_NOTIFICATION_PERSISTENT_DURATION 10000

class dtkNotificationQueuePrivate
{
public:
    QQueue<dtkNotificationEvent> persistent;
    QQueue<dtkNotificationEvent> non_persistent;

public:
    QList<dtkNotifiable *> notifiables;

public:
    QTimer persistent_timer;
    QTimer non_persistent_timer;
};

dtkNotificationQueue *dtkNotificationQueue::instance(void)
{
    if (!s_instance)
        s_instance = new dtkNotificationQueue;

    return s_instance;
}

void dtkNotificationQueue::registerNotifiable(dtkNotifiable *notifiable)
{
    d->notifiables << notifiable;
}

void dtkNotificationQueue::clear(void)
{
    d->persistent.clear();

    d->persistent_timer.stop();

    this->idle();
}

void dtkNotificationQueue::next(void)
{
    d->persistent_timer.stop();

    this->idle();
}

void dtkNotificationQueue::previous(void)
{
    d->persistent_timer.stop();

    for (int i = 0; i < d->persistent.count() - 2; i++)
        d->persistent.enqueue(d->persistent.dequeue());

    this->idle();
}

void dtkNotificationQueue::dismiss(void)
{
    d->persistent_timer.stop();

    d->non_persistent_timer.stop();

    this->idle();
}

void dtkNotificationQueue::push(dtkNotificationEvent *event)
{
    dtkNotificationEvent e = (*event);

    switch (event->type()) {
    case dtkNotificationEvent::Persistent:
        d->persistent.enqueue(dtkNotificationEvent(e));
        this->idle();
        break;

    case dtkNotificationEvent::NonPersistent:
        d->non_persistent.enqueue(dtkNotificationEvent(e));
        d->persistent_timer.stop();
        this->idle();
        break;
    }
}

void dtkNotificationQueue::idle(void)
{
    foreach (dtkNotifiable *notifiable, d->notifiables) {
        notifiable->setPersistentCount(d->persistent.count());
        notifiable->setNonPersistentCount(d->non_persistent.count());
    }

    if (d->non_persistent_timer.isActive())
        return;

    if (!d->non_persistent.isEmpty()) {

        foreach (dtkNotifiable *notifiable, d->notifiables) {
            notifiable->clear();
            notifiable->dismissible(false);
        }

        dtkNotificationEvent event = d->non_persistent.dequeue();

        foreach (dtkNotifiable *notifiable, d->notifiables) {
            notifiable->display(event.message());
            notifiable->setNonPersistentCount(d->non_persistent.count());
            notifiable->dismissible(true);
        }

        d->non_persistent_timer.start(event.duration());

        return;
    }

    if (d->persistent_timer.isActive())
        return;

    if (!d->persistent.isEmpty()) {

        foreach (dtkNotifiable *notifiable, d->notifiables) {
            notifiable->clear();
            notifiable->dismissible(false);
        }

        dtkNotificationEvent event = d->persistent.dequeue();

        foreach (dtkNotifiable *notifiable, d->notifiables)
            notifiable->display(event.message());

        d->persistent.enqueue(event);

        d->persistent_timer.start(DTK_NOTIFICATION_PERSISTENT_DURATION);

        return;
    }

    if (d->persistent.isEmpty() && d->non_persistent.isEmpty()) {

        foreach (dtkNotifiable *notifiable, d->notifiables) {
            notifiable->clear();
            notifiable->dismissible(false);
        }

        return;
    }
}

bool dtkNotificationQueue::event(QEvent *event)
{
    if (event->type() != dtkNotificationEventType)
        return QObject::event(event);

    this->push(static_cast<dtkNotificationEvent *>(event));

    return true;
}

dtkNotificationQueue::dtkNotificationQueue(QObject *parent)
    : QObject(parent), d(new dtkNotificationQueuePrivate)
{
    d->persistent_timer.setSingleShot(true);
    d->non_persistent_timer.setSingleShot(true);

    connect(&(d->persistent_timer), SIGNAL(timeout()), this, SLOT(idle()));
    connect(&(d->non_persistent_timer), SIGNAL(timeout()), this, SLOT(idle()));
}

dtkNotificationQueue::~dtkNotificationQueue(void)
{
    d->persistent.clear();
    d->non_persistent.clear();

    delete d;

    d = NULL;
}

dtkNotificationQueue *dtkNotificationQueue::s_instance = NULL;

//
// dtkNotificationQueue.cpp ends here
