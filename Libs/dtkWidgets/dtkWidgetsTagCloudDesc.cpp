/* dtkWidgetsTagCloudDesc.cpp ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 14:37:36 2013 (+0200)
 * Version:
 * Last-Updated: Mon Apr 15 14:38:16 2013 (+0200)
 *           By: Julien Wintz
 *     Update #: 3
 */

/* Change Log:
 *
 */

#include "dtkWidgetsTagCloudDesc.h"

class dtkWidgetsTagCloudDescPrivate
{
public:
    QTextBrowser *browser;

public:
    QPushButton *back;
};

dtkWidgetsTagCloudDesc::dtkWidgetsTagCloudDesc(QWidget *parent) : QFrame(parent), d(new dtkWidgetsTagCloudDescPrivate)
{
    d->browser = new QTextBrowser(this);
    d->browser->setAttribute(Qt::WA_MacShowFocusRect, false);
    d->browser->setFrameShape(QFrame::NoFrame);

    d->back = new QPushButton("Back", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->browser);
    layout->addWidget(d->back);

    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFrameShape(QFrame::NoFrame);

    connect(d->back, SIGNAL(clicked()), this, SIGNAL(back()));
}

dtkWidgetsTagCloudDesc::~dtkWidgetsTagCloudDesc(void)
{
    delete d;

    d = NULL;
}

void dtkWidgetsTagCloudDesc::clear(void)
{
    d->browser->clear();
}

void dtkWidgetsTagCloudDesc::setDescription(const QString& description)
{
    d->browser->setHtml(description);
}
