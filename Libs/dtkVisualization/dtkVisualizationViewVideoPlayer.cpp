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

#include "dtkVisualizationViewVideoPlayer.h"

#include <dtkWidgets/dtkVisualizationWidgetsVideoControls>

#include <dtkWidgets/dtkWidgetsHUD>
#include <dtkWidgets/dtkWidgetsHUDItem>
#include <dtkWidgets/dtkWidgetsOverlayPane>
#include <dtkWidgets/dtkWidgetsOverlayPaneItem>

#include <dtkFonts/dtkFontAwesome>

#include <QtGui>
#include <QtWidgets>

#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QMediaMetaData>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QVideoProbe>
#include <QtMultimedia/QVideoSurfaceFormat>

#include <QtMultimediaWidgets/QVideoWidget>

//#include <private/qvideoframe_p.h>

// ///////////////////////////////////////////////////////////////////
// VideoFrameGrabber
// ///////////////////////////////////////////////////////////////////

class VideoFrameGrabber : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoFrameGrabber(QObject *parent = nullptr);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;

    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

    bool present(const QVideoFrame &frame) override;

private:
    QImage::Format imageFormat;
    QRect targetRect;
    QRect sourceRect;
    QSize imageSize;
    QVideoFrame currentFrame;

signals:
    void frameAvailable(void);
    void frameAvailable(QImage frame);
};

VideoFrameGrabber::VideoFrameGrabber(QObject *parent) : QAbstractVideoSurface(parent), imageFormat(QImage::Format_Invalid)
{

}

QList<QVideoFrame::PixelFormat> VideoFrameGrabber::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);

    return QList<QVideoFrame::PixelFormat>()
        << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
}

bool VideoFrameGrabber::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool VideoFrameGrabber::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if ((imageFormat == QImage::Format_Invalid &&
         format.pixelFormat() != QVideoFrame::Format_YUV420P &&
         format.pixelFormat() != QVideoFrame::Format_AYUV444 ) ||
         size.isEmpty()) {

        return false;
    }

    this->imageFormat = imageFormat;
    this->imageSize   = size;
    this->sourceRect  = format.viewport();

    QAbstractVideoSurface::start(format);

    return true;
}

void VideoFrameGrabber::stop(void)
{
    this->currentFrame = QVideoFrame();
    this->targetRect   = QRect();

    QAbstractVideoSurface::stop();
}

bool VideoFrameGrabber::present(const QVideoFrame& input_frame)
{
#if 0
    if (input_frame.isValid()) {

        QVideoFrame frame(input_frame); // clone frame
        frame.map(QAbstractVideoBuffer::ReadOnly);

        if (frame.pixelFormat() == QVideoFrame::Format_YUV420P ||
            frame.pixelFormat() == QVideoFrame::Format_AYUV444
            ) { // Need to convert YUV to RGB because imageFormatFromPixelFormat does not handle YUV

            /*
            QImage tmp_img = qt_imageFromVideoFrame(frame);
            QImage image   = tmp_img.convertToFormat(QImage::Format_RGBA8888, Qt::AutoColor);

// /////////////////////////////////////////////////////////////////////////////
// TODO: NO emit
// /////////////////////////////////////////////////////////////////////////////

            emit frameAvailable();

            static_cast<dtkVisualizationViewVideoGL *>(this->parent())->setImage(image);

            */
// /////////////////////////////////////////////////////////////////////////////

            frame.unmap();

            this->currentFrame = input_frame;

            return true;

        } else {

            const QImage image(frame.bits(),
                               frame.width(),
                               frame.height(),
                               QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

// /////////////////////////////////////////////////////////////////////////////
// TODO: No emit + NO BGR TO RGB conversion on mac
// /////////////////////////////////////////////////////////////////////////////
            // if (frame.pixelFormat() == QVideoFrame::Format_ARGB32)
            //     emit frameAvailable(image.rgbSwapped());
            // else

            emit frameAvailable();

            static_cast<dtkVisualizationViewVideoGL *>(this->parent())->setImage(image);

// /////////////////////////////////////////////////////////////////////////////

            frame.unmap();
        }
    }

    if (surfaceFormat().pixelFormat() != input_frame.pixelFormat()
            || surfaceFormat().frameSize() != input_frame.size()) {
        this->setError(IncorrectFormatError);
        this->stop();
        return false;
    }

    this->currentFrame = input_frame;

    return true;
#endif
}

class dtkVisualizationViewVideoPlayerPrivate: public QObject
{
    Q_OBJECT

public:
     dtkVisualizationViewVideoPlayerPrivate(QObject *parent);
    ~dtkVisualizationViewVideoPlayerPrivate(void);

public:
    dtkVisualizationWidgetsVideoControls *controls = nullptr;
    QMediaPlayer *player = nullptr;
    QVideoProbe   *probe = nullptr;
    VideoFrameGrabber *grabber = nullptr;

public:
    dtkWidgetsOverlayPane *overlay = nullptr;
    dtkWidgetsHUD *hud = nullptr;

public slots:
    void onStateChanged(QMediaPlayer::State);

public:
    int hide_timer = 5000;

public:
    dtkVisualizationViewVideoPlayer *q = nullptr;

};

dtkVisualizationViewVideoPlayerPrivate::dtkVisualizationViewVideoPlayerPrivate(QObject *parent)
{
    this->grabber = new VideoFrameGrabber(parent);
    this->player = new QMediaPlayer(parent);
    this->player->setAudioRole(QAudio::VideoRole); // ?
    this->player->setVideoOutput(this->grabber);
}

dtkVisualizationViewVideoPlayerPrivate::~dtkVisualizationViewVideoPlayerPrivate(void)
{
    disconnect(controls, &dtkVisualizationWidgetsVideoControls::pause,  player, &QMediaPlayer::pause);
    disconnect(controls, &dtkVisualizationWidgetsVideoControls::play,   player, &QMediaPlayer::play);
    disconnect(controls, &dtkVisualizationWidgetsVideoControls::seekTo, player, &QMediaPlayer::setPosition);

    disconnect(player, &QMediaPlayer::stateChanged, this, &dtkVisualizationViewVideoPlayerPrivate::onStateChanged);
}

void dtkVisualizationViewVideoPlayerPrivate::onStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState)
        this->controls->toggle();
}


dtkVisualizationViewVideoPlayer::dtkVisualizationViewVideoPlayer(QWidget *parent) : dtkVisualizationViewVideoGL(parent)
{
    d = new dtkVisualizationViewVideoPlayerPrivate(this);
    d->q = this;

    static int count = 1;

    this->setObjectName(QString("ViewVideoPlayer - %1").arg(count++));

    QLineEdit *text_edit = new QLineEdit("TODO Settings");

    dtkWidgetsOverlayPaneItem *display_settings_item = new dtkWidgetsOverlayPaneItem(this->widget());

    display_settings_item->setTitle("Display Settings");
    display_settings_item->layout()->setContentsMargins(0, 0, 0, 0);
    display_settings_item->addWidget(text_edit);

    
    d->overlay  = new dtkWidgetsOverlayPane(this->widget());
    d->hud      = new dtkWidgetsHUD(this->widget());
    d->controls = new dtkVisualizationWidgetsVideoControls(this->widget());

    d->overlay->addWidget(display_settings_item);
    d->overlay->toggle();
    d->overlay->toggle();
    d->controls->setVisible(false);

    dtkWidgetsHUDItem *settings = d->hud->addItem(fa::sliders);
    dtkWidgetsHUDItem *open     = d->hud->addItem(fa::folderopen);

    open->setToolTip("Open File");
    settings->setToolTip("Settings");

    connect(settings, SIGNAL(clicked()), d->overlay, SLOT(toggle()));
   
    connect(open, SIGNAL(clicked()), this, SLOT(open()));

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    connect(shortcut, &QShortcut::activated, this, &dtkVisualizationViewVideoPlayer::open);

    connect(d->player, &QMediaPlayer::durationChanged, d->controls, &dtkVisualizationWidgetsVideoControls::setDuration);
    connect(d->player, &QMediaPlayer::positionChanged, d->controls, &dtkVisualizationWidgetsVideoControls::setPosition);
    connect(d->player, &QMediaPlayer::stateChanged, d, &dtkVisualizationViewVideoPlayerPrivate::onStateChanged);

    connect(d->controls, &dtkVisualizationWidgetsVideoControls::pause,  d->player, &QMediaPlayer::pause);
    connect(d->controls, &dtkVisualizationWidgetsVideoControls::play,   d->player, &QMediaPlayer::play);
    connect(d->controls, &dtkVisualizationWidgetsVideoControls::seekTo, d->player, &QMediaPlayer::setPosition);
    connect(d->controls, &dtkVisualizationWidgetsVideoControls::fullscreen, [=](bool is_full) {
            if (is_full) {
                this->widget()->window()->showFullScreen();
            } else {
                this->widget()->window()->showNormal();
            }
        });
    connect(d->controls, &dtkVisualizationWidgetsVideoControls::restart,[=] () {
            d->player->setPosition(0);
            d->player->play();
            d->controls->toggle();
        });
        

// /////////////////////////////////////////////////////////////////////////////
// TODO: Make this no async
// /////////////////////////////////////////////////////////////////////////////

//  connect(d->grabber, &VideoFrameGrabber::frameAvailable, this, &dtkVisualizationViewVideoGL::setImage);

// /////////////////////////////////////////////////////////////////////////////

    this->setAcceptDrops(true);
    this->setMouseTracking(true);

    this->widget()->setMouseTracking(true);
    
}

dtkVisualizationViewVideoPlayer::~dtkVisualizationViewVideoPlayer(void)
{
    delete d;
}

void dtkVisualizationViewVideoPlayer::open(void)
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));

    QStringList supportedMimeTypes = d->player->supportedMimeTypes();

    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("video/x-mp4"); //FIXME ?
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }

    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));

    if (fileDialog.exec() == QDialog::Accepted) {
        d->controls->reset();
        d->player->setMedia(fileDialog.selectedUrls().first());
        d->controls->setVisible(true);
        QTimer::singleShot(d->hide_timer, [=]() {d->controls->setVisible(false);});
        d->controls->toggle();
    }
}


dtkWidgetsOverlayPane *dtkVisualizationViewVideoPlayer::overlay(void)
{
    return d->overlay;
}

dtkWidgetsHUD *dtkVisualizationViewVideoPlayer::hud(void)
{
    return d->hud;
}

void dtkVisualizationViewVideoPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
        event->accept();
    else
        event->ignore();
}

void dtkVisualizationViewVideoPlayer::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void dtkVisualizationViewVideoPlayer::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasText())
        event->accept();
    else
        event->ignore();
}

void dtkVisualizationViewVideoPlayer::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {

        QString path = event->mimeData()->text();

        if(!path.startsWith("file://"))
            return;

        d->controls->reset();
        d->player->setMedia(QUrl(path));
        d->controls->setVisible(true);
        QTimer::singleShot(d->hide_timer, [=]() {d->controls->setVisible(false);});
        d->controls->toggle();

        event->accept();
    }
}
void dtkVisualizationViewVideoPlayer::mouseMoveEvent(QMouseEvent *event)
{
    if ((this->size().height() - event->pos().y()  < 64) && (!d->controls->isVisible())) {
        d->controls->setVisible(true);
        QTimer::singleShot(d->hide_timer, [=]() {d->controls->setVisible(false) ;}  );
    }
}

void dtkVisualizationViewVideoPlayer::keyPressEvent(QKeyEvent *event)
{
    dtkVisualizationViewVideoGL::keyPressEvent(event);
}

void dtkVisualizationViewVideoPlayer::resizeEvent(QResizeEvent *event)
{
    dtkVisualizationViewVideoGL::resizeEvent(event);

    d->hud->resize(event->size());

    d->overlay->resize(event->size());

    d->controls->setFixedWidth(event->size().width());
    d->controls->move(0, event->size().height() - 64);
}

#include "dtkVisualizationViewVideoPlayer.moc"

//
// dtkVisualizationViewVideoPlayer.cpp ends here
