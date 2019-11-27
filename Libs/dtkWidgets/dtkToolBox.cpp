// Version: $Id: 935185779e3b72ff97f3e4abafe89478feaca054 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkToolBox.h"
#include "dtkPropertyEditorFactory.h"

#include <QDebug>

// ///////////////////////////////////////////////////////////////////
// dtkToolBoxButton implementation
// ///////////////////////////////////////////////////////////////////

dtkToolBoxButton::dtkToolBoxButton(QWidget *parent) : QAbstractButton(parent)
{
    this->setBackgroundRole(QPalette::Window);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    this->setFocusPolicy(Qt::NoFocus);

    this->m_selected = false;
}

QSize dtkToolBoxButton::sizeHint(void) const
{
    QSize iconSize(8, 8);

    if (!this->icon().isNull()) {
        int icone = this->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this->parentWidget() /* QToolBox */);
        iconSize += QSize(icone + 2, icone);
    }

    QSize textSize = fontMetrics().size(Qt::TextShowMnemonic, text()) + QSize(0, 8);

    QSize total(iconSize.width() + textSize.width(), qMax(iconSize.height(), textSize.height()));

    return total.expandedTo(QApplication::globalStrut());
}

QSize dtkToolBoxButton::minimumSizeHint(void) const
{
    if (this->icon().isNull())
        return QSize();

    int icon = this->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this->parentWidget());

    return QSize(icon + 8, icon + 8);
}

void dtkToolBoxButton::initStyleOption(QStyleOptionToolBox *option) const
{
    if (!option)
        return;

    option->initFrom(this);

    if (this->m_selected)
        option->state |= QStyle::State_Selected;

    if (this->isDown())
        option->state |= QStyle::State_Sunken;

    option->text = this->text();
    option->icon = this->icon();
}

void dtkToolBoxButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter paint(this);
    QPainter *p = &paint;
    QStyleOptionToolBox opt;

    this->initStyleOption(&opt);
    this->style()->drawControl(QStyle::CE_ToolBoxTab, &opt, p, this->parentWidget());
}

// ///////////////////////////////////////////////////////////////////
// dtkToolBoxItemPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkToolBoxItemPrivate
{
public:
    QVBoxLayout *layout;

public:
    dtkToolBoxButton *button;

public:
    QWidget *widget;

public:
    bool is_expanded;
    bool is_enforced;

public:
    dtkToolBox *box;
};

// ///////////////////////////////////////////////////////////////////
// dtkToolBoxItem implementation
// ///////////////////////////////////////////////////////////////////

dtkToolBoxItem::dtkToolBoxItem(QWidget *parent) : QFrame(parent), d(new dtkToolBoxItemPrivate)
{
    d->layout = new QVBoxLayout(this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setAlignment(Qt::AlignTop);

    d->button = new dtkToolBoxButton(this);
    d->button->show();

    d->layout->addWidget(d->button);

    d->widget = NULL;
    d->is_expanded = false;
    d->is_enforced = false;
    d->box = NULL;

    this->setBackgroundRole(QPalette::Button);
    this->connect(d->button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
}

dtkToolBoxItem::~dtkToolBoxItem(void)
{
    delete d;

    d = NULL;
}

bool dtkToolBoxItem::isExpanded(void) const
{
    return d->is_expanded;
}

bool dtkToolBoxItem::isEnforced(void) const
{
    return d->is_enforced;
}

QString dtkToolBoxItem::name(void) const
{
    return d->button->text();
}

void dtkToolBoxItem::showButton(void)
{
    d->button->show();
}

void dtkToolBoxItem::hideButton(void)
{
    d->button->hide();
}

void dtkToolBoxItem::setWidget(QWidget *widget, const QString& text, const QIcon& icon)
{
    if (!widget) {
        qDebug() << Q_FUNC_INFO << "Widget is not valid, nothing is done.";
        return;
    }

    d->button->setText(text);
    d->button->setIcon(icon);

    d->widget = widget;

    if (!d->is_expanded)
        d->widget->hide();

    d->layout->addWidget(d->widget);

    if (d->button->isVisible())
        this->resize(d->button->size() + d->widget->size());
    else
        this->resize(d->widget->size());
}

void dtkToolBoxItem::setExpanded(bool expanded)
{
    d->is_expanded = expanded;

    if (!d->widget) {
        qDebug() << Q_FUNC_INFO << "No widget, nothing is done.";
        return;
    }

    if (expanded) {
        d->widget->show();
    } else {
        d->widget->hide();
    }

    d->button->setSelected(expanded);
}

void dtkToolBoxItem::setEnforced(bool enforced)
{
    if (d->is_enforced == enforced)
        return;

    d->is_enforced = enforced;

    if (enforced) {
        this->disconnect(d->button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
        this->setExpanded(true);

    } else {
        this->connect(d->button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
        this->setExpanded(d->is_expanded);
    }
}

void dtkToolBoxItem::setName(const QString& name)
{
    d->button->setText(name);
}

void dtkToolBoxItem::onButtonClicked(void)
{
    this->setExpanded(!d->is_expanded);

    if (d->box)
        d->box->setCurrentItem(this);
}

void dtkToolBoxItem::setToolBox(dtkToolBox *box)
{
    d->box = box;
}

// ///////////////////////////////////////////////////////////////////

dtkToolBoxItem *dtkToolBoxItem::fromObject(QObject *object, int hierarchy_level)
{
    QList<QWidget *> list = dtkPropertyEditorFactory::instance()->createObjectProperties(object, hierarchy_level);

    QFrame *frame = new QFrame;

    QVBoxLayout *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->setAlignment(Qt::AlignTop);

    QString name;

    foreach (QWidget *w, list) {
        name = QString(w->objectName()).append(":");
        layout->addWidget(new QLabel(name, frame));
        layout->addWidget(w);
    }

    frame->adjustSize();

    dtkToolBoxItem *item = new dtkToolBoxItem;
    item->setWidget(frame, qPrintable(object->objectName()));

    return item;
}

// ///////////////////////////////////////////////////////////////////
// dtkToolBoxPrivate declaration and implementation
// ///////////////////////////////////////////////////////////////////

class dtkToolBoxPrivate
{
public:
    dtkToolBox::Order order;
    dtkToolBox::DisplayMode mode;

public:
    dtkToolBoxItem *current_item;

public:
    typedef QList<dtkToolBoxItem *> ItemList;
    ItemList items;

public:
    QFrame *frame;
    QVBoxLayout *layout;

public:
    void clear(void);
    void relayout(void);
    void insert(int index, dtkToolBoxItem *item);
    void setCurrentItem(dtkToolBoxItem *item);
};

// ///////////////////////////////////////////////////////////////////

void dtkToolBoxPrivate::clear(void)
{
    delete this->layout;
    this->layout = NULL;

    qDeleteAll(this->items);
    this->items.clear();
}

void dtkToolBoxPrivate::relayout(void)
{
    if (this->layout)
        delete this->layout;

    this->layout = new QVBoxLayout(this->frame);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(0);
    this->layout->setAlignment(Qt::AlignTop);

    switch (this->order) {

    case dtkToolBox::Ascending: {

        ItemList::ConstIterator it = items.constBegin();

        while (it != items.constEnd()) {
            this->layout->addWidget(*it);
            ++it;
        }

        break;
    }

    case dtkToolBox::Descending: {

        ItemList::ConstIterator it = items.constEnd();

        while (it != items.constBegin()) {
            --it;
            this->layout->addWidget(*it);
        }

        break;
    }

    case dtkToolBox::AlphaBetics: {

        QMultiMap<QString, dtkToolBoxItem *> map;

        ItemList::ConstIterator it = items.constBegin();

        while (it != items.constEnd()) {
            dtkToolBoxItem *item = *it;
            map.insert(item->name(), item);
            ++it;
        }

        QMultiMap<QString, dtkToolBoxItem *>::ConstIterator i = map.constBegin();

        while (i != map.constEnd()) {
            this->layout->addWidget(i.value());
            ++i;
        }

        break;
    }

    default: {

        ItemList::ConstIterator it = items.constBegin();

        while (it != items.constEnd()) {
            this->layout->addWidget(*it);
            ++it;
        }

        break;
    }
    }
}

void dtkToolBoxPrivate::insert(int index, dtkToolBoxItem *item)
{
    switch (this->order) {

    case dtkToolBox::Ascending: {

        this->layout->insertWidget(index, item);

        break;
    }

    case dtkToolBox::Descending: {

        this->layout->insertWidget(this->items.count() - index, item);

        break;
    }

    case dtkToolBox::AlphaBetics: {

        this->relayout();

        break;
    }

    default: {
        this->layout->insertWidget(index, item);
        break;
    }
    }
}

void dtkToolBoxPrivate::setCurrentItem(dtkToolBoxItem *item)
{
    if (this->current_item == item)
        this->current_item = NULL;
    else
        this->current_item = item;


    if (this->mode == dtkToolBox::OneItemExpanded) {
        foreach (dtkToolBoxItem *it, this->items) {
            it->setExpanded(false);
        }

        if (this->current_item)
            this->current_item->setExpanded(true);
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkToolBox implementation
// ///////////////////////////////////////////////////////////////////

dtkToolBox::dtkToolBox(QWidget *parent) : QScrollArea(parent), d(new dtkToolBoxPrivate)
{
    d->frame = new QFrame(this);

    this->setWidget(d->frame);
    this->setWidgetResizable(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setContentsMargins(0, 0, 0, 0);

    d->layout = new QVBoxLayout(d->frame);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);
    d->layout->setAlignment(Qt::AlignTop);

    d->order = dtkToolBox::Ascending;
    d->mode = dtkToolBox::Default;
    d->current_item = NULL;
}

dtkToolBox::~dtkToolBox(void)
{
    delete d;

    d = NULL;
}

void dtkToolBox::clear(void)
{
    d->clear();
    d->relayout();
}

int dtkToolBox::count(void) const
{
    return d->items.count();
}

dtkToolBox::Order dtkToolBox::order(void) const
{
    return d->order;
}

dtkToolBox::DisplayMode dtkToolBox::displayMode(void) const
{
    return d->mode;
}

dtkToolBoxItem *dtkToolBox::itemAt(int index) const
{
    return d->items.at(index);
}

void dtkToolBox::insertItem(int index, dtkToolBoxItem *item)
{
    if (d->items.contains(item)) {
        qDebug() << Q_FUNC_INFO << "Item is already in the toolbox.";
        return;
    }

    item->setToolBox(this);
    item->setEnforced((d->mode == AllItemExpanded));
    d->items.insert(index, item);
    d->insert(index, item);
}

void dtkToolBox::setCurrentItem(dtkToolBoxItem *item)
{
    d->setCurrentItem(item);
}

void dtkToolBox::removeItem(int index)
{
    d->layout->removeWidget(d->items.takeAt(index));
}

void dtkToolBox::removeItem(dtkToolBoxItem *item)
{
    this->removeItem(d->items.indexOf(item));
}

void dtkToolBox::setOrder(Order order)
{
    if (d->order == order)
        return;

    d->order = order;
    d->relayout();
}

void dtkToolBox::setDisplayMode(DisplayMode mode)
{
    if (d->mode == mode)
        return;

    d->mode = mode;

    bool display_all_items = false;

    switch (mode) {
    case Default:
        break;

    case AllItemExpanded:
        display_all_items = true;
        break;

    case OneItemExpanded:
        d->setCurrentItem(d->current_item);
        break;

    default:
        break;
    }

    foreach (dtkToolBoxItem *item, d->items)
        item->setEnforced(display_all_items);
}

//
// dtkToolBox.cpp ends here
