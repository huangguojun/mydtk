// Version: $Id: dbd2fb324534a6a5ac3ae040f00d32fa1757cdcc $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <QtWidgets>

#include "dtkSplitter.h"

// /////////////////////////////////////////////////////////////////
// dtkSplitterHandle
// /////////////////////////////////////////////////////////////////

class dtkSplitterHandle : public QSplitterHandle
{
public:
    dtkSplitterHandle(Qt::Orientation orientation, bool slim, QSplitter *parent);

public:
    QSize sizeHint(void) const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool m_slim;
};

dtkSplitterHandle::dtkSplitterHandle(Qt::Orientation orientation, bool slim, QSplitter *parent) : QSplitterHandle(orientation, parent)
{
    this->m_slim = slim;
}

void dtkSplitterHandle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QColor topColor(145, 145, 145);
    QColor bottomColor(142, 142, 142);
    QColor gradientStart(252, 252, 252);
    QColor gradientStop(223, 223, 223);

    if (orientation() == Qt::Vertical) {
        painter.setPen(topColor);
        painter.drawLine(0, 0, width(), 0);

        if (m_slim)
            return;

        painter.setPen(bottomColor);
        painter.drawLine(0, height() - 1, width(), height() - 1);

        QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height() - 3));
        linearGrad.setColorAt(0, gradientStart);
        linearGrad.setColorAt(1, gradientStop);
        painter.fillRect(QRect(QPoint(0, 1), size() - QSize(0, 2)), QBrush(linearGrad));

    } else {
        painter.setPen(topColor);
        painter.drawLine(0, 0, 0, height());
    }
}

QSize dtkSplitterHandle::sizeHint(void) const
{
    QSize parent = QSplitterHandle::sizeHint();

    if (orientation() == Qt::Vertical) {
        return m_slim ? QSize(parent.width(), 1) : parent + QSize(0, 3);
    } else {
        return QSize(1, parent.height());
    }
}

// /////////////////////////////////////////////////////////////////
// dtkSplitterPrivate
// /////////////////////////////////////////////////////////////////

class dtkSplitterPrivate
{
public:
    bool slim;
};

// /////////////////////////////////////////////////////////////////
// dtkSplitter
// /////////////////////////////////////////////////////////////////

dtkSplitter::dtkSplitter(QWidget *parent, bool slim) : QSplitter(parent), d(new dtkSplitterPrivate)
{
    d->slim = slim;
}

dtkSplitter::~dtkSplitter(void)
{
    delete d;

    d = NULL;
}

QSplitterHandle *dtkSplitter::createHandle(void)
{
    return new dtkSplitterHandle(orientation(), d->slim, this);
}

//
// dtkSplitter.cpp ends here
