/* dtkColorButton.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Mar 18 14:42:33 2011 (+0100)
 * Version: $Id: aa2a94d15573033650c180acd95d83afafc5f98b $
 * Last-Updated: Mon Nov 14 12:32:29 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 54
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKCOLORBUTTON_H
#define DTKCOLORBUTTON_H

#include <QtWidgets/QPushButton>

#include <dtkGuiSupportExport.h>

class dtkColorButtonPrivate;
class dtkColorGrid;
class dtkColorPopup;

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

typedef QList<QColor> dtkColorList;

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class DTKGUISUPPORT_EXPORT dtkColorButton : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(int cellSize READ cellSize WRITE setCellSize)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(PickMode pickModeLeft READ pickModeLeft WRITE setPickModeLeft)
    Q_PROPERTY(PickMode pickModeRight READ pickModeRight WRITE setPickModeRight)

public:
    enum PickMode { PM_NONE, PM_COLORDIALOG, PM_COLORGRID };

    dtkColorButton(QWidget *parent = 0);
    virtual ~dtkColorButton(void);

    int cellSize(void) const;

    const QColor &color(void) const;

    const PickMode &pickModeLeft() const;
    const PickMode &pickModeRight() const;

    dtkColorList *scheme(void) const;

    void setCellSize(int size);
    void setPickModeLeft(const PickMode &mode);
    void setPickModeRight(const PickMode &mode);
    void setScheme(dtkColorList *scheme);

signals:
    void colorChanged(const QColor &color);

public slots:
    void setColor(const QColor &color);

protected:
    virtual void drawColorItem(QPixmap &pm, const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    dtkColorButtonPrivate *d;
};

#endif
