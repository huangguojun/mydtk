// Version: $Id: 3860690ee28d2bdfc43d95282fa50f66c001cd7e $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsTagCloudList.h"
#include "dtkWidgetsTagCloudItem.h"

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListLightDelegate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudListLightDelegate: public QStyledItemDelegate
{
public:
    dtkWidgetsTagCloudListLightDelegate(dtkWidgetsTagCloudList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
    dtkWidgetsTagCloudList *list;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListDarkDelegate
// /////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudListDarkDelegate: public QStyledItemDelegate
{
public:
    dtkWidgetsTagCloudListDarkDelegate(dtkWidgetsTagCloudList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
    dtkWidgetsTagCloudList *list;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListDoomDelegate
// /////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudListDoomDelegate: public QStyledItemDelegate
{
public:
    dtkWidgetsTagCloudListDoomDelegate(dtkWidgetsTagCloudList *list);

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
    dtkWidgetsTagCloudList *list;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudListPrivate
{
public:
    QList<dtkWidgetsTagCloudItem *> items;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudList
// ///////////////////////////////////////////////////////////////////

dtkWidgetsTagCloudList::dtkWidgetsTagCloudList(QWidget *parent) : QListWidget(parent)
{
    d = new dtkWidgetsTagCloudListPrivate;

    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFrameShape(QFrame::NoFrame);
    this->setDragEnabled(true);
    this->setItemDelegate(new dtkWidgetsTagCloudListLightDelegate(this));
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onItemClicked(QListWidgetItem *)));
}

dtkWidgetsTagCloudList::~dtkWidgetsTagCloudList(void)
{
    delete d;
}

void dtkWidgetsTagCloudList::addItem(QString name)
{
    d->items << new dtkWidgetsTagCloudItem(name, QString(), QStringList());

    QListWidget::addItem(d->items.last());
}

void dtkWidgetsTagCloudList::addItem(QString name, QString description)
{
    d->items << new dtkWidgetsTagCloudItem(name, description, QStringList());

    QListWidget::addItem(d->items.last());
}

void dtkWidgetsTagCloudList::addItem(QString name, QString description, QStringList tags)
{
    d->items << new dtkWidgetsTagCloudItem(name, description, tags);

    QListWidget::addItem(d->items.last());
}

void dtkWidgetsTagCloudList::addItem(QString name, QString description, QStringList tags, QString kind, QString type)
{
    d->items << new dtkWidgetsTagCloudItem(name, description, tags, kind, type);

    QListWidget::addItem(d->items.last());
}

void dtkWidgetsTagCloudList::addItem(dtkWidgetsTagCloudItem item)
{
    d->items << new dtkWidgetsTagCloudItem(item);

    QListWidget::addItem(d->items.last());
}

void dtkWidgetsTagCloudList::clear(void)
{
    d->items.clear();

    QListWidget::clear();
}

void dtkWidgetsTagCloudList::setDark(void)
{
    this->setItemDelegate(new dtkWidgetsTagCloudListDarkDelegate(this));
}

void dtkWidgetsTagCloudList::setDoom(void)
{
    this->setItemDelegate(new dtkWidgetsTagCloudListDoomDelegate(this));
}

void dtkWidgetsTagCloudList::onItemClicked(QListWidgetItem *item)
{
    dtkWidgetsTagCloudItem *i = dynamic_cast<dtkWidgetsTagCloudItem *>(item);

    if (!i)
        return;

    emit itemClicked(i->description());
}

QMimeData *dtkWidgetsTagCloudList::mimeData(const QList<QListWidgetItem *> items) const
{
    QMimeData *data = NULL;

    dtkWidgetsTagCloudItem *i = dynamic_cast<dtkWidgetsTagCloudItem *>(items.first());

    if (i) {
        data = new QMimeData;
        data->setUrls(QList<QUrl>() << QUrl(QString("%1:%2").arg(i->kind()).arg(i->type())));
    }

    return data;
}

QStringList dtkWidgetsTagCloudList::mimeTypes(void) const
{
    return QStringList() << "text/uri-list";
}

// ///////////////////////////////////////////////////////////////////
// Helper functions
// ///////////////////////////////////////////////////////////////////

QString dtkWidgetsTagCloudListDelegateUnhtmlize(const QString& htmlString)
{
    QString textString;

    QXmlStreamReader xml(htmlString);

    while (!xml.atEnd())
        if (xml.readNext() == QXmlStreamReader::Characters)
            textString += xml.text();

    return textString;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListLightDelegate
// ///////////////////////////////////////////////////////////////////

dtkWidgetsTagCloudListLightDelegate::dtkWidgetsTagCloudListLightDelegate(dtkWidgetsTagCloudList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkWidgetsTagCloudListLightDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkWidgetsTagCloudItem *item = dynamic_cast<dtkWidgetsTagCloudItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    painter->fillRect(option.rect, QColor("#efefef"));
    painter->setPen(Qt::white);
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-arrow.png");
    static QPixmap tags = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,           w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,      w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::black);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::gray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkWidgetsTagCloudListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(Qt::darkGray);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkWidgetsTagCloudListLightDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListDarkDelegate
// ///////////////////////////////////////////////////////////////////

dtkWidgetsTagCloudListDarkDelegate::dtkWidgetsTagCloudListDarkDelegate(dtkWidgetsTagCloudList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkWidgetsTagCloudListDarkDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkWidgetsTagCloudItem *item = dynamic_cast<dtkWidgetsTagCloudItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    painter->fillRect(option.rect, QColor(020, 020, 020));
    painter->setPen(QColor(055, 055, 055));
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-arrow.png");
    static QPixmap tags = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,           w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,      w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::lightGray);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::darkGray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkWidgetsTagCloudListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(Qt::black);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkWidgetsTagCloudListDarkDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudListDoomDelegate
// ///////////////////////////////////////////////////////////////////

dtkWidgetsTagCloudListDoomDelegate::dtkWidgetsTagCloudListDoomDelegate(dtkWidgetsTagCloudList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkWidgetsTagCloudListDoomDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkWidgetsTagCloudItem *item = dynamic_cast<dtkWidgetsTagCloudItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    painter->fillRect(option.rect, QColor("#282C34"));
    painter->setPen(QColor(055, 055, 055));
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-arrow.png");
    static QPixmap tags = QPixmap(":dtkWidgetsTagCloud/dtkWidgetsTagCloudList-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,           w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,      w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::lightGray);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::darkGray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkWidgetsTagCloudListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(Qt::black);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkWidgetsTagCloudListDoomDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

//
// dtkWidgetsTagCloudList.cpp ends here
