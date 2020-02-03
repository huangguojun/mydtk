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

#include "dtkThemesWidgetsMainWindow+mac.h"
#include "dtkThemesWidgetsMainWindowButton.h"

#include <QtCore>

#include <Cocoa/Cocoa.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class dtkThemesWidgetsMainWindowPrivate : public QObject
{
    Q_OBJECT

public:
    dtkThemesWidgetsMainWindow *q;

    QFrame                           *titlebar = nullptr;
    QLabel                           *title    = nullptr;
    dtkThemesWidgetsMainWindowButton *minimize = nullptr;
    dtkThemesWidgetsMainWindowButton *maximize = nullptr;
    dtkThemesWidgetsMainWindowButton *close    = nullptr;

public:
    QSizeGrip *g = nullptr;
    bool is_grabbing_title_bar = false;
    QPoint cursor_window_pos_when_grabbing;
    const int title_bar_height = 2*7+18; // height of MainWindowButton + vertical gaps
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

dtkThemesWidgetsMainWindow::dtkThemesWidgetsMainWindow(QWidget *parent) : QMainWindow(parent)
{
    d = new dtkThemesWidgetsMainWindowPrivate;
    d->q = this;

    this->setMinButtonVisible(false);
    this->setZoomButtonVisible(false);
    this->setCloseButtonVisible(false);
    this->setMouseTracking(true);

    d->titlebar = new QFrame(this);
    d->titlebar->setAttribute(Qt::WA_NoSystemBackground);
    d->titlebar->setAttribute(Qt::WA_TranslucentBackground);

    this->initialize();

    d->title    = new QLabel(qApp->applicationName(), this);
    d->minimize = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Minimize, d->titlebar);
    d->maximize = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Maximize, d->titlebar);
    d->close    = new dtkThemesWidgetsMainWindowButton(dtkThemesWidgetsMainWindowButton::Close, d->titlebar);

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

}

void dtkThemesWidgetsMainWindow::setCloseButtonVisible(bool visible)
{
    NSView* view = (NSView *)winId();

    if (!view)
        return;

    NSWindow *window = view.window;

    if (!window)
        return;

    if (visible)
        [[window standardWindowButton:NSWindowCloseButton] setHidden:NO];
    else
        [[window standardWindowButton:NSWindowCloseButton] setHidden:YES];
}

void dtkThemesWidgetsMainWindow::setMinButtonVisible(bool visible)
{
    NSView* view = (NSView *)winId();

    if (!view)
        return;

    NSWindow *window = view.window;

    if (!window)
        return;

    if (visible)
        [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:NO];
    else
        [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
}

void dtkThemesWidgetsMainWindow::setZoomButtonVisible(bool visible)
{
    NSView* view = (NSView *)winId();

    if (!view)
        return;

    NSWindow *window = view.window;

    if (!window)
        return;

    if (visible)
        [[window standardWindowButton:NSWindowZoomButton] setHidden:NO];
    else
        [[window standardWindowButton:NSWindowZoomButton] setHidden:YES];
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

bool dtkThemesWidgetsMainWindow::eventFilter(QObject *source, QEvent *event)
{
    bool status = QWidget::eventFilter(source, event);

    if (event->type() == QEvent::MouseMove) {

        QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);

        event->ignore();
    }

    return status;
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

void dtkThemesWidgetsMainWindow::initialize(void)
{
    if (QSysInfo::macVersion() == QSysInfo::MV_None && QSysInfo::MacintoshVersion == QSysInfo::MV_None) {

        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    if (QSysInfo::MacintoshVersion <= QSysInfo::MV_10_9) {

        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    NSView* view = (NSView*)winId();

    if (!view) {

        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    NSWindow *window = view.window;

    if (!window) {

        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    window.titleVisibility = NSWindowTitleHidden;
    window.titlebarAppearsTransparent = YES;

    [window setMovable:NO];

    window.styleMask |= NSWindowStyleMaskFullSizeContentView;
}

// /////////////////////////////////////////////////////////////////////////////

#include "dtkThemesWidgetsMainWindow+mac.moc"

//
// dtkThemesWidgetsMainWindow.mm ends here
