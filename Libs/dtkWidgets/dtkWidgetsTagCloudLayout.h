/* dtkWidgetsTagCloudLayout.h ---
 *
 * Author: Julien Wintz
 * Created: Mon Apr 15 13:48:50 2013 (+0200)
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class DTKWIDGETS_EXPORT dtkWidgetsTagCloudLayout : public QLayout
{
public:
    dtkWidgetsTagCloudLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    dtkWidgetsTagCloudLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~dtkWidgetsTagCloudLayout(void);

#pragma mark -
#pragma mark Spacing

    int horizontalSpacing(void) const;
    int verticalSpacing(void) const;

#pragma mark -
#pragma mark Directions

    Qt::Orientations expandingDirections() const;

#pragma mark -
#pragma mark Items

    void addItem(QLayoutItem *item);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);

#pragma mark -
#pragma mark Sizes

    bool hasHeightForWidth(void) const;
    int heightForWidth(int) const;
    int count(void) const;
    QSize minimumSize(void) const;
    void setGeometry(const QRect& rect);
    QSize sizeHint(void) const;

private:
    int doLayout(const QRect& rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};
