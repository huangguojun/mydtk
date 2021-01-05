#include "dtkVisualizationWidgetsVideoControls.h"

#include <dtkFonts/dtkFontAwesome>

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsVideoControlsItem
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsVideoControlsItem : public QFrame
{
    Q_OBJECT

public:
    dtkVisualizationWidgetsVideoControlsItem(fa::icon icon, int icon_size,
                                             QWidget *parent = nullptr);

    void setIcon(fa::icon icon);

signals:
    void clicked(void);

protected:
    QLabel *label = nullptr;
    int m_icon_size = 0;

protected:
    void mousePressEvent(QMouseEvent *);
};

// ///////////////////////////////////////////////////////////////////

dtkVisualizationWidgetsVideoControlsItem::dtkVisualizationWidgetsVideoControlsItem(fa::icon icon,
                                                                                   int icon_size,
                                                                                   QWidget *parent)
    : QFrame(parent)
{
    m_icon_size = icon_size;

    label = new QLabel(this);
    label->setFixedSize(QSize(icon_size, icon_size));
    label->setPixmap(dtkFontAwesome::instance()->icon(icon).pixmap(icon_size, icon_size));
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setAttribute(Qt::WA_NoSystemBackground);
    label->setVisible(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(label);
    this->setLayout(layout);
    this->setMouseTracking(true);
};

void dtkVisualizationWidgetsVideoControlsItem::setIcon(fa::icon icon)
{
    label->setPixmap(dtkFontAwesome::instance()->icon(icon).pixmap(m_icon_size, m_icon_size));
};

void dtkVisualizationWidgetsVideoControlsItem::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsVideoControlsPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsVideoControlsPrivate : public QObject
{
    Q_OBJECT

public:
    dtkVisualizationWidgetsVideoControlsItem *status_item = nullptr;
    dtkVisualizationWidgetsVideoControlsItem *rewind = nullptr;
    dtkVisualizationWidgetsVideoControlsItem *fullscreen = nullptr;

    dtkVisualizationWidgetsVideoControls::State state =
            dtkVisualizationWidgetsVideoControls::State::StoppedState;

public slots:
    void updateDurationInfo(qint64 currentInfo);

public:
    QProgressBar *progress_bar = nullptr;
    QProgressBar *progress_bar_buffered = nullptr;
    QLabel *label_duration = nullptr;
    bool is_fullscreen = false;
    bool display_frame_info = false;
};

// ///////////////////////////////////////////////////////////////////

void dtkVisualizationWidgetsVideoControlsPrivate::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    int duration = this->progress_bar->maximum() / 1000;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, currentInfo % 60,
                          (currentInfo * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60,
                        (duration * 1000) % 1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    this->label_duration->setText(tStr);
    // TODO: do not hard code color, but this requiress VTK background to use
    // dtkThemes
    this->label_duration->setStyleSheet("font: 14pt; color: #bbbbbb;");
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsVideoControls
// ///////////////////////////////////////////////////////////////////

dtkVisualizationWidgetsVideoControls::dtkVisualizationWidgetsVideoControls(QWidget *parent)
    : QFrame(parent), d(new dtkVisualizationWidgetsVideoControlsPrivate)
{
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);

    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", QColor(Qt::white));

    d->status_item = new dtkVisualizationWidgetsVideoControlsItem(fa::play, 24, parent);
    d->rewind = new dtkVisualizationWidgetsVideoControlsItem(fa::stepbackward, 24, parent);
    d->fullscreen = new dtkVisualizationWidgetsVideoControlsItem(fa::arrowsalt, 24, parent);
    connect(d->status_item, &dtkVisualizationWidgetsVideoControlsItem::clicked, this,
            &dtkVisualizationWidgetsVideoControls::toggle);
    connect(d->fullscreen, &dtkVisualizationWidgetsVideoControlsItem::clicked, [=]() {
        if (d->is_fullscreen) // was fullscreen
            d->fullscreen->setIcon(fa::arrowsalt);
        else
            d->fullscreen->setIcon(fa::compress);

        d->is_fullscreen = !d->is_fullscreen;
        emit fullscreen(d->is_fullscreen);
    });
    connect(d->rewind, &dtkVisualizationWidgetsVideoControlsItem::clicked, [=]() {
        d->state = State::StoppedState;
        d->status_item->setIcon(fa::play);
        d->progress_bar->reset();
        emit restart();
    });

    d->progress_bar = new QProgressBar(parent);
    d->progress_bar->setOrientation(Qt::Horizontal);
    d->progress_bar->setTextVisible(true);
    d->progress_bar->setFixedHeight(10);

    d->progress_bar_buffered = new QProgressBar(parent);
    d->progress_bar_buffered->setOrientation(Qt::Horizontal);
    d->progress_bar_buffered->setTextVisible(true);
    d->progress_bar_buffered->setFixedHeight(10);
    d->progress_bar_buffered->setVisible(false);

    QString style_bar = "QProgressBar{ border: 1px solid transparent; text-align: center; "
                        "color:rgba(0,0,0,100); border-radius: 5px; background-color: "
                        "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, "
                        "182, 182, 100), stop:1 rgba(209, 209, 209, 100)); } "
                        "QProgressBar::chunk{ background-color: rgba(253,26,30,100); }";
    QString style_bar_buffered =
            "QProgressBar{ border: 1px solid transparent; text-align: center; "
            "color:rgba(0,0,0,100); border-radius: 5px; background-color: "
            "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(222, "
            "222, 222, 100), stop:1 rgba(249, 249, 249, 100)); } "
            "QProgressBar::chunk{ background-color: rgba(253,156,170,100); }";

    d->progress_bar->setStyleSheet(style_bar);
    d->progress_bar_buffered->setStyleSheet(style_bar_buffered);

    d->label_duration = new QLabel(this);
    d->label_duration->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(32, 0, 32, 0);
    vlayout->setAlignment(Qt::AlignLeft);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(d->status_item);
    layout->addWidget(d->rewind);
    layout->addWidget(d->label_duration);
    layout->addWidget(d->fullscreen);

    this->setMouseTracking(true);

    vlayout->addWidget(d->progress_bar);
    vlayout->addWidget(d->progress_bar_buffered);
    vlayout->addLayout(layout);

    this->setLayout(vlayout);
}

dtkVisualizationWidgetsVideoControls::~dtkVisualizationWidgetsVideoControls(void)
{
    delete d;
}

void dtkVisualizationWidgetsVideoControls::reset(void)
{
    d->state = State::StoppedState;
    dtkFontAwesome::instance()->setDefaultOption("color", QColor(Qt::white));
    d->status_item->setIcon(fa::play);
}

void dtkVisualizationWidgetsVideoControls::setBufferVisible(bool b)
{
    d->progress_bar_buffered->setVisible(b);
}

dtkVisualizationWidgetsVideoControls::State dtkVisualizationWidgetsVideoControls::state(void) const
{
    return d->state;
}

void dtkVisualizationWidgetsVideoControls::setFrameNumber(qlonglong count)
{
    d->display_frame_info = true;
    d->progress_bar->setRange(1, count);
    d->progress_bar->setValue(1);
    d->progress_bar->reset();

    d->progress_bar_buffered->setRange(1, count);
    d->progress_bar_buffered->setValue(1);
    d->progress_bar_buffered->reset();
}

void dtkVisualizationWidgetsVideoControls::setDuration(qlonglong duration)
{
    d->display_frame_info = false;
    d->progress_bar->setRange(0, duration);
    d->progress_bar_buffered->setRange(0, duration);
    d->updateDurationInfo(0);
}

void dtkVisualizationWidgetsVideoControls::setPosition(qlonglong pos)
{
    d->progress_bar->setValue(pos);
    d->updateDurationInfo(pos / 1000);
}

void dtkVisualizationWidgetsVideoControls::setBufferPosition(qlonglong pos)
{
    d->progress_bar_buffered->setValue(pos);
}

void dtkVisualizationWidgetsVideoControls::toggle(void)
{
    if (d->state == State::StoppedState || d->state == State::PausedState) {
        d->state = State::PlayingState;
        dtkFontAwesome::instance()->setDefaultOption("color", QColor(Qt::white));
        d->status_item->setIcon(fa::pause);
        emit play();

    } else {
        d->state = State::PausedState;
        dtkFontAwesome::instance()->setDefaultOption("color", QColor(Qt::white));
        d->status_item->setIcon(fa::play);
        emit pause();
    }
}

void dtkVisualizationWidgetsVideoControls::setCurrentFrame(qlonglong count)
{
    d->progress_bar->setValue(count);
}

void dtkVisualizationWidgetsVideoControls::setCurrentBufferFrame(qlonglong count)
{
    d->progress_bar_buffered->setValue(count);
}

void dtkVisualizationWidgetsVideoControls::mouseMoveEvent(QMouseEvent *event)
{
    int size = d->progress_bar->width();
    double percent = (event->pos().x() - d->progress_bar->x()) / (double)size;

    QString message;
    int frame_index = percent * d->progress_bar->maximum() / 1000.0;

    QTime currentTime((frame_index / 3600) % 60, (frame_index / 60) % 60, frame_index % 60,
                      (frame_index * 1000) % 1000);

    QString format = "mm:ss";
    if (frame_index > 3600)
        format = "hh:mm:ss";

    message = currentTime.toString(format);
    if (d->display_frame_info) {
        int frame_index = std::round(percent * d->progress_bar->maximum()
                                     + (1 - percent) * d->progress_bar->minimum());
        message += "\n" + QString::number(frame_index) + "/"
                + QString::number(d->progress_bar->maximum());
    }

    d->progress_bar->setToolTip(message);

    event->ignore();

    QFrame::mouseMoveEvent(event);
}

void dtkVisualizationWidgetsVideoControls::mousePressEvent(QMouseEvent *event)
{
    int size = d->progress_bar->width();
    double percent = (event->pos().x() - d->progress_bar->x()) / (double)size;
    int frame_index = std::round(percent * d->progress_bar->maximum()
                                 + (1.0 - percent) * d->progress_bar->minimum());
    emit seekTo(frame_index);
    event->ignore();

    QFrame::mousePressEvent(event);
}

#include "dtkVisualizationWidgetsVideoControls.moc"
