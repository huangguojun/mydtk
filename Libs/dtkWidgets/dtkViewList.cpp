// Version: $Id: 474913b080e06707b40a35dfd9a0b82e4e655c57 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewList.h"
#include "dtkViewController.h"
#include "dtkViewWidget.h"

class dtkViewListPrivate
{
public:
    QList<QWidget *> connected_views;
};

dtkViewList::dtkViewList(QWidget *parent) : QListWidget(parent), d(new dtkViewListPrivate)
{
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setDragEnabled(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(dtkViewController::instance(), SIGNAL(inserted(dtkViewWidget *, const QString &)), this,
            SLOT(update()));

    this->update();
}

dtkViewList::~dtkViewList(void)
{
    delete d;

    d = NULL;
}

void dtkViewList::update(void)
{
    this->clear();

    foreach (QString objectName, dtkViewController::instance()->viewNames()) {
        QWidget *view = dtkViewController::instance()->view(objectName);
        QListWidgetItem *item = new QListWidgetItem;
        QString text = objectName;
        item->setText(text);
        this->addItem(item);

        if (!d->connected_views.contains(view)) {
            d->connected_views.append(view);
        }
    }
}

void dtkViewList::clear(void)
{
    QListWidget::clear();
}

QMimeData *dtkViewList::mimeData(const QList<QListWidgetItem *> items) const
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(items.first()->text());

    return mimeData;
}

QStringList dtkViewList::mimeTypes(void) const
{
    return QStringList() << "text/plain";
}

//
// dtkViewList.cpp ends here
