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

#include "dtkWidgetsOverlayPaneSlider.h"

#include "dtkWidgetsMenu+ux.h"
#include "dtkWidgetsMenuBar_p.h"

#include <dtkThemes/dtkThemesEngine>

#include <QtWidgets>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

static const int duration = 125;

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneSliderBar
// /////////////////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneSliderBar : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsOverlayPaneSliderBar(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneSliderBar(void);

signals:
    void clicked(int);

public:
    void addItem(fa::icon);
    void addItem(fa::icon icon, const QString &tooltip);

public:
    QVBoxLayout *layout;

public:
    int icon_size = 50;

public:
    QList<QToolButton *> buttons;
};

dtkWidgetsOverlayPaneSliderBar::dtkWidgetsOverlayPaneSliderBar(QWidget *parent) : QFrame(parent)
{
    this->layout = new QVBoxLayout(this);
    this->layout->setAlignment(Qt::AlignTop);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);

    this->setFixedWidth(this->icon_size);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

dtkWidgetsOverlayPaneSliderBar::~dtkWidgetsOverlayPaneSliderBar(void) {}

void dtkWidgetsOverlayPaneSliderBar::addItem(fa::icon icon)
{
    dtkFontAwesome::instance()->setProperty("color", dtkThemesEngine::instance()->color("@fg"));

    QToolButton *button = new QToolButton(this);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setIcon(
            dtkFontAwesome::instance()->icon(icon).pixmap(QSize(this->icon_size, this->icon_size)));
    button->setFixedWidth(this->icon_size);
    button->setFixedHeight(this->icon_size);

    this->buttons << button;

    this->layout->addWidget(button);

    connect(button, &QToolButton::clicked,
            [=](void) { emit clicked(this->buttons.indexOf(button)); });
}

void dtkWidgetsOverlayPaneSliderBar::addItem(fa::icon icon, const QString &tooltip)
{
    this->addItem(icon);
    this->buttons.last()->setToolTip(tooltip);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneSliderViewPort
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneSliderViewPort : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsOverlayPaneSliderViewPort(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneSliderViewPort(void);

public slots:
    void addWidget(QWidget *);
    void remWidget(QWidget *);

public:
    QHBoxLayout *layout;

public:
    int count = 0;
    int bound = 300;
};

dtkWidgetsOverlayPaneSliderViewPort::dtkWidgetsOverlayPaneSliderViewPort(QWidget *parent)
    : QFrame(parent)
{
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);
}

dtkWidgetsOverlayPaneSliderViewPort::~dtkWidgetsOverlayPaneSliderViewPort(void) {}

void dtkWidgetsOverlayPaneSliderViewPort::addWidget(QWidget *widget)
{
    widget->setFixedWidth(this->bound);

    this->layout->addWidget(widget);

    this->repaint();

    this->count++;
}

void dtkWidgetsOverlayPaneSliderViewPort::remWidget(QWidget *widget)
{
    this->layout->removeWidget(widget);
    widget->setParent(0);

    this->repaint();

    this->count--;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneSliderAreao
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneSliderArea : public QScrollArea
{
    Q_OBJECT

public:
    dtkWidgetsOverlayPaneSliderArea(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneSliderArea(void);

public:
    void slideH(int dx);
    void slideV(int dx);

public:
    void enableSpying(bool);

public:
    QWidget *widget(int index);

protected:
    bool event(QEvent *) override;

protected:
    void keyPressEvent(QKeyEvent *) override;
    void wheelEvent(QWheelEvent *) override;

protected:
    void scrollContentsBy(int, int) override;

public:
    bool spying = false;

public:
    dtkWidgetsOverlayPaneSlider *q = nullptr;

public:
    int to = 0;
};

dtkWidgetsOverlayPaneSliderArea::dtkWidgetsOverlayPaneSliderArea(QWidget *parent)
    : QScrollArea(parent)
{
    this->setFrameShape(QFrame::NoFrame);
}

dtkWidgetsOverlayPaneSliderArea::~dtkWidgetsOverlayPaneSliderArea(void) {}

void dtkWidgetsOverlayPaneSliderArea::slideH(int dx)
{
    this->horizontalScrollBar()->setValue(dx);
}

void dtkWidgetsOverlayPaneSliderArea::slideV(int dx)
{
    this->verticalScrollBar()->setValue(dx);
}

void dtkWidgetsOverlayPaneSliderArea::enableSpying(bool enable)
{
    this->spying = enable;
}

bool dtkWidgetsOverlayPaneSliderArea::event(QEvent *event)
{
    return QScrollArea::event(event);
}

void dtkWidgetsOverlayPaneSliderArea::keyPressEvent(QKeyEvent *)
{
    ; // Override. This is no bug.
}

void dtkWidgetsOverlayPaneSliderArea::wheelEvent(QWheelEvent *event)
{
    QWheelEvent *e = new QWheelEvent(
            event->posF(), event->globalPosF(), QPoint(0, event->pixelDelta().y()),
            QPoint(0, event->angleDelta().y()), event->delta(), Qt::Vertical, event->buttons(),
            event->modifiers(), event->phase(), event->source(), event->inverted());

    QScrollArea::wheelEvent(e);
}

QWidget *dtkWidgetsOverlayPaneSliderArea::widget(int index)
{
    if (this->size().width() < 100)
        return nullptr;

    if (dtkWidgetsOverlayPaneSliderViewPort *viewport =
                dynamic_cast<dtkWidgetsOverlayPaneSliderViewPort *>(QScrollArea::widget())) {

        for (int i = 0; i < viewport->layout->count(); ++i) {

            if (dtkWidgetsMenuBarContainerSlide *slide =
                        dynamic_cast<dtkWidgetsMenuBarContainerSlide *>(
                                viewport->layout->itemAt(i)->widget())) {

                int k = 0;

                for (int j = 0; j < slide->layout()->count(); ++j) {

                    if (dtkWidgetsMenuHeaderFacade *facade =
                                dynamic_cast<dtkWidgetsMenuHeaderFacade *>(
                                        slide->layout()->itemAt(j)->widget())) {

                        if (k == index) {

                            for (int k = j; k < slide->layout()->count(); ++k) {

                                if (dtkWidgetsMenuFacade *facade =
                                            dynamic_cast<dtkWidgetsMenuFacade *>(
                                                    slide->layout()->itemAt(k)->widget())) {

                                    return facade;
                                }
                            }
                        }

                        ++k;
                    }
                }
            }
        }
    }

    return nullptr;
}

void dtkWidgetsOverlayPaneSliderArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx, dy);

    if (this->spying && !this->to) {

        if (dtkWidgetsOverlayPaneSliderViewPort *viewport =
                    dynamic_cast<dtkWidgetsOverlayPaneSliderViewPort *>(QScrollArea::widget())) {

            for (int i = 0; i < viewport->layout->count(); i++) {

                if (dtkWidgetsMenuBarContainerSlide *slide =
                            dynamic_cast<dtkWidgetsMenuBarContainerSlide *>(
                                    viewport->layout->itemAt(i)->widget())) {

                    int k = 0;

                    for (int j = 0; j < slide->layout()->count(); j++) {

                        if (dtkWidgetsMenuHeaderFacade *facade =
                                    dynamic_cast<dtkWidgetsMenuHeaderFacade *>(
                                            slide->layout()->itemAt(j)->widget())) {

                            int p_y = this->mapToGlobal(this->rect().topLeft()).y();
                            int s_y = facade->mapToGlobal(this->rect().topLeft()).y();

                            if (s_y > p_y) {

                                emit q->indexVisible(k);

                                return;
                            }

                            k++;
                        }
                    }
                }
            }
        }
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneSliderPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneSliderPrivate : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsOverlayPaneSliderPrivate(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneSliderPrivate(void);

public:
    void addWidget(fa::icon, QWidget *);
    void addWidget(fa::icon, QWidget *, const QString &);

public:
    void remWidget(QWidget *);

public slots:
    void slideTo(int);
    void slideTo(int, std::function<void(void)> &);
    void slideToNext(void);
    void slideToPrevious(void);
    void slideToPrevious(std::function<void(void)> &);

public slots:
    void setCurrentIndex(int, std::function<void(void)> &);

private slots:
    QAbstractAnimation *slideToPrivate(int, std::function<void(void)> &);

public:
    // dtkWidgetsOverlayPaneSliderBar *bar;
    dtkWidgetsOverlayPaneSliderArea *area;
    dtkWidgetsOverlayPaneSliderViewPort *viewport;

public:
    int from = 0;

public:
    double h_ss = 0, v_ss = 0;
    double h_sm = 0, v_sm = 0;

public:
    dtkWidgetsOverlayPaneSlider *q = nullptr;

public:
    QSequentialAnimationGroup *animation = nullptr;
};

dtkWidgetsOverlayPaneSliderPrivate::dtkWidgetsOverlayPaneSliderPrivate(QWidget *parent)
    : QFrame(parent)
{
    this->viewport = new dtkWidgetsOverlayPaneSliderViewPort(this);

    // this->bar = new dtkWidgetsOverlayPaneSliderBar(this);

    this->area = new dtkWidgetsOverlayPaneSliderArea(this);
    this->area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->area->setWidgetResizable(true);
    this->area->setWidget(this->viewport);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    //  layout->addWidget(this->bar);
    layout->addWidget(this->area);

    //  connect(this->bar, SIGNAL(clicked(int)), this, SLOT(slideTo(int)));
}

dtkWidgetsOverlayPaneSliderPrivate::~dtkWidgetsOverlayPaneSliderPrivate(void) {}

void dtkWidgetsOverlayPaneSliderPrivate::addWidget(fa::icon icon, QWidget *widget)
{
    widget->show();

    this->viewport->addWidget(widget);
    this->viewport->updateGeometry();
    this->viewport->repaint();

    this->area->setWidgetResizable(true);
    this->area->setWidget(this->viewport);
    this->area->horizontalScrollBar()->updateGeometry();
    this->area->horizontalScrollBar()->update();
    this->area->updateGeometry();
    this->area->repaint();
}

void dtkWidgetsOverlayPaneSliderPrivate::addWidget(fa::icon icon, QWidget *widget,
                                                   const QString &tooltip)
{
    widget->show();

    this->viewport->addWidget(widget);
    this->viewport->updateGeometry();
    this->viewport->repaint();

    this->area->setWidgetResizable(true);
    this->area->setWidget(this->viewport);
    this->area->horizontalScrollBar()->updateGeometry();
    this->area->horizontalScrollBar()->update();
    this->area->updateGeometry();
    this->area->repaint();
}

void dtkWidgetsOverlayPaneSliderPrivate::remWidget(QWidget *widget)
{
    this->viewport->remWidget(widget);
    this->viewport->updateGeometry();
    this->viewport->repaint();

    this->area->setWidget(this->viewport);
    this->area->horizontalScrollBar()->updateGeometry();
    this->area->horizontalScrollBar()->update();
    this->area->updateGeometry();
    this->area->repaint();
}

QAbstractAnimation *
dtkWidgetsOverlayPaneSliderPrivate::slideToPrivate(int to, std::function<void(void)> &callback)
{
    if (this->viewport->count <= 1)
        return 0;

    if (to > this->viewport->count)
        return 0;

    if (to < 0)
        return 0;

    if (this->animation)
        return 0;

    double h_xf = this->from;
    this->from = to;
    this->area->to = to;
    double h_xt = to;
    this->h_ss = this->area->horizontalScrollBar()->value();

    QVariantAnimation *h_animation = new QVariantAnimation;
    h_animation->setEasingCurve(QEasingCurve::OutBounce);
    h_animation->setStartValue(h_xf);
    h_animation->setEndValue(h_xt);
    h_animation->setDuration(::duration);

    connect(h_animation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
        double ss = this->h_ss;
        double sm = this->area->horizontalScrollBar()->maximum();
        double sc = this->viewport->count - 1;

        this->area->slideH(ss + (value.toDouble() - h_xf) * sm / sc);
    });

    // Vertical animation - scroll up.

    double v_xf = this->area->verticalScrollBar()->value();
    double v_xt = 0;

    QVariantAnimation *v_animation = new QVariantAnimation;
    v_animation->setEasingCurve(QEasingCurve::Linear);
    v_animation->setStartValue(v_xf);
    v_animation->setEndValue(v_xt);

    // the vertical scroll duration is proportional to the height we need to
    // scroll up. this way it ends the transition as soon as it has finished and
    // it frees up the dirty flag is_in_transition.
    double ratio_to_scroll = v_xf;
    if (this->area->verticalScrollBar()->maximum() > 0)
        ratio_to_scroll /= this->area->verticalScrollBar()->maximum();

    v_animation->setDuration(::duration * ratio_to_scroll);

    connect(v_animation, &QVariantAnimation::valueChanged,
            [=](const QVariant &value) { this->area->slideV(value.toDouble()); });

    this->animation = new QSequentialAnimationGroup(this);
    this->animation->addAnimation(h_animation);
    this->animation->addAnimation(v_animation);

    connect(this->animation, &QAbstractAnimation::finished, [=](void) -> void {
        this->animation->deleteLater();
        this->animation = nullptr;

        callback();

        // q->blockSignals(false);
        q->informInTransition(false);
    });

    // q->blockSignals(true);
    q->informInTransition(true);

    this->animation->start();

    return animation;
}

void dtkWidgetsOverlayPaneSliderPrivate::slideTo(int to)
{
    std::function<void(void)> callback = [=](void) -> void {
    }; // empty callback to call the same function with it
    Q_UNUSED(this->slideToPrivate(to, callback));
}

void dtkWidgetsOverlayPaneSliderPrivate::slideTo(int to, std::function<void(void)> &callback)
{
    Q_UNUSED(this->slideToPrivate(to, callback));
}

void dtkWidgetsOverlayPaneSliderPrivate::slideToNext(void)
{
    this->slideTo(this->from + 1);
}

void dtkWidgetsOverlayPaneSliderPrivate::slideToPrevious(void)
{
    this->slideTo(this->from - 1);
}

void dtkWidgetsOverlayPaneSliderPrivate::slideToPrevious(std::function<void(void)> &callback)
{
    this->slideTo(this->from - 1, callback);
}

void dtkWidgetsOverlayPaneSliderPrivate::setCurrentIndex(int index,
                                                         std::function<void(void)> &callback)
{
    if (this->size().width() < 100)
        return;

    if (this->area->to)
        return;

    QWidget *destination = this->area->widget(index);

    const QRect microFocus = destination->inputMethodQuery(Qt::ImMicroFocus).toRect();
    const QRect defaultMicroFocus =
            destination->QWidget::inputMethodQuery(Qt::ImMicroFocus).toRect();

    QRect focusRect = (microFocus != defaultMicroFocus)
            ? QRect(destination->mapTo(this->viewport, microFocus.topLeft()), microFocus.size())
            : QRect(destination->mapTo(this->viewport, QPoint(0, 0)), destination->size());

    const QRect visibleRect(-this->viewport->pos(), this->area->viewport()->size());

    if (visibleRect.contains(focusRect))
        return;

    focusRect.adjust(0, -32, 0,
                     32 + 1); // No bug. To include title. // TODO: use theme

    double xf = this->area->verticalScrollBar()->value();
    double xt = -1.0;

    if (focusRect.height() > visibleRect.height())
        xt = focusRect.center().y() - this->area->viewport()->height() / 2;
    else if (focusRect.bottom() > visibleRect.bottom())
        xt = focusRect.bottom() - this->area->viewport()->height();
    else
        xt = focusRect.top();

    if (xt < 0)
        return;

    QVariantAnimation *t_animation = new QVariantAnimation;
    t_animation->setEasingCurve(QEasingCurve::OutBounce);
    t_animation->setStartValue(xf);
    t_animation->setEndValue(xt);
    t_animation->setDuration(::duration);

    connect(t_animation, &QVariantAnimation::valueChanged,
            [=](const QVariant &value) { this->area->slideV(value.toDouble()); });

    connect(t_animation, &QAbstractAnimation::finished, [=](void) -> void {
        if (this->animation) {
            this->animation->deleteLater();
            this->animation = nullptr;
        }

        callback();

        // q->blockSignals(false);
        q->informInTransition(false);
    });

    // q->blockSignals(true);
    q->informInTransition(true);

    t_animation->start();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkWidgetsOverlayPaneSlider::dtkWidgetsOverlayPaneSlider(QWidget *parent)
    : dtkWidgetsOverlayPaneItem(parent)
{
    d = new dtkWidgetsOverlayPaneSliderPrivate;
    d->q = this;
    d->area->q = this;

    this->setSlider(d);
}

dtkWidgetsOverlayPaneSlider::~dtkWidgetsOverlayPaneSlider(void)
{
    delete d;
}

void dtkWidgetsOverlayPaneSlider::informInTransition(bool t)
{
    this->is_in_transition = t;
}

void dtkWidgetsOverlayPaneSlider::setBound(int bound)
{
    d->viewport->bound = bound;

    for (int i = 0; i < d->viewport->layout->count(); ++i) {
        QLayoutItem *item = d->viewport->layout->itemAt(i);
        if (item->widget())
            item->widget()->setFixedWidth(bound);
    }
}

void dtkWidgetsOverlayPaneSlider::addSlide(fa::icon icon, QWidget *contents)
{
    d->addWidget(icon, contents);
}

void dtkWidgetsOverlayPaneSlider::addSlide(fa::icon icon, QWidget *contents, const QString &tooltip)
{
    d->addWidget(icon, contents, tooltip);
}

void dtkWidgetsOverlayPaneSlider::remSlide(QWidget *w)
{
    d->remWidget(w);
}

void dtkWidgetsOverlayPaneSlider::slideTo(int index)
{
    d->slideTo(index);
}

void dtkWidgetsOverlayPaneSlider::slideTo(int index, std::function<void()> &callback)
{
    d->slideTo(index, callback);
}

void dtkWidgetsOverlayPaneSlider::slideToNext(void)
{
    d->slideToNext();
}

void dtkWidgetsOverlayPaneSlider::slideToPrevious(void)
{
    d->slideToPrevious();
}

void dtkWidgetsOverlayPaneSlider::slideToPrevious(std::function<void()> &callback)
{
    d->slideToPrevious(callback);
}

void dtkWidgetsOverlayPaneSlider::setCurrentIndex(int index)
{
    std::function<void(void)> callback = [=](void) -> void {
    }; // empty callback to call the same function with it
    d->setCurrentIndex(index, callback);
}

void dtkWidgetsOverlayPaneSlider::setCurrentIndex(int index, std::function<void()> &callback)
{
    d->setCurrentIndex(index, callback);
}

void dtkWidgetsOverlayPaneSlider::enableSpying(bool enable)
{
    d->area->enableSpying(enable);
}

void dtkWidgetsOverlayPaneSlider::decr(void)
{
    this->slideToPrevious();

    d->viewport->count--;
}

void dtkWidgetsOverlayPaneSlider::reset(void)
{
    d->viewport->count = 1; // starts at 1
    d->from = 0;
}

void dtkWidgetsOverlayPaneSlider::print(void)
{
    qDebug() << "d->from = " << this->d->from;
    qDebug() << "d->viewport->count = " << d->viewport->count;
}

// ///////////////////////////////////////////////////////////////////

#include "dtkWidgetsOverlayPaneSlider.moc"

//
// dtkWidgetsOverlayPaneSlider.cpp ends here
