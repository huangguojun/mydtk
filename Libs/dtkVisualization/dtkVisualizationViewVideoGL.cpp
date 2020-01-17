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

#include "dtkVisualizationViewVideoGL.h"

#include <QtGui/QImage>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLDebugLogger>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>

// /////////////////////////////////////////////////////////////////////////////

static QColor bg = QColor("#222222");

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewVideoGLPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationViewVideoGLPrivate : public QOpenGLWidget, public QOpenGLFunctions
{
public:
    dtkVisualizationViewVideoGLPrivate(QWidget *parent = nullptr);

public:
    void createShaderProgram(void);
    void createGeometry(void);
    void createTexture(void);

public:
    void initializeGL(void);
    void resizeGL(int w, int h);
    void paintGL(void);

public:
    void setVersion(int major, int minor, bool debug);

public:
    QMatrix4x4 projection_matrix, view_matrix;
    QOpenGLShaderProgram     pgm;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer            vbo;
    QOpenGLBuffer            ibo;
    GLuint                   tex;
    GLsizei                  cnt;

public:
    QImage buffer;

public:
    float rb;
    float rw;
    float rx;
    float ry;

public:
    dtkVisualizationViewVideoGL *q = nullptr;

public:
    QString title;
    bool debug;

public:
    QOpenGLContext *context;
};

// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewVideoGLPrivate::dtkVisualizationViewVideoGLPrivate(QWidget *parent) : QOpenGLWidget(parent),
                                                                                          vbo(QOpenGLBuffer::VertexBuffer),
                                                                                          ibo(QOpenGLBuffer::IndexBuffer)
{

}

void dtkVisualizationViewVideoGLPrivate::setVersion(int major, int minor, bool debug)
{
    QSurfaceFormat format;
    format.setVersion(major, minor);
    format.setDepthBufferSize(1);
    format.setAlphaBufferSize(1);
    format.setStencilBufferSize(1);
    format.setSamples(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    
    if (debug) {
        format.setOption(QSurfaceFormat::DebugContext);
    }
    this->setFormat(format);

    if (major == 3 || major == 4)
        q->version = dtkVisualizationViewVideoGL::GLSL150;
    else
        q->version = dtkVisualizationViewVideoGL::GLSL120;

    this->context = new QOpenGLContext(this);
    this->context->setFormat(format);
    this->context->create();

    if (debug && this->context->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        qDebug() << Q_FUNC_INFO << this->context << this->context->shareContext() << QOpenGLContext::globalShareContext();
        qDebug() << Q_FUNC_INFO << this->context->format();
        qDebug() << Q_FUNC_INFO << this->context->surface();
        qDebug() << "--------------------------------------------";
    }

    this->debug = debug;
}

void dtkVisualizationViewVideoGLPrivate::createShaderProgram(void)
{
    if(!pgm.addShaderFromSourceFile(QOpenGLShader::Vertex, q->version == dtkVisualizationViewVideoGL::GLSL120 ? ":dtkVisualizationViewVideoGL.120.vert" : ":dtkVisualizationViewVideoGL.150.vert")) {
        qDebug() << "Error in vertex shader:" << pgm.log();
        exit(1);
    }

    if(!pgm.addShaderFromSourceFile(QOpenGLShader::Fragment, q->version == dtkVisualizationViewVideoGL::GLSL120 ? ":dtkVisualizationViewVideoGL.120.frag" :  ":dtkVisualizationViewVideoGL.150.frag")) {
        qDebug() << "Error in fragment shader:" << pgm.log();
        exit(1);
    }

    if(!pgm.link()) {
        qDebug() << "Error linking shader pgm:" << pgm.log();
        exit(1);
    }
}

void dtkVisualizationViewVideoGLPrivate::createGeometry(void)
{
    vao.bind();

    rx = qreal(this->buffer.width())/qreal(this->buffer.height());
    ry = qreal(this->buffer.width())/qreal(this->buffer.height());

    if(rw < rb)
        rx = 1.0;
    else
        ry = 1.0;

    struct Vertex {
        GLfloat position[3],
            normal  [3],
            texcoord[2];
    } attribs[4]= {
        { { rx * -1.0f, -1.0f / ry, 0.0f }, { 0.0f, 0.0f, 1.0f }, {  0.0f, 0.0f } },
        { { rx * -1.0f,  1.0f / ry, 0.0f }, { 0.0f, 0.0f, 1.0f }, {  0.0f, 1.0f } },
        { { rx *  1.0f,  1.0f / ry, 0.0f }, { 0.0f, 0.0f, 1.0f }, {  1.0f, 1.0f } },
        { { rx *  1.0f, -1.0f / ry, 0.0f }, { 0.0f, 0.0f, 1.0f }, {  1.0f, 0.0f } }
    };

    unsigned char nvidia_bug[4];

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.bind();
    vbo.allocate(attribs, sizeof(attribs) + sizeof(nvidia_bug));

    pgm.enableAttributeArray("vertexPosition");
    pgm.setAttributeBuffer("vertexPosition", GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));

    pgm.enableAttributeArray("vertexNormal");
    pgm.setAttributeBuffer("vertexNormal", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    pgm.enableAttributeArray("texCoord2d");
    pgm.setAttributeBuffer("texCoord2d", GL_FLOAT, offsetof(Vertex, texcoord), 3, sizeof(Vertex));

    struct {
        GLubyte quad[6];
    } indices;

    indices.quad[0] = 0;
    indices.quad[1] = 1;
    indices.quad[2] = 2;
    indices.quad[3] = 0;
    indices.quad[4] = 2;
    indices.quad[5] = 3;

    cnt = 6;

    ibo.create();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.bind();
    ibo.allocate(&indices, sizeof(indices));

    vao.release();
}

void dtkVisualizationViewVideoGLPrivate::createTexture(void)
{
    this->glGenTextures(1, &tex);
}

void dtkVisualizationViewVideoGLPrivate::initializeGL(void)
{
    if (this->debug && QOpenGLWidget::context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        qDebug() << Q_FUNC_INFO << 1 << this->context << this->context->shareContext() << QOpenGLContext::globalShareContext();
        qDebug() << Q_FUNC_INFO << 1 << this->context->format();
        qDebug() << "--------------------------------------------";
    }

    QOpenGLFunctions::initializeOpenGLFunctions();

    createShaderProgram();

    pgm.bind();

    view_matrix.setToIdentity();

// /////////////////////////////////////////////////////////////////////////////
// TODO: Make sure works for both 2.1 & 4.1 formats
// /////////////////////////////////////////////////////////////////////////////

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_TEXTURE_2D);

    // glActiveTexture(GL_TEXTURE0);

// /////////////////////////////////////////////////////////////////////////////

    pgm.setUniformValue("tex", 0);

    createTexture();

    vao.create();

    glClearColor(::bg.redF(), ::bg.greenF(), ::bg.blueF(), 1.f);

    createGeometry();

    if (this->debug && QOpenGLWidget::context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        qDebug() << Q_FUNC_INFO << 2 << this->context << this->context->shareContext() << QOpenGLContext::globalShareContext();
        qDebug() << Q_FUNC_INFO << 2 << this->context->format();
        qDebug() << "--------------------------------------------";
    }
}

void dtkVisualizationViewVideoGLPrivate::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    projection_matrix.setToIdentity();

    rb = qreal(this->buffer.width())/qreal(this->buffer.height());
    rw = qreal(w)/qreal(h);

    if (rw < rb)
        projection_matrix.ortho(-1.f, 1.f, -1.f*h/w, 1.f*h/w, 0, 1.f);
    else
        projection_matrix.ortho(-1.f*w/h, 1.f*w/h, -1.f, 1.f, 0, 1.f);

    update();
}

void dtkVisualizationViewVideoGLPrivate::paintGL(void)
{
    if (this->debug && QOpenGLWidget::context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        qDebug() << Q_FUNC_INFO;
        qDebug() << "Current context:" << QOpenGLWidget::context();
        qDebug() << "Current context:" << QOpenGLWidget::context()->format();
        qDebug() << "   THIS context:" << this->context << this->context->shareContext() << QOpenGLContext::globalShareContext();
        qDebug() << "   THIS context:" << this->context->format();
        qDebug() << "------------------------------------------";
    }

    if(this->buffer.isNull())
        return;

    createGeometry();

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

    QOpenGLContext *context = QOpenGLContext::currentContext();

    bool enable = context->hasExtension(QByteArrayLiteral("GL_KHR_debug"));

    static QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);

    if (enable)
        logger->initialize(); // initializes in the current context, i.e. ctx
// ///////////////////////////////////////////////////////////////////

    this->glBindTexture  (GL_TEXTURE_2D, tex);
    this->glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);
#if defined(Q_OS_MAC)
    this->glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, this->buffer.width(), this->buffer.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, this->buffer.bits());
#else
    this->glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA, this->buffer.width(), this->buffer.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, this->buffer.bits());
#endif
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glClearColor(::bg.redF(), ::bg.greenF(), ::bg.blueF(), 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, tex);

    QMatrix4x4 model;
    QMatrix4x4 model_view_matrix = view_matrix * model;

    pgm.bind();
    pgm.setUniformValue("modelViewMatrix", model_view_matrix);
    pgm.setUniformValue("projectionMatrix", projection_matrix);

    vao.bind();

    glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_BYTE, 0);

    pgm.release();
    vao.release();
    vbo.release();
    ibo.release();

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

    if (enable) {
        const QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }

// ///////////////////////////////////////////////////////////////////
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewVideoGL::dtkVisualizationViewVideoGL(QWidget *parent) : dtkWidgetsWidget(parent)
{
    /*
    d = new dtkVisualizationViewVideoGLPrivate(this);
    d->q = this;
    d->setVersion(4, 3, false);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d);
    */
}

dtkVisualizationViewVideoGL::~dtkVisualizationViewVideoGL(void)
{
    delete d;
}
void dtkVisualizationViewVideoGL::setVersion( int major, int minor, bool debug)
{
    d->setVersion(major, minor, debug);
}

QWidget *dtkVisualizationViewVideoGL::widget(void)
{
    QOpenGLContext *context = d->QOpenGLWidget::context();

    if (context && d->debug && context->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        qDebug() << Q_FUNC_INFO << d->context << d->context->shareContext() << QOpenGLContext::globalShareContext();
        qDebug() << Q_FUNC_INFO << d->format();
        qDebug() << "--------------------------------------------";
    }

    return d;
}

void dtkVisualizationViewVideoGL::update(void)
{
    //dtkWidgetsWidget::update();
}

void dtkVisualizationViewVideoGL::setTitle(const QString& title)
{
    d->title = title;
}

void dtkVisualizationViewVideoGL::setImage(const QImage& image)
{
    d->buffer = image.mirrored(false, true);

    d->resizeGL(d->width(), d->height());
}

float dtkVisualizationViewVideoGL::ratioBuffer(void) const
{
    return d->rb;
}

float dtkVisualizationViewVideoGL::ratioWidget(void) const
{
    return d->rw;
}

//
// dtkVisualizationViewVideoGL.cpp ends here
