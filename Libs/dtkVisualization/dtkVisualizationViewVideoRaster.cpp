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

#include "dtkVisualizationViewVideoRaster.h"

#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewVideoRasterPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationViewVideoRasterPrivate : public QLabel
{
public:
    dtkVisualizationViewVideoRasterPrivate(QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public:
    QImage image;
    QString title;
};

dtkVisualizationViewVideoRasterPrivate::dtkVisualizationViewVideoRasterPrivate(QWidget *parent)
    : QLabel(parent)
{
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

QSize dtkVisualizationViewVideoRasterPrivate::sizeHint(void) const
{
    return QSize(100, 100);
}

void dtkVisualizationViewVideoRasterPrivate::resizeEvent(QResizeEvent *event)
{
    this->setPixmap(QPixmap::fromImage(this->image.scaled(
            event->size().width(), event->size().height(), Qt::KeepAspectRatio)));
}

void dtkVisualizationViewVideoRasterPrivate::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QFontMetrics metrics(qApp->font());

    const int offset = 50;
    const int margin = 20;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 128));
    painter.drawRect(0, this->size().height() - offset - margin, this->size().width(),
                     margin * 7 / 4);
    painter.setPen(Qt::white);
    painter.drawText(this->size().width() / 2 - metrics.width(this->title) / 2,
                     this->size().height() - offset, this->title);
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewVideoRaster
// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewVideoRaster::dtkVisualizationViewVideoRaster(QWidget *parent)
    : dtkWidgetsWidget(parent), d(new dtkVisualizationViewVideoRasterPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->image = QImage(1, 1, QImage::Format_RGBA8888);
    d->image.fill(Qt::black);
    d->setPixmap(QPixmap::fromImage(d->image));
}

dtkVisualizationViewVideoRaster::~dtkVisualizationViewVideoRaster(void)
{
    delete d;
}

QWidget *dtkVisualizationViewVideoRaster::widget(void)
{
    return d;
}

void dtkVisualizationViewVideoRaster::update(void)
{
    qDebug() << Q_FUNC_INFO;
}

void dtkVisualizationViewVideoRaster::setTitle(const QString &title)
{
    d->title = title;
}

void dtkVisualizationViewVideoRaster::setImage(const QImage &image)
{
    d->image = image;
    d->setPixmap(
            QPixmap::fromImage(image.scaled(this->width(), this->height(), Qt::KeepAspectRatio)));
}

void dtkVisualizationViewVideoRaster::resizeEvent(QResizeEvent *event)
{
    dtkWidgetsWidget::resizeEvent(event);
    d->setPixmap(QPixmap::fromImage(
            d->image.scaled(event->size().width(), event->size().height(), Qt::KeepAspectRatio)));
}

//
// dtkVisualizationViewVideoRaster.cpp ends here
