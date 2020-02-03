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

#include "dtkWidgetsOverlayPaneItem.h"

#include "dtkWidgetsLayoutItem.h"

#include <dtkFonts/dtkFontAwesome>

#include <dtkThemes/dtkThemesEngine>

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneItemHeader
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneItemHeader : public QFrame
{
    Q_OBJECT

public:
     dtkWidgetsOverlayPaneItemHeader(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneItemHeader(void);

public:
    QSize sizeHint(void) const;

public:
    void setTitle(const QString&);

public slots:
    void toggle(void);

signals:
    void toggled(bool);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    QLabel *title;
    QLabel *handle;

private:
    bool expanded = false;
};

dtkWidgetsOverlayPaneItemHeader::dtkWidgetsOverlayPaneItemHeader(QWidget *parent) : QFrame(parent)
{
    dtkFontAwesome::instance()->initFontAwesome();

    this->title = new QLabel(this);
    this->title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    this->handle = new QLabel(this);
    this->handle->setAlignment(Qt::AlignRight | Qt::AlignTop);
    this->handle->setPixmap(dtkFontAwesome::instance()->icon(fa::plussquareo).pixmap(25, 25));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->title);
    layout->addStretch();
    layout->addWidget(this->handle);

    this->setLayout(layout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

dtkWidgetsOverlayPaneItemHeader::~dtkWidgetsOverlayPaneItemHeader(void)
{

}

QSize dtkWidgetsOverlayPaneItemHeader::sizeHint(void) const
{
    return QSize(50, 50);
}

void dtkWidgetsOverlayPaneItemHeader::setTitle(const QString& title)
{
    this->title->setText(title);
}

void dtkWidgetsOverlayPaneItemHeader::toggle(void)
{
    // // QVariantMap options;
    // // options.insert("text",           dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("text-active",    dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("text-disabled",  dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("text-selected",  dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("color",          dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("color-active",   dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("color-disabled", dtkThemesEngine::instance()->color("@fg"));
    // // options.insert("color-selected", dtkThemesEngine::instance()->color("@fg"));

    this->expanded = !this->expanded;

    if (this->expanded)
        this->handle->setPixmap(dtkFontAwesome::instance()->icon(fa::minussquareo).pixmap(25, 25));
    else
        this->handle->setPixmap(dtkFontAwesome::instance()->icon(fa::plussquareo).pixmap(25, 25));

    emit toggled(this->expanded);
}

void dtkWidgetsOverlayPaneItemHeader::mousePressEvent(QMouseEvent *)
{
    this->toggle();
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsOverlayPaneItemFooter
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneItemFooter : public QFrame
{
    Q_OBJECT

public:
     dtkWidgetsOverlayPaneItemFooter(QWidget *parent = nullptr);
    ~dtkWidgetsOverlayPaneItemFooter(void);

public slots:
    void addLayout(QLayout *);
    void addWidget(QWidget *);

public slots:
    void clear(void);

private:
    QGroupBox *box;
};

dtkWidgetsOverlayPaneItemFooter::dtkWidgetsOverlayPaneItemFooter(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *box_layout = new QVBoxLayout;
    box_layout->setContentsMargins(10, 0, 10, 0);
    box_layout->setSpacing(5);

    this->box = new QGroupBox(this);
    this->box->setAlignment(Qt::AlignTop);
    this->box->setLayout(box_layout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(this->box);

    this->setVisible(false);
}

dtkWidgetsOverlayPaneItemFooter::~dtkWidgetsOverlayPaneItemFooter(void)
{

}

void dtkWidgetsOverlayPaneItemFooter::addLayout(QLayout *layout)
{
    dynamic_cast<QBoxLayout *>(this->box->layout())->addLayout(layout);
}

void dtkWidgetsOverlayPaneItemFooter::addWidget(QWidget *widget)
{
    dynamic_cast<QBoxLayout *>(this->box->layout())->addWidget(widget);
}

void dtkWidgetsOverlayPaneItemFooter::clear(void)
{
    this->layout()->removeWidget(this->box);

    this->box->deleteLater();
    this->box = nullptr;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsOverlayPaneItemPrivate
{
public:
    dtkWidgetsOverlayPaneItemHeader *header = nullptr;
    dtkWidgetsOverlayPaneItemFooter *footer = nullptr;
};

dtkWidgetsOverlayPaneItem::dtkWidgetsOverlayPaneItem(QWidget *parent) : QFrame(parent), d (new dtkWidgetsOverlayPaneItemPrivate)
{
    d->header = new dtkWidgetsOverlayPaneItemHeader(this);
    d->footer = new dtkWidgetsOverlayPaneItemFooter(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->header);
    layout->addWidget(d->footer);

    connect(d->header, SIGNAL(toggled(bool)), d->footer, SLOT(setVisible(bool)));

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

dtkWidgetsOverlayPaneItem::~dtkWidgetsOverlayPaneItem(void)
{
    delete d;
}

void dtkWidgetsOverlayPaneItem::setTitle(const QString& title)
{
    d->header->setTitle(title);
}

void dtkWidgetsOverlayPaneItem::toggle(void)
{
    d->header->toggle();
}

void dtkWidgetsOverlayPaneItem::addLayout(QLayout *layout)
{
    d->footer->addLayout(layout);
}

void dtkWidgetsOverlayPaneItem::addWidget(QWidget *widget)
{
    d->footer->addWidget(widget);
}

void dtkWidgetsOverlayPaneItem::setSlider(QWidget *slider)
{
    d->header->hide();

    d->footer->clear();
    d->footer->layout()->setContentsMargins(0, 0, 0, 0);
    d->footer->layout()->addWidget(slider);
}

// ///////////////////////////////////////////////////////////////////

#include "dtkWidgetsOverlayPaneItem.moc"

//
// dtkWidgetsOverlayPaneItem.cpp ends here
