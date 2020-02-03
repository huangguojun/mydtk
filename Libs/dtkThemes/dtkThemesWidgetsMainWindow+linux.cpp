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

#include "dtkThemesWidgetsMainWindow+linux.h"
#include "dtkThemesWidgetsMainWindow.h"
#include "dtkThemesWidgetsMainWindowButton.h"

#include <QtCore>

// /////////////////////////////////////////////////////////////////////////////
// dtkThemesWidgetsMainWindowPrivate
// /////////////////////////////////////////////////////////////////////////////

class dtkThemesWidgetsMainWindowPrivate : public QObject
{
public:
    QFrame                           *titlebar = nullptr;
    QLabel                           *title    = nullptr;
    dtkThemesWidgetsMainWindowButton *minimize = nullptr;
    dtkThemesWidgetsMainWindowButton *maximize = nullptr;
    dtkThemesWidgetsMainWindowButton *close    = nullptr;

    dtkThemesWidgetsMainWindow *q = nullptr;

public:
    QSizeGrip *g = nullptr;
    bool is_grabbing_title_bar = false;
    QPoint cursor_window_pos_when_grabbing;
    const int title_bar_height = 2*7+18; // height of MainWindowButton + vertical gaps
};

// /////////////////////////////////////////////////////////////////////////////
// dtkThemesWidgetsMainWindow linux implementation
// /////////////////////////////////////////////////////////////////////////////

dtkThemesWidgetsMainWindow::dtkThemesWidgetsMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new dtkThemesWidgetsMainWindowPrivate; d->q = this;

    qApp->installEventFilter(this);

    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

    d->titlebar = new QFrame(this);
    d->titlebar->setAttribute(Qt::WA_NoSystemBackground);
    d->titlebar->setAttribute(Qt::WA_TranslucentBackground);

    d->title    = new QLabel(qApp->applicationName(), this);
    d->title->adjustSize();

    d->minimize = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Minimize, d->titlebar);
    d->maximize = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Maximize, d->titlebar);
    d->close    = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Close,    d->titlebar);

    QHBoxLayout *layout = new QHBoxLayout(d->titlebar);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addSpacing(10);
    layout->addWidget(d->close);
    layout->addWidget(d->minimize);
    layout->addWidget(d->maximize);
    layout->addStretch();
    layout->addWidget(d->title);
    layout->addStretch();

    d->g = new QSizeGrip(this);
    d->g->setFixedWidth(32);
    d->g->setFixedHeight(32);
    d->g->show();
}

dtkThemesWidgetsMainWindow::~dtkThemesWidgetsMainWindow(void)
{
    delete d;
}

void dtkThemesWidgetsMainWindow::touch(QWidget *widget)
{
    if (!widget)
        widget = QMainWindow::centralWidget();

    if (!widget)
        return;

    std::function<void (dtkThemesWidgetsMainWindow *, QWidget *)> fix = [&fix] (dtkThemesWidgetsMainWindow *self, QWidget *source) -> void {

        source->setMouseTracking(true);
        source->installEventFilter(self);

        for (QObject *child : source->children()) {
            if (QWidget *w = dynamic_cast<QWidget *>(child))
                fix(self, w);
        }
    };

    fix(this, widget);
}

QWidget *dtkThemesWidgetsMainWindow::titlebar(void)
{
    return d->titlebar;
}

void dtkThemesWidgetsMainWindow::setCentralWidget(QWidget *widget)
{
    widget->setObjectName("central");

    this->touch(widget);

    widget->stackUnder(d->titlebar);

    QMainWindow::setCentralWidget(widget);
}

void dtkThemesWidgetsMainWindow::mousePressEvent(QMouseEvent *event)
{
    const bool start_grab = !d->is_grabbing_title_bar && event->button() == Qt::LeftButton && event->y() < d->title_bar_height;

    if (start_grab)
    {
        d->is_grabbing_title_bar = true;
        d->cursor_window_pos_when_grabbing = event->windowPos().toPoint();
        event->accept();
    } else {
        QMainWindow::mousePressEvent(event);
    }
}

void dtkThemesWidgetsMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (d->is_grabbing_title_bar && event->button() == Qt::LeftButton)
    {
        d->is_grabbing_title_bar = false;
        event->accept();
    } else {
        QMainWindow::mouseReleaseEvent(event);
    }
}

void dtkThemesWidgetsMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (d->is_grabbing_title_bar)
    {
        this->topLevelWidget()->move(event->globalPos() - d->cursor_window_pos_when_grabbing);
        event->accept();
    } else {
        QMainWindow::mouseMoveEvent(event);
    }
}

void dtkThemesWidgetsMainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    d->titlebar->resize(event->size().width(), d->title_bar_height);
    d->titlebar->move(0, 0);

    d->g->move(event->size().width() - d->g->width(), event->size().height() - d->g->height());
}

bool dtkThemesWidgetsMainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {

        bool status = QWidget::eventFilter(object, event);

        event->ignore();

        return status;
    }

    return QWidget::eventFilter(object, event);
}

// /////////////////////////////////////////////////////////////////////////////

//
// dtkThemesWidgetsMainWindow+linux.cpp ends here
