// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkThemesWidgetsMainWindowButton.h"

#include "dtkThemesWidgetsMainWindow.h"

#include <dtkThemes>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkThemesWidgetsMainWindowButtonPrivate
{
public:
    dtkThemesWidgetsMainWindowButton::Kind kind;
    QVector<QLine> symbol_lines;
    QPainterPath symbol_path;
    static bool hovered;
    static QVector<dtkThemesWidgetsMainWindowButton *> buttons;
};

bool dtkThemesWidgetsMainWindowButtonPrivate::hovered = false;
QVector<dtkThemesWidgetsMainWindowButton *> dtkThemesWidgetsMainWindowButtonPrivate::buttons;

dtkThemesWidgetsMainWindowButton::dtkThemesWidgetsMainWindowButton(Kind kind, QWidget *parent)
    : QFrame(parent), d(new dtkThemesWidgetsMainWindowButtonPrivate)
{
    d->kind = kind;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setAttribute(Qt::WA_Hover, true);

    static const int length{ 18 };

    switch (d->kind) {
    case Minimize: {
        QPoint x1{ length * 27 / 100, length / 2 };
        QPoint x2{ length * 73 / 100, length / 2 };
        d->symbol_lines = { { x1, x2 } };
        break;
    }
    case Close: {
        QPoint x1{ length * 30 / 100, length * 30 / 100 };
        QPoint x2{ length * 70 / 100, length * 70 / 100 };
        QPoint x3{ length * 70 / 100, length * 30 / 100 };
        QPoint x4{ length * 30 / 100, length * 70 / 100 };
        d->symbol_lines = { { x1, x2 }, { x3, x4 } };
        break;
    }
    case Maximize: {
        QPoint x1{ length * 25 / 100, length * 75 / 100 };
        QPoint x2{ length * 25 / 100, length * 34 / 100 };
        QPoint x3{ length * 66 / 100, length * 75 / 100 };
        QPoint x4{ length * 34 / 100, length * 25 / 100 };
        QPoint x5{ length * 75 / 100, length * 25 / 100 };
        QPoint x6{ length * 75 / 100, length * 66 / 100 };
        d->symbol_path.addPolygon(QPolygon{ { x1, x2, x3 } });
        d->symbol_path.addPolygon(QPolygon{ { x4, x5, x6 } });
        break;
    }
    default:
        break;
    };

    dtkThemesWidgetsMainWindowButtonPrivate::buttons.append(this);
}

dtkThemesWidgetsMainWindowButton::~dtkThemesWidgetsMainWindowButton(void)
{
    delete d;
}

QSize dtkThemesWidgetsMainWindowButton::sizeHint(void) const
{
    return QSize(25, 18);
}

void dtkThemesWidgetsMainWindowButton::mousePressEvent(QMouseEvent *)
{
    dtkThemesWidgetsMainWindow *window =
            static_cast<dtkThemesWidgetsMainWindow *>(parentWidget()->parentWidget());

    switch (d->kind) {
    case Maximize:
        window->isMaximized() ? window->showNormal() : window->showMaximized();
        break;
    case Minimize:
        window->showMinimized();
        break;
    case Close:
        window->close();
        break;
    }
}

void dtkThemesWidgetsMainWindowButton::enterEvent(QEvent *event)
{
    dtkThemesWidgetsMainWindowButtonPrivate::hovered = true;

    for (QWidget *b : dtkThemesWidgetsMainWindowButtonPrivate::buttons)
        b->update();

    QFrame::enterEvent(event);
}

void dtkThemesWidgetsMainWindowButton::leaveEvent(QEvent *event)
{
    dtkThemesWidgetsMainWindowButtonPrivate::hovered = false;

    for (QWidget *b : dtkThemesWidgetsMainWindowButtonPrivate::buttons)
        b->update();

    QFrame::leaveEvent(event);
}

void dtkThemesWidgetsMainWindowButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    switch (d->kind) {
    case Maximize:
        painter.setBrush(dtkThemesEngine::instance()->color("@green"));
        break;
    case Minimize:
        painter.setBrush(dtkThemesEngine::instance()->color("@orange"));
        break;
    default:
        painter.setBrush(dtkThemesEngine::instance()->color("@red"));
        break;
    };

    static const int length{ size().height() * 5 / 6 };
    static const QRect rect((size().height() - length) / 2, (size().height() - length) / 2, length,
                            length);

    painter.drawEllipse(rect);

    if (dtkThemesWidgetsMainWindowButtonPrivate::hovered) {
        static const QPen pen{ QBrush{ QColor::fromRgb(0, 0, 0, 160) }, 2 };
        painter.setPen(pen);
        if (d->symbol_lines.count())
            painter.drawLines(d->symbol_lines);
        else if (d->symbol_path.elementCount())
            painter.fillPath(d->symbol_path, pen.brush());
    }
}

//
// dtkThemesWidgetsMainWindowButton.cpp ends here
