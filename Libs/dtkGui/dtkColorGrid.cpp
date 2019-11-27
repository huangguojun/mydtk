/* dtkColorGrid.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Mar 18 15:07:40 2011 (+0100)
 * Version: $Id: 213200b97673bb3f54d2feaf34fb3ea56793e989 $
 * Last-Updated: Thu Nov 29 14:12:18 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 46
 */

/* Commentary:
 * See credits at EOF.
 */

/* Change log:
 *
 */

#include "dtkColorGrid.h"

#include <dtkCoreSupport/dtkGlobal.h>

#include <QtWidgets>

#include <math.h>

class dtkColorGridPrivate
{
public:
    int cellSize;
    int widthInCells;
    bool autoSize;
    int row, col, idx;
    QPixmap pix;
    bool pickDrag;
    dtkColorGrid::ClickMode clickMode;
    QPoint pos;

    QColor hlColor, selColor;

    dtkColorList *colors;
};

dtkColorGrid::dtkColorGrid(QWidget *parent) : QWidget(parent), d(new dtkColorGridPrivate)
{
    d->cellSize = 12;
    d->widthInCells = 32;
    d->autoSize = false;
    d->row = -1;
    d->col = -1;
    d->idx = -1;
    d->pickDrag = true;
    d->clickMode = CM_PRESS;
    d->colors = 0;

    setScheme(defaultColors());
    setFixedSize(minimumSizeHint());
    setMouseTracking(true);
}

dtkColorGrid::~dtkColorGrid(void)
{
    delete d;

    d = NULL;
}

QSize dtkColorGrid::minimumSizeHint() const
{
    return QSize((d->cellSize + 1) * d->widthInCells + 3, (d->cellSize + 1) * heightInCells() + 3);
}

QSize dtkColorGrid::sizeHint() const
{
    return minimumSizeHint();
}

dtkColorGrid::ClickMode dtkColorGrid::clickMode(void) const
{
    return d->clickMode;
}

bool dtkColorGrid::autoSize(void) const
{
    return d->autoSize;
}

int dtkColorGrid::cellSize(void) const
{
    return d->cellSize;
}

int dtkColorGrid::widthInCells(void) const
{
    return d->widthInCells;
}

const QColor& dtkColorGrid::lastHighlighted(void) const
{
    return d->hlColor;
}

const QColor& dtkColorGrid::lastPicked(void) const
{
    return d->selColor;
}

bool dtkColorGrid::pickByDrag(void) const
{
    return d->pickDrag;
}

dtkColorList *dtkColorGrid::scheme(void) const
{
    return d->colors;
}

int dtkColorGrid::heightInCells() const
{
    if (!d->colors) return 0;

    int h = d->colors->size() / d->widthInCells;

    if (d->colors->size() % d->widthInCells) h++;

    return h;
}

void dtkColorGrid::setAutoSize(bool autosize)
{
    d->autoSize = autosize;

    if (d->autoSize && d->colors) {
        setWidthInCells(sqrt((float)(d->colors->count())));
    }
}

void dtkColorGrid::setCellSize(int size)
{
    if (size < 8) size = 8;

    d->cellSize = size;
    setFixedSize(minimumSizeHint());
    redraw();
}

void dtkColorGrid::setWidthInCells(int width)
{
    if (width < 1) width = 1;

    d->widthInCells = width;
    setFixedSize(minimumSizeHint());
    redraw();
}

void dtkColorGrid::setPickByDrag(bool set)
{
    d->pickDrag = set;
}

void dtkColorGrid::setClickMode(ClickMode mode)
{
    d->clickMode = mode;
}

void dtkColorGrid::setScheme(dtkColorList *list)
{
    if (!list) return;

    d->colors = list;
    d->idx = -1;

    if (d->autoSize && d->colors) {
        setWidthInCells(sqrt((float)(d->colors->count())));
    } else
        redraw();

    update();
}

/////////////////////////////////////////////////////////////////////

void dtkColorGrid::redraw(void)
{
    int rows = heightInCells();

    int c = d->cellSize + 1; // frame

    d->pix = QPixmap(minimumSizeHint());
    QPainter p(&d->pix);

    p.setBrush(palette().base());
    p.setPen(palette().shadow().color());
    p.drawRect(d->pix.rect().adjusted(0, 0, -1, -1));

    int x = 2, y = 2;
    int idx = 0;

    for (int j = 0; j < rows; j++, y += c) {
        x = 2;

        for (int i = 0; i < d->widthInCells; i++, x += c) {
            if (idx == d->colors->size())
                return;

            p.fillRect(QRect(x, y, d->cellSize, d->cellSize), d->colors->at(idx++));
        }
    }

    repaint();
}

int dtkColorGrid::index() const
{
    int i = d->widthInCells * d->row + d->col;

    if (d->col >= d->widthInCells || i < 0 || i >= d->colors->size())
        i = -1;

    return i;
}

void dtkColorGrid::paintEvent ( QPaintEvent *event )
{
    DTK_UNUSED(event);

    QPainter p(this);
    p.fillRect(rect(), palette().button());
    p.drawPixmap(0, 0, d->pix);

    d->hlColor = QColor();

    if (d->idx >= 0) {
        d->hlColor = d->colors->at(d->idx);

        int c = d->cellSize + 1;
        int x = d->col * c;
        int y = d->row * c;;

        p.setPen(QPen(palette().highlight(), 2));
        p.drawRect(QRect(x + 1, y + 1, c, c));
    }
}

void dtkColorGrid::mouseMoveEvent ( QMouseEvent *event )
{
    //if (!hasFocus()) setFocus();

    QWidget::mouseMoveEvent(event);

    d->pos = event->pos();

    int c = d->cellSize + 1;
    d->row = d->pos.y() / c;
    d->col = d->pos.x() / c;

    int i = index();

    if (i != d->idx) {
        d->idx = i;
        repaint();

        QToolTip::hideText();

        if (d->idx != -1) {
            emit highlighted(d->hlColor);

            QToolTip::showText(event->globalPos(), d->hlColor.name(), this);

            if (d->pickDrag && event->buttons() & Qt::LeftButton) {
                d->selColor = d->hlColor;
                emit picked(d->selColor);
                //emit accepted();
            }
        }
    }

}

void dtkColorGrid::mousePressEvent ( QMouseEvent *event )
{
    if (!hasFocus()) setFocus();

    if (d->clickMode == CM_PRESS && event->button() == Qt::LeftButton && d->hlColor.isValid()) {
        d->selColor = d->hlColor;
        emit picked(d->selColor);
        emit accepted();
    }
}

void dtkColorGrid::mouseReleaseEvent ( QMouseEvent *event )
{
    if (!hasFocus()) setFocus();

    if (d->clickMode == CM_RELEASE && event->button() == Qt::LeftButton && d->hlColor.isValid()) {
        d->selColor = d->hlColor;
        emit picked(d->selColor);
        emit accepted();
    }
}

void dtkColorGrid::leaveEvent ( QEvent *event )
{
    DTK_UNUSED(event);

    QToolTip::hideText();

    if (d->idx != -1) {
        d->idx = -1;
        repaint();
    }
}

bool dtkColorGrid::event ( QEvent *event )
{
    //if (event->type() == QEvent::ToolTip) {
    //    QToolTip::showText(d->pos, "zzz", this);

    //    event->accept();
    //    return true;
    //}

    return QWidget::event(event);
}

void dtkColorGrid::keyPressEvent ( QKeyEvent *event )
{
    QToolTip::hideText();

    switch (event->key()) {

    case Qt::Key_Right:
        if (d->idx == -1) {
            d->row = d->col = 0;
            d->idx = 0;
        } else {
            if (++d->col == d->widthInCells) {
                d->col = 0;
                d->row++;
            }

            d->idx = index();

            if (d->idx == -1) {
                d->row = d->col = 0;
                d->idx = 0;
            }
        }

        repaint();
        d->hlColor = d->colors->at(d->idx);
        emit highlighted(d->hlColor);
        event->accept();
        return;

    case Qt::Key_Left:
        if (d->idx == -1) {
            d->row = d->col = 0;
            d->idx = 0;
        } else {
            if (--d->col < 0) {
                d->col = d->widthInCells - 1;
                d->row--;
            }

            d->idx = index();

            if (d->idx == -1) {
                d->row = heightInCells() - 1;
                d->idx = d->colors->size() - 1;
                d->col = d->idx % d->widthInCells;
            }
        }

        repaint();
        d->hlColor = d->colors->at(d->idx);
        emit highlighted(d->hlColor);
        event->accept();
        return;

    case Qt::Key_Up:
        if (d->idx == -1) {
            d->row = d->col = 0;
            d->idx = 0;
        } else {
            int h = heightInCells() - 1;

            if (--d->row < 0) {
                d->row = h;

                if (--d->col < 0) {
                    d->row = h;
                    d->idx = d->colors->size() - 1;
                    d->col = d->idx % d->widthInCells;
                }
            }

            d->idx = index();

            if (d->idx == -1 && d->row == h) {
                d->row--;
                d->idx = index();
            }

            if (d->idx == -1) {
                d->row = h;
                d->idx = d->colors->size() - 1;
                d->col = d->idx % d->widthInCells;
            }
        }

        repaint();
        d->hlColor = d->colors->at(d->idx);
        emit highlighted(d->hlColor);
        event->accept();
        return;

    case Qt::Key_Down:
        if (d->idx == -1) {
            d->row = d->col = 0;
            d->idx = 0;
        } else {
            int h = heightInCells() - 1;

            if (++d->row > h) {
                d->row = 0;
                d->col++;
            }

            d->idx = index();

            if (d->idx == -1 && d->row == h) {
                d->row = 0; d->col++;
                d->idx = index();
            }

            if (d->idx == -1) {
                d->row = d->col = 0;
                d->idx = 0;
            }
        }

        repaint();
        d->hlColor = d->colors->at(d->idx);
        emit highlighted(d->hlColor);
        event->accept();
        return;

    case Qt::Key_Return:
        if (d->idx != -1) {
            emit picked(d->selColor = d->hlColor);
            emit accepted();
        }

        event->accept();
        return;

    case Qt::Key_Escape:
        emit rejected();
        event->accept();
        return;

    default:
        event->ignore();
    }

    return QWidget::keyPressEvent(event);
}

dtkColorList *dtkColorGrid::defaultColors()
{
    static dtkColorList s_defaultColors;

    if (!s_defaultColors.isEmpty())
        return &s_defaultColors;

    const int step = 32;

    for (int b = 0; b <= 256; b += 16) {
        if (b == 256) b = 255;

        s_defaultColors.append(QColor(b, b, b));
    }

    for (int r = 0; r <= 256; r += step) {
        if (r == 256) r = 255;

        for (int g = 0; g <= 256; g += step) {
            if (g == 256) g = 255;

            for (int b = 0; b <= 256; b += step) {
                if (b == 256) b = 255;

                s_defaultColors.append(QColor(r, g, b));
            }
        }
    }

    return &s_defaultColors;
}

dtkColorList *dtkColorGrid::defaultColors2()
{
    static dtkColorList s_defaultColors2;

    if (!s_defaultColors2.isEmpty())
        return &s_defaultColors2;

    const int step = 64;

    for (int b = 0; b <= 256; b += 16) {
        if (b == 256) b = 255;

        s_defaultColors2.append(QColor(b, b, b));
    }

    for (int r = 0; r <= 256; r += step) {
        if (r == 256) r = 255;

        for (int g = 0; g <= 256; g += step) {
            if (g == 256) g = 255;

            for (int b = 0; b <= 256; b += step) {
                if (b == 256) b = 255;

                s_defaultColors2.append(QColor(r, g, b));
            }
        }
    }

    return &s_defaultColors2;
}

dtkColorList *dtkColorGrid::baseColors()
{
    static dtkColorList s_baseColors;

    if (!s_baseColors.isEmpty())
        return &s_baseColors;

    for (int i = Qt::black; i <= Qt::darkYellow; i++)
        s_baseColors.append((Qt::GlobalColor)i);

    return &s_baseColors;
}

dtkColorList *dtkColorGrid::namedColors()
{
    static dtkColorList s_namedColors;

    if (!s_namedColors.isEmpty())
        return &s_namedColors;

    QStringList names = QColor::colorNames();

    for (int i = 0; i < names.size(); i++)
        s_namedColors.append(QColor(names.at(i)));

    return &s_namedColors;
}

// /////////////////////////////////////////////////////////////////
// Credits
// /////////////////////////////////////////////////////////////////

/* QSint library. */
