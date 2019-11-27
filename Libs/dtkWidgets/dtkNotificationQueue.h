// Version: $Id: 9f223099894108adae4c44a72751efd2f655185b $
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

#include <QtCore>

class dtkNotifiable;
class dtkNotificationEvent;
class dtkNotificationQueuePrivate;

class DTKWIDGETS_EXPORT dtkNotificationQueue : public QObject
{
    Q_OBJECT

public:
    static dtkNotificationQueue *instance(void);

public:
    void registerNotifiable(dtkNotifiable *notifiable);

public:
    void clear(void);

public:
    void next(void);
    void previous(void);

public:
    void dismiss(void);

public:
    void push(dtkNotificationEvent *event);

protected slots:
    void idle(void);

public:
    bool event(QEvent *event);

private:
    static dtkNotificationQueue *s_instance;

private:
     dtkNotificationQueue(QObject *parent = 0);
    ~dtkNotificationQueue(void);

private:
    dtkNotificationQueuePrivate *d;
};

//
// dtkNotificationQueue.h ends here
