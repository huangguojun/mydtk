// Version: $Id: 131d7a61618f9116e7525fde7f775c684cd126e6 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsLayoutItem.h"
#include "dtkWidgetsController.h"
#include "dtkWidgetsLayout.h"
#include "dtkWidgetsLayoutItem_p.h"
#include "dtkWidgetsWidget.h"

#include <dtkFonts>
//#include <dtkThemes>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemButton
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsLayoutItemButton : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsLayoutItemButton(fa::icon icon, QWidget *parent = nullptr);
    ~dtkWidgetsLayoutItemButton(void);

signals:
    void clicked(void);

public:
    QSize sizeHint(void) const override;

protected:
    void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;

private:
    fa::icon m_icon;
};

dtkWidgetsLayoutItemButton::dtkWidgetsLayoutItemButton(fa::icon icon, QWidget *parent)
    : QFrame(parent)
{
    dtkFontAwesome::instance()->initFontAwesome();
    this->m_icon = icon;
}

dtkWidgetsLayoutItemButton::~dtkWidgetsLayoutItemButton(void) {}

QSize dtkWidgetsLayoutItemButton::sizeHint(void) const
{
    return QSize(16, 16);
}

void dtkWidgetsLayoutItemButton::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

void dtkWidgetsLayoutItemButton::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    // dtkFontAwesome::instance()->setDefaultOption("color",
    // dtkThemesEngine::instance()->color("@fg"));

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, 16, 16, dtkFontAwesome::instance()->icon(m_icon).pixmap(16, 16));
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemProxy
// /////////////////////////////////////////////////////////////////

dtkWidgetsLayoutItemProxy::dtkWidgetsLayoutItemProxy(QWidget *parent)
    : QFrame(parent), d(new dtkWidgetsLayoutItemProxyPrivate)
{
    d->view = nullptr;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    if (!dtkWidgetsLayoutItem::actions.isEmpty()) {
        d->form = new QFormLayout;
        d->form->setFormAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        for (QString action : dtkWidgetsLayoutItem::actions) {
            QPushButton *button = new QPushButton(action, this);
            button->setFixedWidth(150);

            d->form->addWidget(button);

            connect(button, &QPushButton::clicked, this,
                    [=]() { emit create(dtkWidgetsLayoutItem::actions.key(action)); });
        }

        layout->addLayout(d->form);
    }

    this->setFocusPolicy(Qt::StrongFocus);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMouseTracking(true);
}

dtkWidgetsLayoutItemProxy::~dtkWidgetsLayoutItemProxy(void)
{
    if (!d->view)
        goto finalize;

    if (!d->view->widget())
        goto finalize;

    if (!d->view->widget()->parentWidget())
        goto finalize;

    if (dtkWidgetsLayoutItemProxy *proxy = dynamic_cast<dtkWidgetsLayoutItemProxy *>(
                d->view->widget()->parentWidget()->parentWidget())) {

        if (proxy == this) {

            layout()->removeWidget(d->view);

            d->view->setParent(0);

            disconnect(d->view, SIGNAL(focused()), proxy, SIGNAL(focusedIn()));

            d->view = nullptr;
        }
    }

finalize:

    delete d;

    d = nullptr;
}

dtkWidgetsWidget *dtkWidgetsLayoutItemProxy::view(void)
{
    return d->view;
}

void dtkWidgetsLayoutItemProxy::setView(dtkWidgetsWidget *view)
{
    if (!view)
        return;

    if (!view->widget())
        return;

    if (view->parentWidget()) {

        if (dtkWidgetsLayoutItem *item = dynamic_cast<dtkWidgetsLayoutItem *>(
                    view->parentWidget()->parentWidget()->parentWidget())) {

            if (item->d->proxy->d->view == view) {

                QStringList items = dtkWidgetsController::instance()->viewNames();
                item->d->label->blockSignals(true);
                item->d->label->clear();
                item->d->label->addItems(items);
                int index = item->d->label->findText("");
                item->d->label->setCurrentIndex(index);
                item->d->label->blockSignals(false);
                item->d->close->setEnabled(false);
                item->d->vertc->setEnabled(false);
                item->d->horzt->setEnabled(false);
                item->d->maxmz->setEnabled(false);
            }

            item->d->proxy->d->view = nullptr;
        }
    }

    if (d->view) {
        this->layout()->removeWidget(d->view->widget());
        d->view->setParent(nullptr);
    }

    if (d->form)
        this->layout()->removeItem(d->form);

    this->layout()->addWidget(view);

    d->view = view;

    connect(view, SIGNAL(focused()), this, SIGNAL(focusedIn()));

    if (dtkWidgetsLayoutItem *item =
                dynamic_cast<dtkWidgetsLayoutItem *>(this->parentWidget()->parentWidget())) {

        QStringList items = dtkWidgetsController::instance()->viewNames();

        item->d->label->blockSignals(true);
        item->d->label->clear();
        item->d->label->addItems(items);
        int index = item->d->label->findText(d->view->objectName());
        item->d->label->setCurrentIndex(index);
        item->d->label->blockSignals(false);
        item->d->close->setEnabled(true);
        item->d->vertc->setEnabled(true);
        item->d->horzt->setEnabled(true);
        item->d->maxmz->setEnabled(true);

        emit focusedIn();
    }

    QTimer::singleShot(100, d->view->widget(), SLOT(update()));
}

void dtkWidgetsLayoutItemProxy::focusInEvent(QFocusEvent *event)
{
    QFrame::focusInEvent(event);

    emit focusedIn();

    d->dirty = true;
}

void dtkWidgetsLayoutItemProxy::focusOutEvent(QFocusEvent *event)
{
    QFrame::focusOutEvent(event);

    emit focusedOut();

    d->dirty = true;
}

void dtkWidgetsLayoutItemProxy::keyPressEvent(QKeyEvent *event)
{
    // qDebug() << Q_FUNC_INFO << event->modifiers();

    // event->accept();

    // if(d->view)
    //     qApp->sendEvent(d->view, new QKeyEvent(event->type(), event->key(),
    //     event->modifiers()));

    QFrame::keyPressEvent(event);
}

void dtkWidgetsLayoutItemProxy::mouseMoveEvent(QMouseEvent *event)
{
    if (d->view && d->view->parentWidget() == this)
        return QFrame::mouseMoveEvent(event);

    if (dtkWidgetsLayoutItem::actions.count())
        return;

    QRect rect(this->rect().width() / 2 - this->rect().width() / 4,
               this->rect().height() / 2 - this->rect().height() / 4,
               this->rect().size().width() / 2, this->rect().size().height() / 2);

    /*
    if(rect.contains(event->pos()))
        dtkFontAwesome::instance()->setDefaultOption("color",
    dtkThemesEngine::instance()->color("@fg").lighter()); else
        dtkFontAwesome::instance()->setDefaultOption("color",
    dtkThemesEngine::instance()->color("@fg"));
        */

    this->update();
}

void dtkWidgetsLayoutItemProxy::mousePressEvent(QMouseEvent *event)
{
    if (d->view && d->view->parentWidget() == this)
        return QFrame::mousePressEvent(event);

    if (dtkWidgetsLayoutItem::actions.count())
        return;

    QRect rect(this->rect().width() / 2 - this->rect().width() / 4,
               this->rect().height() / 2 - this->rect().height() / 4,
               this->rect().size().width() / 2, this->rect().size().height() / 2);

    if (rect.contains(event->pos()))
        emit create();
}

void dtkWidgetsLayoutItemProxy::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (dtkWidgetsLayoutItem::actions.count())
        return QFrame::paintEvent(event);

    QRect rect(event->rect().width() / 2 - event->rect().width() / 4,
               event->rect().height() / 2 - event->rect().height() / 4,
               event->rect().size().width() / 2, event->rect().size().height() / 2);

    /*   if (d->dirty)
           dtkFontAwesome::instance()->setDefaultOption("color",
       dtkThemesEngine::instance()->color("@fg"));
   */
    QPainter painter(this);
    painter.drawPixmap(rect,
                       dtkFontAwesome::instance()
                               ->icon(fa::plussquare)
                               .pixmap(rect.size().width(), rect.size().height()));

    d->dirty = false;
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItemPrivate
// /////////////////////////////////////////////////////////////////

dtkWidgetsLayoutItemProxy *dtkWidgetsLayoutItemPrivate::firstViewChild(dtkWidgetsLayoutItem *item)
{
    if (item->d->proxy)
        return item->d->proxy;

    if (item->d->a)
        return firstViewChild(item->d->a);

    if (item->d->b)
        return firstViewChild(item->d->b);

    return nullptr;
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsLayoutItem
// /////////////////////////////////////////////////////////////////

dtkWidgetsLayoutItem::dtkWidgetsLayoutItem(dtkWidgetsLayoutItem *parent)
    : QFrame(parent), d(new dtkWidgetsLayoutItemPrivate)
{
    d->a = nullptr;
    d->b = nullptr;
    d->q = this;

    d->layout = nullptr;

    if ((d->parent = parent)) {
        d->root = d->parent->d->root;
        d->layout = d->parent->d->layout;
    } else {
        d->root = this;
    }

    d->proxy = new dtkWidgetsLayoutItemProxy(this);

    connect(d->proxy, SIGNAL(create()), this, SIGNAL(create()));
    connect(d->proxy, SIGNAL(create(const QString &)), this, SIGNAL(create(const QString &)));

    d->splitter = new QSplitter(this);
    d->splitter->setHandleWidth(1);
    d->splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    d->splitter->addWidget(d->proxy);

    d->horzt = new dtkWidgetsLayoutItemButton(fa::arrowsh, this);
    d->vertc = new dtkWidgetsLayoutItemButton(fa::arrowsv, this);
    d->close = new dtkWidgetsLayoutItemButton(fa::compress, this);
    d->maxmz = new dtkWidgetsLayoutItemButton(fa::expand, this);

    d->label = new QComboBox(this);
    d->label->setInsertPolicy(QComboBox::InsertAlphabetically);
    d->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    d->label->setEditable(true);

    connect(d->label->lineEdit(), &QLineEdit::editingFinished, [=]() {
        if (d->proxy->view()) {
            d->proxy->view()->setObjectName(d->label->lineEdit()->text());
        }
    });

    QHBoxLayout *footer_layout = new QHBoxLayout;
    footer_layout->setSpacing(20);
    footer_layout->addWidget(d->label);
    footer_layout->addWidget(d->horzt);
    footer_layout->addWidget(d->vertc);
    footer_layout->addWidget(d->maxmz);
    footer_layout->addWidget(d->close);

    d->footer = new QFrame(this);
    d->footer->setLayout(footer_layout);
    d->footer->setObjectName("dtkWidgetsLayoutItemFooterUnfocused");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);
    layout->addWidget(d->footer);

    this->setAcceptDrops(true);

    connect(d->close, SIGNAL(clicked()), this, SLOT(close()));
    connect(d->horzt, SIGNAL(clicked()), this, SLOT(horzt()));
    connect(d->vertc, SIGNAL(clicked()), this, SLOT(vertc()));
    connect(d->maxmz, SIGNAL(clicked()), this, SLOT(maxmz()));

    connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));

    connect(d->label, SIGNAL(activated(const QString)), this, SLOT(onActivated(const QString)));

    d->proxy->setFocus(Qt::OtherFocusReason);

    d->close->setEnabled(d->parent != nullptr);
    d->vertc->setEnabled(false);
    d->horzt->setEnabled(false);
    d->maxmz->setEnabled(false);

    this->setFocusPolicy(Qt::ClickFocus);
}

dtkWidgetsLayoutItem::~dtkWidgetsLayoutItem(void)
{
    delete d;

    d = nullptr;
}

dtkWidgetsWidget *dtkWidgetsLayoutItem::view(void)
{
    if (this->proxy())
        return proxy()->view();
    else
        return nullptr;
}

dtkWidgetsLayoutItem *dtkWidgetsLayoutItem::parent(void)
{
    return d->parent;
}

dtkWidgetsLayoutItem *dtkWidgetsLayoutItem::first(void)
{
    return d->a;
}

dtkWidgetsLayoutItem *dtkWidgetsLayoutItem::second(void)
{
    return d->b;
}

void dtkWidgetsLayoutItem::setOrientation(Qt::Orientation orientation)
{
    d->splitter->setOrientation(orientation);
}

void dtkWidgetsLayoutItem::setSizes(const QList<int> &sizes)
{
    d->splitter->setSizes(sizes);
}

int dtkWidgetsLayoutItem::canvasHeight(void)
{
    return d->splitter->sizes().first();
}

int dtkWidgetsLayoutItem::footerHeight(void)
{
    return d->footer->height();
}

int dtkWidgetsLayoutItem::handleHeight(void)
{
    return 1;
}

int dtkWidgetsLayoutItem::handleWidth(void)
{
    return 1;
}

dtkWidgetsLayout *dtkWidgetsLayoutItem::layout(void) const
{
    return d->layout;
}

dtkWidgetsLayoutItemProxy *dtkWidgetsLayoutItem::proxy(void)
{
    return d->proxy;
}

void dtkWidgetsLayoutItem::setLayout(dtkWidgetsLayout *layout)
{
    d->layout = layout;
}

void dtkWidgetsLayoutItem::clear(void)
{
    if (d->proxy && d->proxy->view())
        emit unfocused(d->proxy->view());

    if (d->proxy && d->proxy->view())
        d->proxy->view()->setParent(nullptr);

    if (d->proxy)
        delete d->proxy;

    d->proxy = new dtkWidgetsLayoutItemProxy(d->root);

    connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));
    connect(d->proxy, SIGNAL(create()), this, SIGNAL(create()));
    connect(d->proxy, SIGNAL(create(const QString &)), this, SIGNAL(create(const QString &)));

    d->splitter->addWidget(d->proxy);

    d->proxy->setFocus(Qt::OtherFocusReason);

    d->footer->show();

    this->setUpdatesEnabled(true);

    if (d->a)
        d->a->deleteLater();

    if (d->b)
        d->b->deleteLater();

    d->a = nullptr;
    d->b = nullptr;

    d->close->setEnabled(false);
    d->vertc->setEnabled(false);
    d->horzt->setEnabled(false);
    d->maxmz->setEnabled(false);

    QStringList items = dtkWidgetsController::instance()->viewNames();
    items.sort();

    d->label->clear();
    d->label->addItems(items);
    d->label->setCurrentText("");
}

void dtkWidgetsLayoutItem::split(bool force)
{
    if (!force && !d->proxy->view())
        return;

    QSize size = this->size();

    d->a = new dtkWidgetsLayoutItem(this);
    d->b = new dtkWidgetsLayoutItem(this);

    d->splitter->addWidget(d->a);
    d->splitter->addWidget(d->b);

    d->a->d->proxy->setFocus(Qt::OtherFocusReason);
    d->a->d->proxy->setView(d->proxy->view());

    disconnect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
    disconnect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));
    disconnect(d->proxy, SIGNAL(create()), this, SIGNAL(create()));
    disconnect(d->proxy, SIGNAL(create(const QString &)), this, SIGNAL(create(const QString &)));

    delete d->proxy;

    d->proxy = nullptr;

    d->footer->hide();

    d->splitter->resize(size);
    d->splitter->setSizes(QList<int>() << size.width() / 2 << size.width() / 2);

    d->layout->setCurrent(d->b);
}

void dtkWidgetsLayoutItem::unsplit(void)
{
    if (!d->a && !d->b)
        return;

    d->root->setUpdatesEnabled(false);

    if (d->layout->current() == d->a) {

        d->a->deleteLater();

        d->a = nullptr;

        if (d->b->d->a && d->b->d->b) {

            dtkWidgetsLayoutItem *a = d->b->d->a;
            a->d->parent = this;
            dtkWidgetsLayoutItem *b = d->b->d->b;
            b->d->parent = this;

            d->splitter->setOrientation(d->b->d->splitter->orientation());

            d->b->deleteLater();

            d->b = nullptr;

            d->a = a;
            d->b = b;

            d->splitter->addWidget(d->a);
            d->splitter->addWidget(d->b);

            dtkWidgetsLayoutItemProxy *child = nullptr;

            if (!(child = dtkWidgetsLayoutItemPrivate::firstViewChild(d->a)))
                child = dtkWidgetsLayoutItemPrivate::firstViewChild(d->b);

            if (child)
                child->setFocus(Qt::OtherFocusReason);

        } else {

            d->proxy = new dtkWidgetsLayoutItemProxy(this);

            connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
            connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));
            connect(d->proxy, SIGNAL(create()), this, SIGNAL(create()));
            connect(d->proxy, SIGNAL(create(const QString &)), this,
                    SIGNAL(create(const QString &)));

            d->splitter->addWidget(d->proxy);

            d->proxy->setView(d->b->d->proxy->view());

            d->proxy->setFocus(Qt::OtherFocusReason);

            d->b->deleteLater();

            d->b = nullptr;

            d->footer->show();

            if (d->proxy->view())
                d->label->setCurrentText(d->proxy->view()->widget()->objectName());
        }
    }

    else if (d->layout->current() == d->b) {

        d->b->deleteLater();

        d->b = nullptr;

        if (d->a->d->a && d->a->d->b) {

            dtkWidgetsLayoutItem *a = d->a->d->a;
            a->d->parent = this;
            dtkWidgetsLayoutItem *b = d->a->d->b;
            b->d->parent = this;

            d->splitter->setOrientation(d->a->d->splitter->orientation());

            d->a->deleteLater();

            d->a = nullptr;

            d->a = a;
            d->b = b;

            d->splitter->addWidget(d->a);
            d->splitter->addWidget(d->b);

            dtkWidgetsLayoutItemProxy *child = nullptr;

            if (!(child = dtkWidgetsLayoutItemPrivate::firstViewChild(d->a)))
                child = dtkWidgetsLayoutItemPrivate::firstViewChild(d->b);

            if (child)
                child->setFocus(Qt::OtherFocusReason);

        } else {

            d->proxy = new dtkWidgetsLayoutItemProxy(this);

            connect(d->proxy, SIGNAL(focusedIn()), this, SLOT(onFocusedIn()));
            connect(d->proxy, SIGNAL(focusedOut()), this, SLOT(onFocusedOut()));
            connect(d->proxy, SIGNAL(create()), this, SIGNAL(create()));
            connect(d->proxy, SIGNAL(create(const QString &)), this,
                    SIGNAL(create(const QString &)));

            d->splitter->addWidget(d->proxy);

            d->proxy->setView(d->a->d->proxy->view());
            d->proxy->setFocus(Qt::OtherFocusReason);

            d->a->deleteLater();

            d->a = nullptr;

            d->footer->show();

            d->label->setCurrentText(d->proxy->view()->widget()->objectName());
        }

    } else {

        qDebug() << Q_FUNC_INFO << "Unhandled case.";
    }

    d->root->setUpdatesEnabled(true);
}

void dtkWidgetsLayoutItem::maximize(void)
{
    if (d->a && d->b)
        return;

    if (this == d->root)
        return;

    d->root->setUpdatesEnabled(false);

    d->root->d->proxy = new dtkWidgetsLayoutItemProxy(d->root);

    d->root->connect(d->root->d->proxy, SIGNAL(focusedIn()), d->root, SLOT(onFocusedIn()));
    d->root->connect(d->root->d->proxy, SIGNAL(focusedOut()), d->root, SLOT(onFocusedOut()));
    d->root->connect(d->root->d->proxy, SIGNAL(create()), d->root, SIGNAL(create()));
    d->root->connect(d->root->d->proxy, SIGNAL(create(const QString &)), d->root,
                     SIGNAL(create(const QString &)));

    d->root->d->splitter->addWidget(d->root->d->proxy);

    d->root->d->proxy->setView(d->proxy->view());
    d->root->d->proxy->setFocus(Qt::OtherFocusReason);

    d->root->d->footer->show();

    d->root->setUpdatesEnabled(true);

    d->root->d->a->deleteLater();
    d->root->d->b->deleteLater();

    d->root->d->a = nullptr;
    d->root->d->b = nullptr;
}

QFrame *dtkWidgetsLayoutItem::footer(void)
{
    return d->footer;
}

void dtkWidgetsLayoutItem::onActivated(const QString text)
{
    if (d->a && d->b)
        return;

    dtkWidgetsWidget *view = dtkWidgetsController::instance()->view(text);

    d->proxy->setView(view);
    d->proxy->update();
}

void dtkWidgetsLayoutItem::onFocusedIn(void)
{
    if (d->layout->current())
        d->layout->current()->onFocusedOut();

    d->layout->setCurrent(this);

    d->footer->setObjectName("dtkWidgetsLayoutItemFooterFocused");
    d->footer->style()->unpolish(d->footer);
    d->footer->style()->polish(d->footer);
    d->footer->update();
}

void dtkWidgetsLayoutItem::onFocusedOut(void)
{
    d->footer->setObjectName("dtkWidgetsLayoutItemFooterUnfocused");
    d->footer->style()->unpolish(d->footer);
    d->footer->style()->polish(d->footer);
    d->footer->update();
}

void dtkWidgetsLayoutItem::setActions(const dtkWidgetsLayoutItem::Actions &actions)
{
    dtkWidgetsLayoutItem::actions = actions;
}

void dtkWidgetsLayoutItem::close(void)
{
    this->onFocusedIn();

    if (d->parent)
        d->parent->unsplit();
    else
        clear();
}

void dtkWidgetsLayoutItem::horzt(void)
{
    this->onFocusedIn();

    d->splitter->setOrientation(Qt::Horizontal);

    this->split();
}

void dtkWidgetsLayoutItem::vertc(void)
{
    this->onFocusedIn();

    d->splitter->setOrientation(Qt::Vertical);

    this->split();
}

void dtkWidgetsLayoutItem::maxmz(void)
{
    this->onFocusedIn();

    this->maximize();
}

void dtkWidgetsLayoutItem::setView(dtkWidgetsWidget *view)
{
    d->proxy->setView(view);
    d->proxy->update();

    this->update();
}

void dtkWidgetsLayoutItem::enterEvent(QEvent *event)
{
    QString current_text = d->label->currentText();

    QStringList items = dtkWidgetsController::instance()->viewNames();
    items.sort();

    d->label->blockSignals(true);
    d->label->clear();
    d->label->addItems(items);
    int index = d->label->findText(current_text);
    d->label->setCurrentIndex(index);
    d->label->blockSignals(false);

    QFrame::enterEvent(event);
}

void dtkWidgetsLayoutItem::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void dtkWidgetsLayoutItem::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void dtkWidgetsLayoutItem::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void dtkWidgetsLayoutItem::dropEvent(QDropEvent *event)
{
    Q_UNUSED(event);

    if (d->a && d->b)
        return;

    if (d->proxy->view())
        return;

    if (dtkWidgetsWidget *view =
                dtkWidgetsController::instance()->view(event->mimeData()->text())) {
        view->setObjectName(event->mimeData()->text());

        this->setView(view);
    }
}

void dtkWidgetsLayoutItem::keyPressEvent(QKeyEvent *event)
{
    event->ignore();

    QFrame::keyPressEvent(event);
}

void dtkWidgetsLayoutItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    this->onFocusedIn();
}

dtkWidgetsLayoutItem::Actions dtkWidgetsLayoutItem::actions = dtkWidgetsLayoutItem::Actions();

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

#include "dtkWidgetsLayoutItem.moc"

//
// dtkWidgetsLayoutItem.cpp ends here
