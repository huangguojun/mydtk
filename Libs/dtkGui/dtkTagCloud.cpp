/* dtkTagCloud.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sun May  3 10:42:27 2009 (+0200)
 * Version: $Id: dfd1a3640cc8f8a50dedd4d0359163fcc953b941 $
 * Last-Updated: lun. févr.  3 15:40:07 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 1634
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkFlowLayout.h"
#include "dtkSwitch.h"
#include "dtkTagCloud.h"

#include <QtDebug>
#include <QWidget>

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

bool dtkTagAlphaLessThan(const dtkTag& t1, const dtkTag& t2)
{
    return t1.text() < t2.text();
}

bool dtkTagNumLessThan(const dtkTag& t1, const dtkTag& t2)
{
    return t1.count() < t2.count();
}

bool dtkTagAlphaMoreThan(const dtkTag& t1, const dtkTag& t2)
{
    return t1.text() >= t2.text();
}

bool dtkTagNumMoreThan(const dtkTag& t1, const dtkTag& t2)
{
    return t1.count() >= t2.count();
}

// /////////////////////////////////////////////////////////////////
// dtkTagCloudHasher
// /////////////////////////////////////////////////////////////////

class dtkTagCloudHasher
{
public:
    dtkTagCloudHasher(int buckets, int min, int max) {
        if (buckets < 1)
            qDebug() << "dtkTagCloudHasher: Must have at least one bucket.";

        this->buckets = buckets;
        this->min = min;
        this->max = max + 1;

        this->width = ((double)(this->max - this->min)) / ((double)(this->buckets));
    }

    int bucket(dtkTag tag) {
        return ((float)(tag.count() - this->min)) / ((float)(this->width));
    }

private:
    int min, max, buckets;

private:
    double width;
};

// /////////////////////////////////////////////////////////////////
// dtkTagPrivate
// /////////////////////////////////////////////////////////////////

class dtkTagPrivate
{
public:
    int count;
    QString text;
    QString color;
    QStringList items;
};

// /////////////////////////////////////////////////////////////////
// dtkTag
// /////////////////////////////////////////////////////////////////

dtkTag::dtkTag(void) : d(new dtkTagPrivate)
{

}

dtkTag::dtkTag(QString text, int count) : d(new dtkTagPrivate)
{
    d->text = text;
    d->count = count;
}

dtkTag::dtkTag(QString text, int count, QStringList items) : d(new dtkTagPrivate)
{
    d->text = text;
    d->count = count;
    d->items = items;
}

dtkTag::dtkTag(QString text, int count, QStringList items, QString color) : d(new dtkTagPrivate)
{
    d->text = text;
    d->count = count;
    d->items = items;
    d->color = color;
}

dtkTag::dtkTag(const dtkTag& other) : d(new dtkTagPrivate)
{
    d->text = other.d->text;
    d->count = other.d->count;
    d->items = other.d->items;
    d->color = other.d->color;
}

dtkTag::~dtkTag(void)
{
    delete d;

    d = NULL;
}

int dtkTag::count(void) const
{
    return d->count;
}

QString dtkTag::text(void) const
{
    return d->text;
}

QStringList dtkTag::items(void) const
{
    return d->items;
}

QString dtkTag::color(void) const
{
    return d->color;
}

void dtkTag::setCount(int count)
{
    if (count > 0)
        d->count = count;
    else
        d->count = count;
}

void dtkTag::setText(QString text)
{
    if (!text.isNull())
        d->text = text;
    else
        d->text = "";
}

void dtkTag::setItems(QStringList items)
{
    if (!items.isEmpty())
        d->items = items;
    else
        d->items = QStringList();
}

void dtkTag::setColor(QString color)
{
    if (!color.isNull())
        d->color = color;
    else
        d->color = "";
}

// /////////////////////////////////////////////////////////////////
// dtkTagCloudPrivate
// /////////////////////////////////////////////////////////////////

class dtkTagCloudPrivate
{
public:
    QList<dtkTag> tags;

    int averageFontSize;
    int fontSizeRange;
    int mincount;
    int maxcount;
    int tagCount;

    dtkTagCloud::SortingType  sortingType;
    dtkTagCloud::SortingOrder sortingOrder;
};

// /////////////////////////////////////////////////////////////////
// dtkTagCloud
// /////////////////////////////////////////////////////////////////

dtkTagCloud::dtkTagCloud(QWidget *parent) : QTextBrowser(parent)
{
    d = new dtkTagCloudPrivate;

    d->averageFontSize = 0;
    d->fontSizeRange   = 0;

    d->sortingType  = Alpha;
    d->sortingOrder = Asc;

    this->setFrameShape(QFrame::NoFrame);

    connect(this, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(onLinkClicked(const QUrl&)));
}

dtkTagCloud::~dtkTagCloud(void)
{
    delete d;

    d = NULL;
}

void dtkTagCloud::addTag(QString text, int count)
{
    d->tags << dtkTag(text, count);
}

void dtkTagCloud::addTag(QString text, int count, QStringList items)
{
    d->tags << dtkTag(text, count, items);
}

void dtkTagCloud::addTag(QString text, int count, QStringList items, QString color)
{
    d->tags << dtkTag(text, count, items, color);
}

void dtkTagCloud::addTag(dtkTag tag)
{
    d->tags << tag;
}

void dtkTagCloud::setFontSize(int size)
{
    d->averageFontSize = (size > 0) ? size : 0;
}

void dtkTagCloud::setFontRange(int range)
{
    d->fontSizeRange = (range > 0) ? range : 0;
}

void dtkTagCloud::setSortingType(SortingType type)
{
    d->sortingType = type;
}

void dtkTagCloud::setSortingOrder(SortingOrder order)
{
    d->sortingOrder = order;
}

void dtkTagCloud::sort(void)
{
    if (d->sortingType == Alpha && d->sortingOrder == Asc)
        qSort(d->tags.begin(), d->tags.end(), dtkTagAlphaLessThan);

    if (d->sortingType == Alpha && d->sortingOrder == Desc)
        qSort(d->tags.begin(), d->tags.end(), dtkTagAlphaMoreThan);

    if (d->sortingType == Num && d->sortingOrder == Asc)
        qSort(d->tags.begin(), d->tags.end(), dtkTagNumLessThan);

    if (d->sortingType == Num && d->sortingOrder == Desc)
        qSort(d->tags.begin(), d->tags.end(), dtkTagNumMoreThan);
}

void dtkTagCloud::clear(void)
{
    d->tags.clear();

    this->render();
}

void dtkTagCloud::render(void)
{
    if (d->averageFontSize <= 0)
        qDebug() << "AverageFontSize must be non-zero and non-negative.";

    if (d->fontSizeRange <= 0)
        qDebug() << "FontSizeRange must be non-zero and non-negative.";

    if (d->tags.isEmpty())
        return;

    d->mincount = 100000000;
    d->maxcount = 0;
    d->tagCount = 0;

    foreach (dtkTag tag, d->tags) {

        if (tag.count() < d->mincount)
            d->mincount = tag.count();

        if (tag.count() > d->maxcount)
            d->maxcount = tag.count();

        d->tagCount++;
    }

    dtkTagCloudHasher hasher(d->fontSizeRange, d->mincount, d->maxcount);

    int baseFontSize = d->averageFontSize - ((double)(d->fontSizeRange - 1) / 2);

    QString cloud; cloud.append(QString("<div align=\"justify\">\n"));

    foreach (dtkTag tag, d->tags) {

        int fontSize = baseFontSize + hasher.bucket(tag);

        QString color = "";

        if (!tag.color().isEmpty()) {
            color  = "color: ";
            color += (!tag.color().startsWith("#")) ? "#" : "";
            color += tag.color();
            color += ";";
        } else {
            color = "color: #6a769d;";
        }

        QString count = QString::number(tag.count()) + " item" + ((tag.count() != 1) ? "s" : "");

        cloud.append(QString("<a href=\"tag://%1\" title=\"%2\" style=\"font-size: %4px; text-decoration: none; %5\" item=\"%3\">%1</a> ")
                     .arg(tag.text())
                     .arg(count)
                     .arg(tag.items().first())
                     .arg(fontSize)
                     .arg(color));
    }

    cloud.append("</div>\n");

    this->setHtml(cloud);
}

void dtkTagCloud::onLinkClicked(const QUrl& url)
{
    emit tagClicked(url.host());
}

// /////////////////////////////////////////////////////////////////
// dtkTagScopeTag
// /////////////////////////////////////////////////////////////////

class dtkTagScopeTagPrivate
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

dtkTagScopeTag::dtkTagScopeTag(QWidget *parent) : QWidget(parent), d(new dtkTagScopeTagPrivate)
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
    d->bg->setStyleSheet(
        "border-image: url(:dtkGui/pixmaps/dtk-tag_bg.png) 3 10 3 10;"
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
    d->fg->setStyleSheet(
        "border-image: url(:dtkGui/pixmaps/dtk-tag_fg.png) 3 10 3 10;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 10px transparent;"
        "border-left: 10px transparent;");
    d->fg->raise();

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

dtkTagScopeTag::~dtkTagScopeTag(void)
{
    delete d;

    d = NULL;
}

void dtkTagScopeTag::setBlue(void)
{
    d->fg->setStyleSheet(
        "border-image: url(:dtkGui/pixmaps/dtk-tag_fg_blue.png) 3 10 3 10;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 10px transparent;"
        "border-left: 10px transparent;");
}

void dtkTagScopeTag::setDark(void)
{
    d->fg->setStyleSheet(
        "border-image: url(:dtkGui/pixmaps/dtk-tag_fg_dark.png) 3 10 3 10;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 10px transparent;"
        "border-left: 10px transparent;");
}

QSize dtkTagScopeTag::sizeHint(void) const
{
    return QSize(d->width + d->offset_max, d->height);
}

QString dtkTagScopeTag::text(void)
{
    return d->fg->text();
}

void dtkTagScopeTag::setText(const QString& text)
{
    d->fg->setText(text);

    static int   margin = 2;
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

void dtkTagScopeTag::setCount(int count)
{
    d->bg->setText(QString::number(count));
}

void dtkTagScopeTag::enterEvent(QEvent *event)
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

void dtkTagScopeTag::leaveEvent(QEvent *event)
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

void dtkTagScopeTag::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}

// /////////////////////////////////////////////////////////////////
// dtkTagScope
// /////////////////////////////////////////////////////////////////

class dtkTagScopePrivate
{
public:
    QStringList filters;
    QLineEdit *edit;
    QToolButton *clear;
    dtkSwitch *switsh;

public:
    QList<dtkTagScopeTag *> tags;

public:
    QHash<QString, int> counts;

public:
    QCompleter *completer;
    QStringListModel *completer_model;

public:
    dtkFlowLayout *layout;

public:
    bool light;
    bool dark;
};

dtkTagScope::dtkTagScope(QWidget *parent) : QFrame(parent)
{
    Q_INIT_RESOURCE(dtkGui);

    d = new dtkTagScopePrivate;

    d->light = true;
    d->dark = false;

    d->completer_model = new QStringListModel(this);

    d->completer = new QCompleter(this);
    d->completer->setModel(d->completer_model);
    d->completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);

    d->edit = new QLineEdit(this);
    d->edit->setFixedHeight(21);
    d->edit->setAttribute(Qt::WA_MacShowFocusRect, false);
    d->edit->setCompleter(d->completer);

    d->clear = new QToolButton(this);
    d->clear->setFixedHeight(21);
    d->clear->setAttribute(Qt::WA_MacShowFocusRect, false);

    d->switsh = new dtkSwitch();
    d->switsh->setMaximumHeight(21);
    d->switsh->setMinimumHeight(21);
    d->switsh->setMaximumWidth(39);
    d->switsh->setMinimumWidth(39);
    //QWidget *switsh_container = QWidget::createWindowContainer(d->switsh, this);
    //d->switsh->setAttribute(Qt::WA_MacShowFocusRect, false);

    QHBoxLayout *t_layout = new QHBoxLayout;
    t_layout->setContentsMargins(5, 5, 5, 5);
    // t_layout->setSpacing(0);
    t_layout->addWidget(d->edit);
    t_layout->addWidget(d->clear);
    //t_layout->addWidget(switsh_container);

    d->layout = new dtkFlowLayout;
    d->layout->setContentsMargins(5, 5, 5, 5);
    d->layout->setSpacing(0);

    QFrame *header = new QFrame(this);
    header->setObjectName("dtkTagScopeHeader");
    header->setLayout(t_layout);

    QFrame *well = new QFrame(this);
    well->setObjectName("dtkTagScopeWell");
    well->setLayout(d->layout);
    well->setMinimumHeight(50);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(header);
    layout->addWidget(well);

    this->setFrameShape(QFrame::NoFrame);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(d->edit, SIGNAL(returnPressed()), this, SLOT(onTagAdded()));
    connect(d->clear, SIGNAL(clicked()), this, SIGNAL(cleared()));
    connect(d->switsh, SIGNAL(toggled(bool)), this, SIGNAL(unionMode(bool)));
}

dtkTagScope::~dtkTagScope(void)
{
    delete d->clear;
    delete d->edit;
    delete d;
}

QSize dtkTagScope::sizeHint(void) const
{
    return QSize(100, 24);
}

void dtkTagScope::clear(void)
{
    d->filters.clear();
    d->counts.clear();
    d->edit->clear();
}

void dtkTagScope::setBlue(void)
{
    d->light = false;
    d->dark = false;
}

void dtkTagScope::setDark(void)
{
    d->light = false;
    d->dark = true;
}

void dtkTagScope::toggle(void)
{
    d->switsh->toggle();
}


void dtkTagScope::render(void)
{
    foreach (dtkTagScopeTag *tag, d->tags) {
        d->layout->removeWidget(tag);
    }

    qDeleteAll(d->tags); d->tags.clear();

    foreach (QString filter, d->filters) {
        dtkTagScopeTag *tag = new dtkTagScopeTag;

        if (!d->light && !d->dark)
            tag->setBlue();

        if (!d->light && d->dark)
            tag->setDark();

        tag->setText(filter);

        if (d->counts.contains(filter))
            tag->setCount(d->counts[filter]);

        d->tags << tag;
        d->layout->addWidget(tag);

        connect(tag, SIGNAL(clicked()), this, SLOT(onTagRemoved()));
    }

    this->update();
}

void dtkTagScope::addTag(QString tag)
{
    d->filters << tag;
}

void dtkTagScope::addTag(QString tag, int count)
{
    d->filters << tag;

    d->counts[tag] = count;
}

void dtkTagScope::setTags(const QStringList& tags)
{
    QList<QString> t = tags;
    qSort(t.begin(), t.end(), qLess<QString>());

    d->completer_model->setStringList(t);
}

void dtkTagScope::onTagAdded(void)
{
    QString tag = d->edit->text();

    emit tagAdded(tag);

    d->edit->clear();
}

void dtkTagScope::onTagRemoved(void)
{
    dtkTagScopeTag *tag = dynamic_cast<dtkTagScopeTag *>(sender());

    if (!tag)
        return;

    QString value = tag->text();

    emit tagRemoved(value);
}

// /////////////////////////////////////////////////////////////////
// dtkItem
// /////////////////////////////////////////////////////////////////

class dtkItemPrivate
{
public:
    QString name;
    QString description;
    QStringList tags;
    QString kind;
    QString type;
};

dtkItem::dtkItem(QString name) : QListWidgetItem(name)
{
    d = new dtkItemPrivate;
    d->name = name;
}

dtkItem::dtkItem(QString name, QString description) : QListWidgetItem(name)
{
    d = new dtkItemPrivate;
    d->name = name;
    d->description = description;
}

dtkItem::dtkItem(QString name, QString description, QStringList tags) : QListWidgetItem(name)
{
    d = new dtkItemPrivate;
    d->name = name;
    d->description = description;
    d->tags = tags;
}

dtkItem::dtkItem(QString name, QString description, QStringList tags, QString kind, QString type) : QListWidgetItem(name)
{
    d = new dtkItemPrivate;
    d->name = name;
    d->description = description;
    d->tags = tags;
    d->kind = kind;
    d->type = type;
}

dtkItem::dtkItem(const dtkItem& item) : QListWidgetItem(item.name())
{
    d = new dtkItemPrivate;
    d->name = item.d->name;
    d->description = item.d->description;
    d->tags = item.d->tags;
    d->kind = item.d->kind;
    d->type = item.d->type;
}

dtkItem::~dtkItem(void)
{
    delete d;

    d = NULL;
}

QString dtkItem::name(void) const
{
    return d->name;
}

QString dtkItem::description(void) const
{
    return d->description;
}

QStringList dtkItem::tags(void) const
{
    return d->tags;
}

QString dtkItem::kind(void) const
{
    return d->kind;
}

QString dtkItem::type(void) const
{
    return d->type;
}

// /////////////////////////////////////////////////////////////////
// dtkItemList
// /////////////////////////////////////////////////////////////////

class dtkItemListPrivate
{
public:
    QList<dtkItem *> items;
};

dtkItemList::dtkItemList(QWidget *parent) : QListWidget(parent)
{
    d = new dtkItemListPrivate;

    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFrameShape(QFrame::NoFrame);
    this->setDragEnabled(true);
    this->setItemDelegate(new dtkItemListDelegate(this));
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onItemClicked(QListWidgetItem *)));
}

dtkItemList::~dtkItemList(void)
{
    delete d;
}

void dtkItemList::addItem(QString name)
{
    d->items << new dtkItem(name, QString(), QStringList());

    QListWidget::addItem(d->items.last());
}

void dtkItemList::addItem(QString name, QString description)
{
    d->items << new dtkItem(name, description, QStringList());

    QListWidget::addItem(d->items.last());
}

void dtkItemList::addItem(QString name, QString description, QStringList tags)
{
    d->items << new dtkItem(name, description, tags);

    QListWidget::addItem(d->items.last());
}

void dtkItemList::addItem(QString name, QString description, QStringList tags, QString kind, QString type)
{
    d->items << new dtkItem(name, description, tags, kind, type);

    QListWidget::addItem(d->items.last());
}

void dtkItemList::addItem(dtkItem item)
{
    d->items << new dtkItem(item);

    QListWidget::addItem(d->items.last());
}

void dtkItemList::clear(void)
{
    d->items.clear();

    QListWidget::clear();
}

void dtkItemList::setBlue(void)
{
    this->setItemDelegate(new dtkItemBlueDelegate(this));
}

void dtkItemList::setDark(void)
{
    this->setItemDelegate(new dtkItemDarkDelegate(this));
}

void dtkItemList::onItemClicked(QListWidgetItem *item)
{
    dtkItem *i = dynamic_cast<dtkItem *>(item);

    if (!i)
        return;

    emit itemClicked(i->description());
}

QMimeData *dtkItemList::mimeData(const QList<QListWidgetItem *> items) const
{
    QMimeData *data = NULL;

    dtkItem *i = dynamic_cast<dtkItem *>(items.first());

    if (i) {
        data = new QMimeData;
        data->setUrls(QList<QUrl>() << QUrl(QString("%1:%2").arg(i->kind()).arg(i->type())));
    }

    return data;
}

QStringList dtkItemList::mimeTypes(void) const
{
    return QStringList() << "text/uri-list";
}

// /////////////////////////////////////////////////////////////////
// Helper function
// /////////////////////////////////////////////////////////////////

QString dtkItemListDelegateUnhtmlize(const QString& htmlString)
{
    QString textString;

    QXmlStreamReader xml(htmlString);

    while (!xml.atEnd())
        if (xml.readNext() == QXmlStreamReader::Characters)
            textString += xml.text();

    return textString;
}

// /////////////////////////////////////////////////////////////////
// dtkItemListDelegate
// /////////////////////////////////////////////////////////////////

dtkItemListDelegate::dtkItemListDelegate(dtkItemList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkItemListDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkItem *item = dynamic_cast<dtkItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    QLinearGradient gradiant(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.bottom());
    gradiant.setColorAt(0.0, QColor(247, 247, 247));
    gradiant.setColorAt(0.3, QColor(240, 240, 240));
    gradiant.setColorAt(1.0, QColor(220, 220, 220));

    painter->fillRect(option.rect, gradiant);

    painter->setPen(Qt::white);
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-arrow.png");
    static QPixmap tags = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,       w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,    w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::black);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::gray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkItemListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(Qt::darkGray);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkItemListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

// /////////////////////////////////////////////////////////////////
// dtkItemDarkDelegate
// /////////////////////////////////////////////////////////////////

dtkItemDarkDelegate::dtkItemDarkDelegate(dtkItemList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkItemDarkDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkItem *item = dynamic_cast<dtkItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    QLinearGradient gradiant(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.bottom());
    gradiant.setColorAt(0.0, QColor(047, 047, 047));
    gradiant.setColorAt(0.3, QColor(040, 040, 040));
    gradiant.setColorAt(1.0, QColor(020, 020, 020));

    painter->fillRect(option.rect, gradiant);

    painter->setPen(QColor(055, 055, 055));
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-arrow.png");
    static QPixmap tags = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,       w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,    w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::lightGray);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::darkGray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkItemListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(Qt::black);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkItemDarkDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

// /////////////////////////////////////////////////////////////////
// dtkItemBlueDelegate
// /////////////////////////////////////////////////////////////////

dtkItemBlueDelegate::dtkItemBlueDelegate(dtkItemList *list) : QStyledItemDelegate(list)
{
    this->list = list;
}

void dtkItemBlueDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    dtkItem *item = dynamic_cast<dtkItem *>(list->itemFromIndex(index));

    if (!item)
        return;

    QLinearGradient gradiant(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.bottom());
    gradiant.setColorAt(0.0, QColor("#60666c"));
    gradiant.setColorAt(1.0, QColor("#4b5157"));

    painter->fillRect(option.rect, gradiant);

    painter->setPen(QColor("#828589"));
    painter->drawLine(option.rect.topLeft() + QPoint(0, 1), option.rect.topRight() + QPoint(0, 1));

    static QPixmap arrow = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-arrow.png");
    static QPixmap tags = QPixmap(":dtkGui/pixmaps/dtk-item-list-delegate-tags.png");

    static int m  =  5;
    static int h1 = 20;
    static int h2 = 20;
    static int h3 = 20;

    int w = option.rect.width();
    int h = option.rect.height();
    int t = option.rect.top();
    int r = option.rect.right();

    QRect name_rect = QRect(m, t + 1 * m,       w - 2 * m, h1);
    QRect desc_rect = QRect(m, t + 2 * m + h1,    w - 6 * m, h2);
    QRect tags_rect = QRect(m, t + 3 * m + h1 + h2, w - 2 * m, h3);

    QFontMetrics metrics = QFontMetrics(painter->font());

    painter->setPen(Qt::lightGray);
    painter->drawText(name_rect, Qt::AlignLeft | Qt::AlignTop, item->name());

    painter->setPen(Qt::lightGray);
    painter->drawText(desc_rect, Qt::AlignLeft | Qt::AlignTop, metrics.elidedText(dtkItemListDelegateUnhtmlize(item->description()), Qt::ElideRight, desc_rect.width()));

    painter->setPen(QColor("#6a769d"));
    painter->drawText(tags_rect.adjusted(m + tags.width(), 0, -tags.width(), 0), Qt::AlignLeft | Qt::AlignTop, item->tags().join(", "));

    painter->setPen(QColor("#1f2025"));
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QPointF arrow_pos = QPointF(r - m - arrow.width(), t + h / 2 - arrow.height() / 2);
    painter->drawPixmap(arrow_pos, arrow);
    painter->drawPixmap(tags_rect.topLeft(), tags);
}

QSize dtkItemBlueDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(100, 80);
}

// /////////////////////////////////////////////////////////////////
// dtkItemDesc
// /////////////////////////////////////////////////////////////////

class dtkItemDescPrivate
{
public:
    QTextBrowser *browser;

public:
    QPushButton *back;
};

dtkItemDesc::dtkItemDesc(QWidget *parent) : QFrame(parent), d(new dtkItemDescPrivate)
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

dtkItemDesc::~dtkItemDesc(void)
{
    delete d;

    d = NULL;
}

void dtkItemDesc::clear(void)
{
    d->browser->clear();
}

void dtkItemDesc::setDescription(const QString& description)
{
    d->browser->setHtml(description);
}

// /////////////////////////////////////////////////////////////////
// dtkItemView
// /////////////////////////////////////////////////////////////////

class dtkItemViewPrivate
{
public:
    QWidget *parent;

public:
    QEasingCurve::Type type;

public:
    bool vertical;
    bool wrap;
    bool active;

public:
    int speed;
    int now;
    int next;

public:
    QPoint pnow;

public:
    dtkItemList *list;
    dtkItemDesc *desc;
};

dtkItemView::dtkItemView(QWidget *parent) : QStackedWidget(parent), d(new dtkItemViewPrivate)
{
    d->list = new dtkItemList(this);
    d->desc = new dtkItemDesc(this);

    if (parent != 0)
        d->parent = parent;
    else
        d->parent = this;

    d->vertical = false;
    d->speed = 500;
    d->type = QEasingCurve::OutBack;
    d->now = 0;
    d->next = 0;
    d->wrap = false;
    d->pnow = QPoint(0, 0);
    d->active = false;

    this->addWidget(d->list);
    this->addWidget(d->desc);

    connect(d->list, SIGNAL(itemClicked(const QString&)), this, SLOT(onItemClicked(const QString&)));
    connect(d->desc, SIGNAL(back()), this, SLOT(slideInPrev()));
}

dtkItemView::~dtkItemView(void)
{
    delete d;

    d = NULL;
}

dtkItemList *dtkItemView::list(void)
{
    return d->list;
}

dtkItemDesc *dtkItemView::desc(void)
{
    return d->desc;
}

void dtkItemView::setBlue(void)
{
    d->list->setBlue();
}

void dtkItemView::setDark(void)
{
    d->list->setDark();
}

void dtkItemView::onItemClicked(const QString& description)
{
    d->desc->setDescription(description);

    this->slideInNext();
}

void dtkItemView::setVerticalMode(bool vertical)
{
    d->vertical = vertical;
}

void dtkItemView::setSpeed(int speed)
{
    d->speed = speed;
}

void dtkItemView::setAnimation(QEasingCurve::Type type)
{
    d->type = type;
}

void dtkItemView::setWrap(bool wrap)
{
    d->wrap = wrap;
}

void dtkItemView::slideInNext(void)
{
    int now = currentIndex();

    if (d->wrap || (now < count() - 1))
        slideInIdx(now + 1);
}

void dtkItemView::slideInPrev(void)
{
    int now = currentIndex();

    if (d->wrap || (now > 0))
        slideInIdx(now - 1);
}

void dtkItemView::slideInIdx(int idx, Direction direction)
{
    if (idx > count() - 1) {
        direction = d->vertical ? Top2Bottom : Right2Left;
        idx = (idx) % count();
    } else if (idx < 0) {
        direction =  d->vertical ? Bottom2Top : Left2Right;
        idx = (idx + count()) % count();
    }

    slideInWgt(widget ( idx ), direction);
}

void dtkItemView::slideInWgt(QWidget *newwidget, Direction direction)
{
    if (d->active)
        return;
    else
        d->active = true;

    Direction directionhint;
    int now = currentIndex();
    int next = indexOf(newwidget);

    if (now == next) {
        d->active = false;
        return;
    } else if (now < next) {
        directionhint = d->vertical ? Top2Bottom : Right2Left;
    } else {
        directionhint = d->vertical ? Bottom2Top : Left2Right;
    }

    if (direction == Automatic) {
        direction = directionhint;
    }

    int offsetx = frameRect().width();
    int offsety = frameRect().height();

    widget(next)->setGeometry ( 0,  0, offsetx, offsety );

    if (direction == Bottom2Top)  {
        offsetx = 0;
        offsety = -offsety;
    } else if (direction == Top2Bottom) {
        offsetx = 0;
    } else if (direction == Right2Left) {
        offsetx = -offsetx;
        offsety = 0;
    } else if (direction == Left2Right) {
        offsety = 0;
    }

    QPoint pnext = widget(next)->pos();
    QPoint pnow = widget(now)->pos();
    d->pnow = pnow;

    widget(next)->move(pnext.x() - offsetx, pnext.y() - offsety);
    widget(next)->show();
    widget(next)->raise();

    QPropertyAnimation *animnow = new QPropertyAnimation(widget(now), "pos");
    animnow->setDuration(d->speed);
    animnow->setEasingCurve(d->type);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx + pnow.x(), offsety + pnow.y()));

    QPropertyAnimation *animnext = new QPropertyAnimation(widget(next), "pos");
    animnext->setDuration(d->speed);
    animnext->setEasingCurve(d->type);
    animnext->setStartValue(QPoint(-offsetx + pnext.x(), offsety + pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

    QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(animnow);
    animgroup->addAnimation(animnext);

    QObject::connect(animgroup, SIGNAL(finished()), this, SLOT(animationDoneSlot()));

    d->next = next;
    d->now = now;
    d->active = true;

    animgroup->start();
}

void dtkItemView::animationDoneSlot(void)
{
    setCurrentIndex(d->next);

    widget(d->now)->hide();
    widget(d->now)->move(d->pnow);

    d->active = false;

    emit animationFinished();
}

// /////////////////////////////////////////////////////////////////
// dtkTagController
// /////////////////////////////////////////////////////////////////

class dtkTagControllerPrivate
{
public:
    dtkItemList *list;
    dtkTagCloud *cloud;
    dtkTagScope *scope;

    QList<dtkTag>  tags;
    QList<dtkItem> items;
    QStringList    filters;

    bool union_mode;
};

dtkTagController::dtkTagController(void) : QObject()
{
    d = new dtkTagControllerPrivate;
    d->list = NULL;
    d->cloud = NULL;
    d->scope = NULL;

    d->union_mode = false;
}

dtkTagController::~dtkTagController(void)
{
    delete d;

    d = NULL;
}

void dtkTagController::attach(dtkItemView *view)
{
    d->list = view->list();
}

void dtkTagController::attach(dtkTagCloud *cloud)
{
    d->cloud = cloud;

    connect(d->cloud, SIGNAL(tagClicked(QString)), this, SLOT(addFilter(QString)));
}

void dtkTagController::attach(dtkTagScope *scope)
{
    d->scope = scope;

    connect(d->scope, SIGNAL(tagSet(QString)),   this, SLOT(setFilter(QString)));
    connect(d->scope, SIGNAL(tagAdded(QString)), this, SLOT(addFilter(QString)));
    connect(d->scope, SIGNAL(tagRemoved(QString)), this, SLOT(remFilter(QString)));
    connect(d->scope, SIGNAL(cleared(void)), this, SLOT(clear()));
    connect(d->scope, SIGNAL(unionMode(bool)), this, SLOT(onUnionMode(bool)));
}

void dtkTagController::addItem(QString name)
{
    d->items << dtkItem(name, "", QStringList());

    this->update();
    this->render();
}

void dtkTagController::addItem(QString name, QString description)
{
    d->items << dtkItem(name, description, QStringList());

    this->update();
    this->render();
}

void dtkTagController::addItem(QString name, QString description, QStringList tags)
{
    d->items << dtkItem(name, description, tags);

    this->update();
    this->render();
}

void dtkTagController::addItem(QString name, QString description, QStringList tags, QString kind, QString type)
{
    d->items << dtkItem(name, description, tags, kind, type);

    this->update();
    this->render();
}

void dtkTagController::update(void)
{
    // d->tags.clear();

    // QHash<QString, QStringList> tags;

    // foreach(dtkItem item, d->items)
    //     foreach(QString tag, item.tags())
    //         tags[tag] << item.name();

    // foreach(QString tag, tags.keys())
    //     d->tags << dtkTag(tag, tags[tag].count(), tags[tag]);
}

static bool intersect(QStringList l1, QStringList l2)
{
    bool pass = true;

    foreach(QString s, l1) if (!l2.contains(s)) pass = false;

    return pass;
}

static bool unite(QStringList l1, QStringList l2)
{
    if (l1.isEmpty()) return true;

    bool pass = false;

    foreach(QString s, l1) if (l2.contains(s)) pass = true;

    return pass;
}

void dtkTagController::clear(void)
{
    d->filters.clear();

    this->update();
    this->render();
}

void dtkTagController::render(void)
{
    d->tags.clear();

    QHash<QString, QStringList> tags;

    foreach (dtkItem item, d->items)
        foreach (QString tag, item.tags())
            tags[tag] << item.name();

    foreach (QString tag, tags.keys())
        d->tags << dtkTag(tag, tags[tag].count(), tags[tag]);

    if (d->list) {
        d->list->clear();

        foreach (dtkItem item, d->items)
            if ((d->union_mode && unite(d->filters, item.tags())) || (!d->union_mode && intersect(d->filters, item.tags())))
                d->list->addItem(item);
    }

    if (d->cloud) {
        d->cloud->clear();

        foreach (dtkTag tag, d->tags)
            d->cloud->addTag(tag);

        d->cloud->render();
    }

    if (d->scope) {
        d->scope->clear();

        foreach (QString filter, d->filters)
            d->scope->addTag(filter, tags[filter].count());

        d->scope->render();
    }

    if (d->scope)
        d->scope->setTags(tags.keys());
}

void dtkTagController::addFilter(QString filter)
{
    d->filters << filter;

    this->render();
}

void dtkTagController::setFilter(QString filter)
{
    d->filters.clear();
    d->filters << filter;

    this->render();
}

void dtkTagController::remFilter(QString filter)
{
    d->filters.removeOne(filter);

    this->render();
}

void dtkTagController::onUnionMode(bool mode)
{
    d->union_mode = !mode;

    this->render();
}
