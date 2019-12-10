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

#include "dtkVisualizationViewVideo.h"

#include <QtCore>

#include <QtGui/QGuiApplication>
#include <QtGui/QImage>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QSurfaceFormat>

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewVideoPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationViewVideoPrivate : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    dtkVisualizationViewVideoPrivate(QWidget *parent = nullptr);

public:
    void initializeGL(void);
    void      paintGL(void);
    void     resizeGL(int, int);

public:
    void recalculatePosition(void);
    void renderImage(void);
    void updateScene(void);

public:
    QImage render_image;
    QImage resize_image;

public:
    float image_ratio;

public:
    int render_width;
    int render_height;
    int render_x;
    int render_y;

public:
    QString title;
};

// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewVideoPrivate::dtkVisualizationViewVideoPrivate(QWidget *parent) : QOpenGLWidget(parent), QOpenGLFunctions()
{
    QSurfaceFormat format;
    format.setVersion(1, 2);
    format.setSamples(32);

    this->QOpenGLWidget::setFormat(format);
}

void dtkVisualizationViewVideoPrivate::initializeGL(void)
{
    this->QOpenGLWidget::makeCurrent();

    this->QOpenGLFunctions::initializeOpenGLFunctions();

    QColor background_color = Qt::gray;

    float r = background_color.redF();
    float g = background_color.greenF();
    float b = background_color.blueF();

    glClearColor(r, g, b, 1.0f);
}

void dtkVisualizationViewVideoPrivate::paintGL(void)
{
    this->QOpenGLWidget::makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->renderImage();
}

void dtkVisualizationViewVideoPrivate::resizeGL(int width, int height)
{
    this->QOpenGLWidget::makeCurrent();

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    this->recalculatePosition();

    updateScene();
}

void dtkVisualizationViewVideoPrivate::updateScene(void)
{
    if (this->QWidget::isVisible()) {
        this->QWidget::update();
    }
}

void dtkVisualizationViewVideoPrivate::recalculatePosition(void)
{
    this->image_ratio = (float)this->render_image.width()/(float)this->render_image.height();

    this->render_width = this->size().width();
    this->render_height = floor(this->render_width / this->image_ratio);

    if (this->render_height > this->size().height()) {
        this->render_height = this->size().height();
        this->render_width = floor(this->render_height * this->image_ratio);
    }

    this->render_x = +floor((this->size().width()  - this->render_width)  / 2);
    this->render_y = -floor((this->size().height() - this->render_height) / 2);

    this->resize_image = QImage();
}

void dtkVisualizationViewVideoPrivate::renderImage(void)
{
    if(!this->render_image.isNull()) {
        glLoadIdentity();

        glPushMatrix();
        {
            if (this->resize_image.width() <= 0) {
                if (this->render_width == this->render_image.width() && this->render_height == this->render_image.height())
                    this->resize_image = this->render_image;
                else
                    this->resize_image = this->render_image.scaled(QSize(this->render_width * qGuiApp->devicePixelRatio(), this->render_height * qGuiApp->devicePixelRatio()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }

            glRasterPos2i(this->render_x, this->render_y);

            glPixelZoom(1, -1);

            glDrawPixels(this->resize_image.width(), this->resize_image.height(), GL_BGRA, GL_UNSIGNED_BYTE, this->resize_image.bits());
        }

        glPopMatrix();

        glFlush();
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewVideo
// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewVideo::dtkVisualizationViewVideo(QWidget *parent) : dtkWidgetsWidget(parent), d(new dtkVisualizationViewVideoPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d);
}

dtkVisualizationViewVideo::~dtkVisualizationViewVideo(void)
{
    delete d;
}

QWidget *dtkVisualizationViewVideo::widget(void)
{
    return d;
}

void dtkVisualizationViewVideo::update(void)
{
    qDebug() << Q_FUNC_INFO;
}

void dtkVisualizationViewVideo::setTitle(const QString& title)
{
    d->title = title;
}

void dtkVisualizationViewVideo::setImage(const QImage& image)
{
    d->render_image = image;

    d->recalculatePosition();

    d->updateScene();
}

//
// dtkVisualizationViewVideo.cpp ends here
