// Version: $Id: d88db6c5235956743ab5c4625358b1a4d9bf1b00 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkNotificationDisplay.h"
#include "dtkNotification.h"
#include "dtkNotificationEvent.h"
#include "dtkNotificationQueue.h"

class dtkNotificationDisplayPrivate
{
public:
    QString read(const QString &path);

public:
    QLabel *message;
    QLabel *persistent_count;
    QLabel *non_persistent_count;

public:
    QToolButton *next;
    QToolButton *prev;
    QToolButton *clse;
};

QString dtkNotificationDisplayPrivate::read(const QString &path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly))
        qDebug() << "Unable to read file" << path;

    QString contents = file.readAll();

    file.close();

    return contents;
}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

dtkNotificationDisplay::dtkNotificationDisplay(QWidget *parent)
    : QFrame(parent), d(new dtkNotificationDisplayPrivate)
{
    dtkNotificationQueue::instance()->registerNotifiable(this);

    d->message = new QLabel(this);
    d->message->setObjectName("dtkNotificationDisplayMessage");
    d->message->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    d->persistent_count = new QLabel(QString::number(0), this);
    d->persistent_count->setObjectName("dtkNotificationDisplayPersistentCount");
    d->persistent_count->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    d->persistent_count->setVisible(false);

    d->non_persistent_count = new QLabel(QString::number(0), this);
    d->non_persistent_count->setObjectName("dtkNotificationDisplayNonPersistentCount");
    d->non_persistent_count->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    d->non_persistent_count->setVisible(false);

    d->next = new QToolButton(this);
    d->next->setObjectName("dtkNotifiable-next");
    d->next->setVisible(false);

    d->prev = new QToolButton(this);
    d->prev->setObjectName("dtkNotifiable-prev");
    d->prev->setVisible(false);

    d->clse = new QToolButton(this);
    d->clse->setObjectName("dtkNotifiable-clse");
    d->clse->setVisible(false);

    QVBoxLayout *l_layout = new QVBoxLayout;
    l_layout->setContentsMargins(0, 0, 0, 0);
    l_layout->setSpacing(0);
    l_layout->addWidget(d->next);
    l_layout->addWidget(d->prev);

    QVBoxLayout *r_layout = new QVBoxLayout;
    r_layout->setContentsMargins(0, 0, 0, 0);
    r_layout->setSpacing(0);
    r_layout->addWidget(d->persistent_count);
    r_layout->addWidget(d->non_persistent_count);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    layout->addLayout(l_layout);
    layout->addWidget(d->message);
    layout->addWidget(d->clse);
    layout->addLayout(r_layout);

    this->setFixedHeight(46);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->setStyleSheet(d->read(":dtkNotification/dtkNotificationDisplay.qss"));

    connect(d->next, SIGNAL(released()), this, SLOT(next()));
    connect(d->prev, SIGNAL(released()), this, SLOT(previous()));
    connect(d->clse, SIGNAL(released()), this, SLOT(dismiss()));
}

dtkNotificationDisplay::~dtkNotificationDisplay(void)
{
    delete d;

    d = NULL;
}

void dtkNotificationDisplay::clear(void)
{
    d->message->clear();
}

void dtkNotificationDisplay::next(void)
{
    dtkNotificationQueue::instance()->next();
}

void dtkNotificationDisplay::previous(void)
{
    dtkNotificationQueue::instance()->previous();
}

void dtkNotificationDisplay::dismiss(void)
{
    dtkNotificationQueue::instance()->dismiss();
}

void dtkNotificationDisplay::dismissible(bool dismissible)
{
    d->clse->setVisible(dismissible);
}

void dtkNotificationDisplay::display(const QString &message)
{
    d->message->setText(message);
}

void dtkNotificationDisplay::setPersistentCount(int count)
{
    d->persistent_count->setText(QString::number(count));
    d->persistent_count->setVisible(count > 1);

    d->prev->setVisible(count > 1);
    d->next->setVisible(count > 1);

    this->update();
}

void dtkNotificationDisplay::setNonPersistentCount(int count)
{
    d->non_persistent_count->setText(QString::number(count));
    d->non_persistent_count->setVisible(count);

    this->update();
}

QSize dtkNotificationDisplay::sizeHint(void) const
{
    return QSize(350, 46);
}

//
// dtkNotificationDisplay.cpp ends here
