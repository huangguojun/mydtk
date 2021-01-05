/* dtkWidgetsTagCloud.cpp ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 12:00:34 2013 (+0200)
 * Version:
 * Last-Updated: mar. avril  8 14:54:47 2014 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 33
 */

/* Change Log:
 *
 */

#include "dtkWidgetsTagCloud.h"
#include "dtkWidgetsTag.h"

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

bool dtkWidgetsTagAlphaLessThan(const dtkWidgetsTag &t1, const dtkWidgetsTag &t2)
{
    return t1.text() < t2.text();
}

bool dtkWidgetsTagNumLessThan(const dtkWidgetsTag &t1, const dtkWidgetsTag &t2)
{
    return t1.count() < t2.count();
}

bool dtkWidgetsTagAlphaMoreThan(const dtkWidgetsTag &t1, const dtkWidgetsTag &t2)
{
    return t1.text() >= t2.text();
}

bool dtkWidgetsTagNumMoreThan(const dtkWidgetsTag &t1, const dtkWidgetsTag &t2)
{
    return t1.count() >= t2.count();
}

// /////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudHasher
// /////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudHasher
{
public:
    dtkWidgetsTagCloudHasher(int buckets, int min, int max)
    {
        if (buckets < 1)
            qDebug() << "dtkWidgetsTagCloudHasher: Must have at least one bucket.";

        this->buckets = buckets;
        this->min = min;
        this->max = max + 1;

        this->width = ((double)(this->max - this->min)) / ((double)(this->buckets));
    }

    int bucket(dtkWidgetsTag tag)
    {
        return ((float)(tag.count() - this->min)) / ((float)(this->width));
    }

private:
    int min, max, buckets;

private:
    double width;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloudPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsTagCloudPrivate
{
public:
    QList<dtkWidgetsTag> tags;

    int averageFontSize;
    int fontSizeRange;
    int mincount;
    int maxcount;
    int tagCount;

    dtkWidgetsTagCloud::SortingType sortingType;
    dtkWidgetsTagCloud::SortingOrder sortingOrder;
};

// /////////////////////////////////////////////////////////////////
// dtkWidgetsTagCloud
// /////////////////////////////////////////////////////////////////

/*!
  \class dtkWidgetsTagCloud

  \inmodule dtkWidgets

  \brief The dtkWidgetsTagCloud class provides a configurable tag
  cloud.

  \l {Core Module} {Core Module} ...
*/

dtkWidgetsTagCloud::dtkWidgetsTagCloud(QWidget *parent) : QTextBrowser(parent)
{
    d = new dtkWidgetsTagCloudPrivate;

    d->averageFontSize = 0;
    d->fontSizeRange = 0;

    d->sortingType = Alpha;
    d->sortingOrder = Asc;

    this->setFrameShape(QFrame::NoFrame);

    connect(this, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(onLinkClicked(const QUrl &)));
}

dtkWidgetsTagCloud::~dtkWidgetsTagCloud(void)
{
    delete d;

    d = NULL;
}

void dtkWidgetsTagCloud::addTag(QString text, int count)
{
    d->tags << dtkWidgetsTag(text, count);
}

void dtkWidgetsTagCloud::addTag(QString text, int count, QStringList items)
{
    d->tags << dtkWidgetsTag(text, count, items);
}

void dtkWidgetsTagCloud::addTag(QString text, int count, QStringList items, QString color)
{
    d->tags << dtkWidgetsTag(text, count, items, color);
}

void dtkWidgetsTagCloud::addTag(dtkWidgetsTag tag)
{
    d->tags << tag;
}

void dtkWidgetsTagCloud::setFontSize(int size)
{
    d->averageFontSize = (size > 0) ? size : 0;
}

void dtkWidgetsTagCloud::setFontRange(int range)
{
    d->fontSizeRange = (range > 0) ? range : 0;
}

void dtkWidgetsTagCloud::setSortingType(SortingType type)
{
    d->sortingType = type;
}

void dtkWidgetsTagCloud::setSortingOrder(SortingOrder order)
{
    d->sortingOrder = order;
}

void dtkWidgetsTagCloud::sort(void)
{
    if (d->sortingType == Alpha && d->sortingOrder == Asc)
        qSort(d->tags.begin(), d->tags.end(), dtkWidgetsTagAlphaLessThan);

    if (d->sortingType == Alpha && d->sortingOrder == Desc)
        qSort(d->tags.begin(), d->tags.end(), dtkWidgetsTagAlphaMoreThan);

    if (d->sortingType == Num && d->sortingOrder == Asc)
        qSort(d->tags.begin(), d->tags.end(), dtkWidgetsTagNumLessThan);

    if (d->sortingType == Num && d->sortingOrder == Desc)
        qSort(d->tags.begin(), d->tags.end(), dtkWidgetsTagNumMoreThan);
}

void dtkWidgetsTagCloud::clear(void)
{
    d->tags.clear();

    this->render();
}

void dtkWidgetsTagCloud::render(void)
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

    foreach (dtkWidgetsTag tag, d->tags) {

        if (tag.count() < d->mincount)
            d->mincount = tag.count();

        if (tag.count() > d->maxcount)
            d->maxcount = tag.count();

        d->tagCount++;
    }

    dtkWidgetsTagCloudHasher hasher(d->fontSizeRange, d->mincount, d->maxcount);

    int baseFontSize = d->averageFontSize - ((double)(d->fontSizeRange - 1) / 2);

    QString cloud;
    cloud.append(QString("<div align=\"justify\">\n"));

    foreach (dtkWidgetsTag tag, d->tags) {

        int fontSize = baseFontSize + hasher.bucket(tag);

        QString color = "";

        if (!tag.color().isEmpty()) {
            color = "color: ";
            color += (!tag.color().startsWith("#")) ? "#" : "";
            color += tag.color();
            color += ";";
        } else {
            color = "color: #6a769d;";
        }

        QString count = QString::number(tag.count()) + " item" + ((tag.count() != 1) ? "s" : "");

        cloud.append(QString("<a href=\"tag://%1\" title=\"%2\" style=\"font-size: "
                             "%4px; text-decoration: none; %5\" item=\"%3\">%1</a> ")
                             .arg(tag.text())
                             .arg(count)
                             .arg(tag.items().first())
                             .arg(fontSize)
                             .arg(color));
    }

    cloud.append("</div>\n");

    this->setHtml(cloud);
}

void dtkWidgetsTagCloud::onLinkClicked(const QUrl &url)
{
    emit tagClicked(url.host());
}
