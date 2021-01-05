/* dtkColorGrid.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Mar 18 15:03:54 2011 (+0100)
 * Version: $Id: 3ba2cce5759fd9437a928ac3114ff4b7da7f9dbd $
 * Last-Updated: Mon Nov 14 12:33:24 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 61
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKCOLORGRID_H
#define DTKCOLORGRID_H

#include <QtWidgets/QWidget>

#include "dtkColorButton.h"

#include <dtkGuiSupportExport.h>

class dtkColorGridPrivate;

class DTKGUISUPPORT_EXPORT dtkColorGrid : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int cellSize READ cellSize WRITE setCellSize)
    Q_PROPERTY(int widthInCells READ widthInCells WRITE setWidthInCells)
    Q_PROPERTY(bool autoSize READ autoSize WRITE setAutoSize)
    Q_PROPERTY(bool pickByDrag READ pickByDrag WRITE setPickByDrag)
    Q_PROPERTY(ClickMode clickMode READ clickMode WRITE setClickMode)

public:
    enum ClickMode { CM_PRESS, CM_RELEASE };

    dtkColorGrid(QWidget *parent = 0);
    virtual ~dtkColorGrid(void);

    virtual QSize minimumSizeHint(void) const;
    virtual QSize sizeHint(void) const;

    ClickMode clickMode(void) const;

    bool autoSize(void) const;
    int cellSize(void) const;
    int widthInCells(void) const;
    int heightInCells(void) const;

    const QColor &lastHighlighted(void) const;
    const QColor &lastPicked(void) const;

    bool pickByDrag(void) const;

    dtkColorList *scheme(void) const;

    void setAutoSize(bool autosize);
    void setCellSize(int size);
    void setWidthInCells(int width);
    void setPickByDrag(bool set);
    void setClickMode(ClickMode mode);
    void setScheme(dtkColorList *scheme);

public:
    static dtkColorList *defaultColors(void);
    static dtkColorList *defaultColors2(void);
    static dtkColorList *baseColors(void);
    static dtkColorList *namedColors(void);

signals:
    void highlighted(const QColor &color);
    void picked(const QColor &color);
    void accepted(void);
    void rejected(void);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual bool event(QEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

protected:
    void redraw(void);

protected:
    int index(void) const;

private:
    dtkColorGridPrivate *d;
};

#endif
