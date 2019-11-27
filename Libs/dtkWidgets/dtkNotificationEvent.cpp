// Version: $Id: ca357594ba49953a79c782f47e74827fa629dd1a $
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

class dtkNotificationEventPrivate
{
public:
    dtkNotificationEvent::Type type;
    QString message;
    int duration;
};

dtkNotificationEvent::dtkNotificationEvent(void) : QEvent((QEvent::Type)dtkNotificationEventType), d(new dtkNotificationEventPrivate)
{

}

dtkNotificationEvent::dtkNotificationEvent(const dtkNotificationEvent& other) : QEvent(QEvent::User), d(new dtkNotificationEventPrivate)
{
    d->type = other.d->type;
    d->message = other.d->message;
    d->duration = other.d->duration;
}

dtkNotificationEvent::~dtkNotificationEvent(void)
{
    delete d;

    d = NULL;
}

dtkNotificationEvent& dtkNotificationEvent::operator=(const dtkNotificationEvent& other)
{
    d->type = other.d->type;
    d->message = other.d->message;
    d->duration = other.d->duration;

    return (*this);
}

dtkNotificationEvent::Type dtkNotificationEvent::type(void)
{
    return d->type;
}

QString dtkNotificationEvent::message(void) const
{
    return d->message;
}

int dtkNotificationEvent::duration(void) const
{
    return d->duration;
}

void dtkNotificationEvent::setType(dtkNotificationEvent::Type type)
{
    d->type = type;
}

void dtkNotificationEvent::setMessage(const QString& message)
{
    d->message = message;
}

void dtkNotificationEvent::setDuration(int ms)
{
    d->duration = ms;
}

//
// dtkNotificationEvent.cpp ends here
