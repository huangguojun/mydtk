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

#include "dtkWidgetsOverlayRope.h"

#include "dtkWidgetsHUD.h"
#include "dtkWidgetsHUDInfo.h"

#include <QtGui>
#include <QtMath>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayRopeVertex
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayRopeVertex
{
public:
    QVector2D velocity;
    QVector2D position;
    QVector2D oldPosition;

public:
    qreal invMass;
};

/////////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayRopePrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayRopePrivate
{
public:
    void solve(void);
    void integrate(qreal dt);
    void velocityFixup(qreal inv_dt);
    void collide(void);
    void drag(void);
    void anchor(void);
    void solveRope(qreal dt);
    void buildRope(void);
    void renderRope(QPainter *painter);
    void renderCollider(QPainter *painter);

public:
    QVector<dtkWidgetsOverlayRopeVertex> vertices;

public:
    qreal collision_radius = 40.0;

public:
    quint32 vertices_distance = 2;
    quint32 vertices_count = 60;

public:
    qreal integration_factor = (1.0 / 15.0);

public:
    qreal xf;
    qreal yf;

public:
    qreal xi;
    qreal yi;

public:
    bool collider = false;
    bool dragging = false;
    bool dynamic = false;

public:
    dtkWidgetsOverlayRope::Mode mode = dtkWidgetsOverlayRope::Collider;

public:
    dtkWidgetsHUD *hud;

public:
    dtkWidgetsOverlayRope *q;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayRopePrivate
// ///////////////////////////////////////////////////////////////////

void dtkWidgetsOverlayRopePrivate::solve(void)
{
    if(!vertices.count())
        return;

    for (int i = 0; i < vertices.count() - 1; i++)
    {
        dtkWidgetsOverlayRopeVertex& a = vertices[i];
        dtkWidgetsOverlayRopeVertex& b = vertices[i + 1];

        QVector2D v = b.position - a.position;

        qreal length = v.length();

        if (length != 0.0)
        {
            qreal error = vertices_distance / length - 1.0;

            QVector2D correction = v * error;

            if (i != 0)
            {
                qreal invMass = a.invMass + b.invMass;
                a.position -= correction * (a.invMass / invMass);
                b.position += correction * (b.invMass / invMass);
            }
            else
            {
                b.position += correction;
            }
        }
    }
}

void dtkWidgetsOverlayRopePrivate::integrate(qreal dt)
{
    for (int i = 1; i < vertices.size(); ++i)
    {
        dtkWidgetsOverlayRopeVertex& a = vertices[i];

        a.velocity += QVector2D(0.0, 9.8) * dt;
        a.position += a.velocity * dt;
    }
}

void dtkWidgetsOverlayRopePrivate::velocityFixup(qreal inv_dt)
{
    for (int i = 1; i < vertices.size(); ++i)
    {
        dtkWidgetsOverlayRopeVertex& a = vertices[i];

        a.velocity = (a.position - a.oldPosition) * inv_dt;
        a.oldPosition = a.position;
    }
}

void dtkWidgetsOverlayRopePrivate::collide(void)
{
    if (this->collider)
    {
        QVector2D mouse(xf, yf);

        for (int i = 1; i < vertices.size(); ++i)
        {
            dtkWidgetsOverlayRopeVertex& a = vertices[i];

            qreal d = QVector2D::dotProduct(a.position - mouse, a.position - mouse);

            if (d < collision_radius * collision_radius)
            {
                QVector2D dir = a.position - mouse;

                dir.normalize();
                dir *= collision_radius - qSqrt(d);
                a.position += dir;
            }
        }
    }
}

void dtkWidgetsOverlayRopePrivate::drag(void)
{
    if(!this->vertices.count())
        return;

    if(!xf || !yf)
        return;

    if (this->dragging) {
        this->vertices.last().position.setX(xf);
        this->vertices.last().position.setY(yf);
    }

    if (this->dynamic) {
        this->vertices.last().position.setX(xf);
        this->vertices.last().position.setY(yf);
    }
}

void dtkWidgetsOverlayRopePrivate::solveRope(qreal dt)
{
    this->integrate(dt);
    if (this->mode == dtkWidgetsOverlayRope::Collider)
        this->collide();
    if (this->mode == dtkWidgetsOverlayRope::Dragging)
        this->drag();
    if (this->mode == dtkWidgetsOverlayRope::Dynamic)
        this->drag();
    this->solve();
    this->velocityFixup(qreal(1.0) / dt);
}

void dtkWidgetsOverlayRopePrivate::buildRope(void)
{
    vertices.clear();

    if (this->mode != dtkWidgetsOverlayRope::Dynamic) {
        this->xi = q->width()/2.0;
        this->yi = 50.0;
    }

    for (quint32 i = 0; i < vertices_count; ++i)
    {
        dtkWidgetsOverlayRopeVertex v;

        v.position.setX(xi);
        v.position.setY(yi);
        v.oldPosition = v.position;
        v.velocity.setX(0.0);
        v.velocity.setY(0.0);
        v.invMass = 1.0 / qreal(vertices_count * 3 - i * 2);

        vertices.push_back(v);
    }
}

void dtkWidgetsOverlayRopePrivate::renderRope(QPainter *painter)
{
    if(!vertices.count())
        return;

    painter->setPen(QPen(Qt::gray, 2));

    int i;

    for (i = 0; i < vertices.size() - 1; ++i)
    {
        dtkWidgetsOverlayRopeVertex& a = vertices[i];
        dtkWidgetsOverlayRopeVertex& b = vertices[i + 1];

        painter->drawLine(a.position.toPoint(), b.position.toPoint());
    }

    painter->setPen(QPen(Qt::gray, 4));
    painter->setBrush(Qt::white);
    painter->drawRoundedRect(vertices[0].position.x() - 10, vertices[0].position.y() - 10, 20, 20, 10, 10);

    i--;

    painter->setPen(QPen(Qt::gray, 4));
    painter->setBrush(Qt::white);
    painter->drawRoundedRect(vertices[i].position.x() - 10, vertices[i].position.y() - 10, 20, 20, 10, 10);
}

void dtkWidgetsOverlayRopePrivate::renderCollider(QPainter *painter)
{
    if(!this->collider)
        return;

    QPointF start;

    QPainterPath path;

    const quint32 kSegments = 60;

    QVector2D pos(xf, yf);

    qreal theta = 0;
    qreal inc = M_PI * 2.0 / (qreal)kSegments;

    for(quint32 i = 0; i < kSegments; ++i)
    {
        theta += inc;
        QVector2D p(std::cos(theta), std::sin(theta));
        p *= collision_radius;
        p += pos;

        if (start.isNull()) {
            start = p.toPointF();
            path.moveTo(start);
        } else {
            path.lineTo(p.toPointF());
        }
    }
    path.lineTo(start);

    painter->setPen(QPen(Qt::darkRed, 2));
    painter->drawPath(path);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayRope
// ///////////////////////////////////////////////////////////////////

dtkWidgetsOverlayRope::dtkWidgetsOverlayRope(QWidget *parent) : QFrame(parent)
{
    d = new dtkWidgetsOverlayRopePrivate;

    d->q = this;

    d->hud = new dtkWidgetsHUD(this);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000/60);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    this->setAttribute(Qt::WA_DeleteOnClose);
}

dtkWidgetsOverlayRope::~dtkWidgetsOverlayRope(void)
{
    delete d;
}

void dtkWidgetsOverlayRope::toggle(void)
{
    this->setVisible(!this->isVisible());
}

void dtkWidgetsOverlayRope::setMode(Mode mode)
{
    d->mode = mode;
}

void dtkWidgetsOverlayRope::setVerticesCount(quint32 count)
{
    d->vertices_count = count;
}

void dtkWidgetsOverlayRope::setVerticesInterval(quint32 distance)
{
    d->vertices_distance = distance;
}

void dtkWidgetsOverlayRope::setIntegrationFactor(qreal factor)
{
    d->integration_factor = factor;
}

void dtkWidgetsOverlayRope::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    d->solveRope(d->integration_factor);
    d->renderRope(&painter);
    d->renderCollider(&painter);

    QRect bottom_rect = QRect(event->rect().bottomLeft().x(), event->rect().bottomLeft().y() - 50, event->rect().width(), 51);

    painter.setPen(QColor("#282828"));
    painter.setBrush(QColor("#2d2e2e"));
    painter.drawRect(bottom_rect);
    painter.setPen(QColor("#bebfbf"));
    painter.drawText(bottom_rect, "Hit Qt::Key_B to (re)build the rope, Qt::Key_C to switch to collide mode, Qt::Key_G to switch to dragging mode, Qt::Key_D to switch to dynamic mode, Qt::Key_K to clear, Qt::LeftButton to interact", QTextOption(Qt::AlignCenter));

    QFrame::paintEvent(event);
}

void dtkWidgetsOverlayRope::resizeEvent(QResizeEvent *event)
{
    d->hud->resize(event->size());

    QFrame::resizeEvent(event);
}

void dtkWidgetsOverlayRope::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_B:
        event->accept();
        d->buildRope();
        d->hud->addInfo("Built rope");
        break;
    case Qt::Key_C:
        event->accept();
        this->setMode(dtkWidgetsOverlayRope::Collider);
        d->hud->addInfo("Switched to collider mode");
        break;
    case Qt::Key_G:
        event->accept();
        this->setMode(dtkWidgetsOverlayRope::Dragging);
        d->hud->addInfo("Switched to dragging mode");
        break;
    case Qt::Key_D:
        event->accept();
        this->setMode(dtkWidgetsOverlayRope::Dynamic);
        d->hud->addInfo("Switched to dynamic mode");
        d->vertices.clear();
        d->xi = 0.0;
        d->yi = 0.0;
        break;
    case Qt::Key_K:
        event->accept();
        this->setMode(dtkWidgetsOverlayRope::Collider);
        d->hud->addInfo("Cleared rope");
        d->vertices.clear();
        break;
    default:
        event->ignore();
    }
}

void dtkWidgetsOverlayRope::mousePressEvent(QMouseEvent *event)
{
    d->xf = event->pos().x();
    d->yf = event->pos().y();

    if(!d->xi && !d->yi) {
        d->xi = d->xf;
        d->yi = d->yf;
        d->buildRope();
    }

    switch (event->button())
    {
    case Qt::LeftButton:
        event->accept();
        if (d->mode == dtkWidgetsOverlayRope::Collider) {
            d->collider = true;
            d->dragging = false;
            d->dynamic = false;
        }
        if (d->mode == dtkWidgetsOverlayRope::Dragging) {
            d->collider = false;
            d->dragging = true;
            d->dynamic = false;
        }
        if (d->mode == dtkWidgetsOverlayRope::Dynamic) {
            d->collider = false;
            d->dragging = false;
            d->dynamic = true;

            this->hide();

            if (QWidget *widget = this->parentWidget()->childAt(d->xf, d->yf)) {
                emit source(widget);
            }

            this->show();
        }
        break;
    default:
        event->ignore();
        break;
    }

    QFrame::mousePressEvent(event);
}

void dtkWidgetsOverlayRope::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();

    d->xf = event->pos().x();
    d->yf = event->pos().y();

    QFrame::mouseMoveEvent(event);
}

void dtkWidgetsOverlayRope::mouseReleaseEvent(QMouseEvent *event)
{
    d->xf = event->pos().x();
    d->yf = event->pos().y();

    switch(event->button())
    {
    case Qt::LeftButton:
        event->accept();
        if (d->mode == dtkWidgetsOverlayRope::Collider) {
            d->collider = false;
            d->dragging = false;
            d->dynamic = false;
        }
        if (d->mode == dtkWidgetsOverlayRope::Dragging) {
            d->collider = false;
            d->dragging = false;
            d->dynamic = false;
        }
        if (d->mode == dtkWidgetsOverlayRope::Dynamic) {
            d->collider = false;
            d->dragging = false;
            d->dynamic = true;

            this->hide();

            if(QWidget *widget = this->parentWidget()->childAt(d->xf, d->yf)) {
                emit target(widget);
            }

            this->show();
        }
        break;
    default:
        event->ignore();
        break;
    }

    QFrame::mouseReleaseEvent(event);
}

//
// dtkWidgetsOverlayRope.cpp ends here
