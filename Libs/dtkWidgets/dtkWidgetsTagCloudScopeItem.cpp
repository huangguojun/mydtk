// Version: $Id: b8fbf58a1cab819229a6b484cb6dd8537389b16f $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudScopeItem.h"

class dtkWidgetsTagCloudScopeItemPrivate
{
public:
    QLabel *bg;
    QLabel *fg;

public:
    int height;
    int width;
    int offset_min;
    int offset_max;
};

dtkWidgetsTagCloudScopeItem::dtkWidgetsTagCloudScopeItem(QWidget *parent)
    : QWidget(parent), d(new dtkWidgetsTagCloudScopeItemPrivate)
{
    d->width = 100;
    d->height = 24;
    d->offset_min = 10;
    d->offset_max = 30;

    d->bg = new QLabel(this);
    d->bg->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    d->bg->move(d->offset_min, 1);
    d->bg->setFixedHeight(d->height);
    d->bg->setFixedWidth(d->width);
    d->bg->setStyleSheet("border-image: "
                         "url(:dtkWidgetsTagCloud/dtkWidgetsTagCloudScopeItem-bg.png) 3 10 3 10;"
                         "border-top: 3px transparent;"
                         "border-bottom: 3px transparent;"
                         "border-right: 10px transparent;"
                         "border-left: 10px transparent;"
                         "padding-right: 0px;"
                         "color: white;"
                         "font-size: 10px;");
    d->bg->lower();

    d->fg = new QLabel(this);
    d->fg->setAlignment(Qt::AlignCenter);
    d->fg->setFixedHeight(d->height);
    d->fg->setFixedWidth(d->width);
    d->fg->setStyleSheet("border-image: "
                         "url(:dtkWidgetsTagCloud/"
                         "dtkWidgetsTagCloudScopeItem-fg-light.png) 3 10 3 10;"
                         "border-top: 3px transparent;"
                         "border-bottom: 3px transparent;"
                         "border-right: 10px transparent;"
                         "border-left: 10px transparent;");
    d->fg->raise();

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

dtkWidgetsTagCloudScopeItem::~dtkWidgetsTagCloudScopeItem(void)
{
    delete d;

    d = NULL;
}

void dtkWidgetsTagCloudScopeItem::setDark(void)
{
    d->fg->setStyleSheet("border-image: "
                         "url(:dtkWidgetsTagCloud/"
                         "dtkWidgetsTagCloudScopeItem-fg-dark.png) 3 10 3 10;"
                         "border-top: 3px transparent;"
                         "border-bottom: 3px transparent;"
                         "border-right: 10px transparent;"
                         "border-left: 10px transparent;");
}

void dtkWidgetsTagCloudScopeItem::setDoom(void)
{
    d->fg->setStyleSheet("border-image: "
                         "url(:dtkWidgetsTagCloud/"
                         "dtkWidgetsTagCloudScopeItem-fg-dark.png) 3 10 3 10;"
                         "border-top: 3px transparent;"
                         "border-bottom: 3px transparent;"
                         "border-right: 10px transparent;"
                         "border-left: 10px transparent;");
}

QSize dtkWidgetsTagCloudScopeItem::sizeHint(void) const
{
    return QSize(d->width + d->offset_max, d->height);
}

QString dtkWidgetsTagCloudScopeItem::text(void)
{
    return d->fg->text();
}

void dtkWidgetsTagCloudScopeItem::setText(const QString &text)
{
    d->fg->setText(text);

    static int margin = 2;
    static int h_border = 10;
    static int v_border = 3;

    QFontMetrics metrics(this->font());
    d->width = metrics.width(text) + 2 * margin + 2 * h_border;
    d->height = metrics.height() + 2 * v_border;

    d->fg->setFixedHeight(d->height);
    d->fg->setFixedWidth(d->width);

    d->bg->setFixedHeight(d->height);
    d->bg->setFixedWidth(d->width);
}

void dtkWidgetsTagCloudScopeItem::setCount(int count)
{
    d->bg->setText(QString::number(count));
}

void dtkWidgetsTagCloudScopeItem::enterEvent(QEvent *event)
{
    if (d->bg->text().isEmpty())
        return;

    Q_UNUSED(event);

    QPropertyAnimation *animation = new QPropertyAnimation(d->bg, "pos");
    animation->setDuration(250);
    animation->setStartValue(QPoint(d->offset_min, 1));
    animation->setEndValue(QPoint(d->offset_max, 1));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void dtkWidgetsTagCloudScopeItem::leaveEvent(QEvent *event)
{
    if (d->bg->text().isEmpty())
        return;

    Q_UNUSED(event);

    QPropertyAnimation *animation = new QPropertyAnimation(d->bg, "pos");
    animation->setDuration(250);
    animation->setStartValue(QPoint(d->offset_max, 1));
    animation->setEndValue(QPoint(d->offset_min, 1));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void dtkWidgetsTagCloudScopeItem::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}

//
// dtkWidgetsTagCloudScopeItem.cpp ends here
