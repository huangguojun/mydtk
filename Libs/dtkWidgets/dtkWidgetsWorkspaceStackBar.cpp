#include "dtkWidgetsWorkspaceStackBar.h"
#include "dtkWidgetsWorkspace.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsWorkspaceStackBarOverview : public QLabel
{
public:
    dtkWidgetsWorkspaceStackBarOverview(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
};

dtkWidgetsWorkspaceStackBarOverview::dtkWidgetsWorkspaceStackBarOverview(QWidget *parent)
    : QLabel(parent)
{
}

void dtkWidgetsWorkspaceStackBarOverview::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QFontMetrics metrics(qApp->font());

    const int offset = 30;
    const int margin = 20;

    QString text = this->objectName();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 128));
    painter.drawRect(0, this->size().height() - offset - margin, this->size().width(),
                     margin * 7 / 4);
    painter.setPen(Qt::white);
    painter.drawText(this->size().width() / 2 - metrics.width(text) / 2,
                     this->size().height() - offset, text);
}

class dtkWidgetsWorkspaceStackBarItem : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    dtkWidgetsWorkspaceStackBarItem(int index, QWidget *parent = nullptr);
    ~dtkWidgetsWorkspaceStackBarItem(void);

signals:
    void clicked(int);

signals:
    void activeChanged(void);

public:
    bool active(void) { return this->on; }

    void setActive(bool active)
    {
        this->on = active;

        emit activeChanged();
    }

public:
    void setSource(QWidget *);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    QWidget *source = nullptr;

private:
    dtkWidgetsWorkspaceStackBarOverview *overview;

private:
    int index = -1;

private:
    bool on = false;
};

dtkWidgetsWorkspaceStackBarItem::dtkWidgetsWorkspaceStackBarItem(int index, QWidget *parent)
    : QFrame(parent)
{
    this->index = index;

    this->overview = new dtkWidgetsWorkspaceStackBarOverview(this);
    this->overview->setObjectName("overview");
    this->overview->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(this->overview);
    layout->addStretch();
}

dtkWidgetsWorkspaceStackBarItem::~dtkWidgetsWorkspaceStackBarItem(void) {}

void dtkWidgetsWorkspaceStackBarItem::setSource(QWidget *source)
{
    this->source = source;

    this->overview->setObjectName(this->source->objectName());

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);

    connect(timer, &QTimer::timeout, [=](void) {
        if (this->visibleRegion().isNull())
            return;
        QPixmap pixmap(this->source->size());
        this->source->render(&pixmap);
        this->overview->setPixmap(pixmap.scaledToWidth(280));
    });

    timer->start();
}

void dtkWidgetsWorkspaceStackBarItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit clicked(this->index);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsWorkspaceStackBarPrivate
{
public:
    QStackedWidget *stack = nullptr;

public:
    QList<dtkWidgetsWorkspaceStackBarItem *> items;

public:
    QVBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkWidgetsWorkspaceStackBar::dtkWidgetsWorkspaceStackBar(QWidget *parent) : QFrame(parent)
{
    d = new dtkWidgetsWorkspaceStackBarPrivate;

    d->layout = new QVBoxLayout(this);
    d->layout->setContentsMargins(0, 0, 1, 0);
    d->layout->setSpacing(1);
}

dtkWidgetsWorkspaceStackBar::~dtkWidgetsWorkspaceStackBar(void)
{
    delete d;
}

QSize dtkWidgetsWorkspaceStackBar::sizeHint(void) const
{
    return QSize(300, 100);
}

void dtkWidgetsWorkspaceStackBar::setStack(QStackedWidget *stack)
{
    d->stack = stack;

    if (!d->stack->count())
        qDebug() << "Please setup stack before computing the bar.";

    for (int i = 0; i < d->stack->count(); i++) {

        dtkWidgetsWorkspaceStackBarItem *item = new dtkWidgetsWorkspaceStackBarItem(i, this);
        item->setSource(d->stack->widget(i));

        connect(item, &dtkWidgetsWorkspaceStackBarItem::clicked, [=](int i) {
            dtkWidgetsWorkspace *previous =
                    dynamic_cast<dtkWidgetsWorkspace *>(d->stack->currentWidget());
            previous->leave();
            d->stack->setCurrentIndex(i);
            dtkWidgetsWorkspace *current =
                    dynamic_cast<dtkWidgetsWorkspace *>(d->stack->currentWidget());
            current->enter();
        });
        connect(item, SIGNAL(clicked(int)), d->stack, SLOT(setCurrentIndex(int)));
        connect(item, &dtkWidgetsWorkspaceStackBarItem::clicked, [=](int) {
            for (dtkWidgetsWorkspaceStackBarItem *it : d->items) {
                this->style()->unpolish(it);
                it->setActive(it == item);
                this->style()->polish(it);
            }
        });

        d->layout->addWidget(item);
        d->items << item;
    }

    d->items.first()->setActive(true);
}

// ///////////////////////////////////////////////////////////////////

#include "dtkWidgetsWorkspaceStackBar.moc"

//
// dtkWidgetsWorkspaceStackBar.cpp ends here
