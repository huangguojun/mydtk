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

#include "dtkWidgetsMenuBar.h"
#include "dtkWidgetsMenuBar_p.h"
#include "dtkWidgetsMenu+ux.h"
#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenuBar_p.h"
#include "dtkWidgetsMenuSpy.h"
#include "dtkWidgetsOverlayPane.h"
#include "dtkWidgetsOverlayPaneSlider.h"

#include <dtkFonts/dtkFontAwesome>

#include <dtkThemes/dtkThemesEngine>

// ///////////////////////////////////////////////////////////////////

namespace dtkWidgetsMenuBarHelper {

void handle(const QList<QWidget *>& items, dtkWidgetsMenuBarContainer *c, dtkWidgetsMenuBar *mb = nullptr)
{
    auto setup = [=] (QWidget *facade, int i) {
        if (items.count() == 1 || (i > 1 && i < items.count() - 1 && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i - 1)) && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i + 1)))) {
            facade->setProperty("state", "alone");
        } else if (facade == items.last()) {
            facade->setProperty("state", "last");
            if(items.count() > 1 && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i - 1))) {
                facade->setProperty("state", "alone");
            }
        } else if (i < items.count() - 1 && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i + 1))) {
            facade->setProperty("state", "last");
        } else if (i == 0) {
            facade->setProperty("state", "first");
            if(items.count() > 1 && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i + 1))) {
                facade->setProperty("state", "alone");
            }
        } else if (i > 1 && dynamic_cast<dtkWidgetsMenuItemSpacerFacade *>(items.at(i - 1))) {
            facade->setProperty("state", "first");
        } else {
            facade->setProperty("state", "inner");
        }
    };

    int i = 0;
    for (QWidget *item : items) {
        if (!item)
            continue;

        if (dtkWidgetsMenuItemFacade *facade = dynamic_cast<dtkWidgetsMenuItemFacade *>(item)) {
            QObject::connect(facade, &dtkWidgetsMenuItemFacade::clicked, [=] (void) -> void
            {
                facade->menu_item->emit triggered();
                facade->menu_item->emit clicked();
            });

        } else if (dtkWidgetsMenuInnerFacade *facade = dynamic_cast<dtkWidgetsMenuInnerFacade *>(item)) {

            facade->parent_index = i;

            QObject::connect(facade, &dtkWidgetsMenuInnerFacade::clicked, [=](void) -> void
            {
                c->switchToNextSlide(facade->menu);

                if (mb && facade->parent_index >= 0) {
                    mb->setCurrentIndex(facade->parent_index);
                }
            });
        }

        setup(item, i++);
    }
}

} // namespace dtkWidgetsMenuBarHelper

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class dtkWidgetsMenuBarButtonCollapse : public QLabel
{
    Q_OBJECT

public:
     dtkWidgetsMenuBarButtonCollapse(dtkWidgetsMenuBar *parent = nullptr);
    ~dtkWidgetsMenuBarButtonCollapse(void);

signals:
    void clicked(void);

public:
    QSize sizeHint(void) const override;

protected:
    void mousePressEvent(QMouseEvent *) override;

public:
    void touch(void);

public:
    bool collapsed = true;
    QMetaObject::Connection m_connection;

public:
    dtkWidgetsMenuBar *q;
};

dtkWidgetsMenuBarButtonCollapse::dtkWidgetsMenuBarButtonCollapse(dtkWidgetsMenuBar *parent) : QLabel(parent)
{
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setMouseTracking(true);
    this->q = parent;
    this->touch();

    m_connection = connect(dtkThemesEngine::instance(), &dtkThemesEngine::changed, [=]()
    {
        this->touch();
    });
}

dtkWidgetsMenuBarButtonCollapse::~dtkWidgetsMenuBarButtonCollapse(void)
{
    disconnect(m_connection);
}

QSize dtkWidgetsMenuBarButtonCollapse::sizeHint(void) const
{
    return QSize(q->d->size + 2 * q->d->margin, q->d->size + 2 * q->d->margin);
}

void dtkWidgetsMenuBarButtonCollapse::touch(void)
{
    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@base7"));

    this->setToolTip(this->collapsed ? "Expand" : "Collapse");
    this->setPixmap(dtkFontAwesome::instance()->icon(this->collapsed ? fa::chevronright : fa::chevronleft).pixmap(QSize(q->d->size * 3 / 4, q->d->size * 3 / 4)));

    dtkThemesEngine::instance()->polish(this);
}

void dtkWidgetsMenuBarButtonCollapse::mousePressEvent(QMouseEvent *event)
{
    this->collapsed = !this->collapsed;

    this->emit clicked();

    this->touch();

    event->accept();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarButton
// /////////////////////////////////////////////////////////////////////////////

class dtkWidgetsMenuBarButton : public QLabel
{
    Q_OBJECT

public:
     dtkWidgetsMenuBarButton(int id, const QString &title, dtkWidgetsMenuBar *parent = nullptr);
    ~dtkWidgetsMenuBarButton(void);

signals:
    void clicked(void);

public slots:
    void touch(bool);

public:
    bool selected(void) const;

public:
    QSize sizeHint(void) const override;

protected:
    void mousePressEvent(QMouseEvent *) override;

    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

public:
    dtkWidgetsMenu *menu = nullptr;

private:
    bool m_selected = false;
    int icon_id = 0;
    QMetaObject::Connection m_connection;

private:
    dtkWidgetsMenuBar *q;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

dtkWidgetsMenuBarButton::dtkWidgetsMenuBarButton(int id, const QString &title, dtkWidgetsMenuBar *parent) : QLabel(parent)
{
    this->icon_id = id;
    this->setAlignment(Qt::AlignCenter);
    this->setToolTip(title);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setMouseTracking(true);

    this->q = parent;

    this->touch(false);

    m_connection = connect(dtkThemesEngine::instance(), &dtkThemesEngine::changed, [=]()
    {
        this->touch(this->m_selected);
    });
}

dtkWidgetsMenuBarButton::~dtkWidgetsMenuBarButton(void)
{
    disconnect(m_connection);
}

bool dtkWidgetsMenuBarButton::selected(void) const
{
    return this->m_selected;
}

void dtkWidgetsMenuBarButton::touch(bool selected)
{
    this->m_selected = selected;

    if (!selected) {
        dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@base7"));
        this->setProperty("state", "default");

    } else {
        dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@fg"));
        this->setProperty("state", "current");
    }

    this->setPixmap(dtkFontAwesome::instance()->icon(this->icon_id).pixmap(QSize(q->d->size * 3 / 4, q->d->size * 3 / 4)));

    dtkThemesEngine::instance()->polish(this);
}

QSize dtkWidgetsMenuBarButton::sizeHint(void) const
{
    return QSize(q->d->size, q->d->size);
}

void dtkWidgetsMenuBarButton::mousePressEvent(QMouseEvent *event)
{
    this->emit clicked();

    event->accept();
}

void dtkWidgetsMenuBarButton::enterEvent(QEvent *)
{
    dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@fg"));

    this->setPixmap(dtkFontAwesome::instance()->icon(this->icon_id).pixmap(QSize(q->d->size * 3 / 4, q->d->size * 3 / 4)));
}

void dtkWidgetsMenuBarButton::leaveEvent(QEvent *)
{
    if (!this->m_selected) {
        dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@base7"));
        this->setPixmap(dtkFontAwesome::instance()->icon(this->icon_id).pixmap(QSize(q->d->size * 3 / 4, q->d->size * 3 / 4)));
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

dtkWidgetsMenuBarContainerSlide::dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *q, dtkWidgetsMenuBarContainer *parent) : QFrame(parent)
{
    this->q = q;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

dtkWidgetsMenuBarContainerSlide::dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *q, dtkWidgetsMenu *m, dtkWidgetsMenuBarContainer *c) : QFrame(c), menu(m)
{
    this->q = q;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFixedWidth(c->width());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    this->touch(m, c);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

dtkWidgetsMenuBarContainerSlide::dtkWidgetsMenuBarContainerSlide(dtkWidgetsMenuBar *q, dtkWidgetsMenuBar *mb, dtkWidgetsMenuBarContainer *c) : QFrame(c), menu_bar(mb)
{
    this->q = q;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFixedWidth(c->width());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    this->touch(mb, c);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

dtkWidgetsMenuBarContainerSlide::~dtkWidgetsMenuBarContainerSlide(void)
{
    menu = nullptr;
    menu_bar = nullptr;
}

void dtkWidgetsMenuBarContainerSlide::touch(dtkWidgetsMenu *m, dtkWidgetsMenuBarContainer *c)
{
    dtkWidgetsMenuFacade *facade = new dtkWidgetsMenuFacade(m);

    this->layout()->addWidget(new dtkWidgetsMenuItemSpacerFacade(q->d->margin/2, q->d->margin/2, this));
    this->layout()->addWidget(facade);

    ::dtkWidgetsMenuBarHelper::handle(facade->items(), c);

    static_cast<QBoxLayout *>(this->layout())->addStretch();
}

void dtkWidgetsMenuBarContainerSlide::touch(dtkWidgetsMenuBar *mb, dtkWidgetsMenuBarContainer *c)
{
    QLayoutItem *item = nullptr; while ((item = this->layout()->takeAt(0))) delete item->widget();

    for (dtkWidgetsMenu *m : mb->menus()) {

        dtkWidgetsMenuHeaderFacade *header = new dtkWidgetsMenuHeaderFacade(m);
        dtkWidgetsMenuFacade *widget = new dtkWidgetsMenuFacade(m);

        this->layout()->addWidget(header);
        this->layout()->addWidget(widget);

        ::dtkWidgetsMenuBarHelper::handle(widget->items(), c);
    }

    this->layout()->addWidget(new dtkWidgetsMenuItemSpacerFacade(q->d->margin/2, q->d->margin/2, this));

    static_cast<QBoxLayout *>(this->layout())->addStretch();
}

void dtkWidgetsMenuBarContainerSlide::touch(const QVector<dtkWidgetsMenu *>& menus, dtkWidgetsMenuBarContainer *c)
{
    QLayoutItem *item = nullptr; while ((item = this->layout()->takeAt(0))) delete item->widget();

    for (dtkWidgetsMenu *m : menus) {

        dtkWidgetsMenuHeaderFacade *header = new dtkWidgetsMenuHeaderFacade(m);
        dtkWidgetsMenuFacade *widget = new dtkWidgetsMenuFacade(m);

        this->layout()->addWidget(header);
        this->layout()->addWidget(widget);

        ::dtkWidgetsMenuBarHelper::handle(widget->items(), c);
    }

    int margin; q ? margin = q->d->margin : margin = 12;

    this->layout()->addWidget(new dtkWidgetsMenuItemSpacerFacade(margin/2, margin/2, this));

    static_cast<QBoxLayout *>(this->layout())->addStretch();
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarContainer implementation
// ///////////////////////////////////////////////////////////////////

QList<dtkWidgetsMenu *> stack;

dtkWidgetsMenuBarContainer::dtkWidgetsMenuBarContainer(QWidget *parent) : dtkWidgetsOverlayPane(parent)
{
    this->slider = new dtkWidgetsOverlayPaneSlider;
    this->slider->setBound(300);
    this->slider->toggle();
    this->slider->enableSpying(true);

    this->navigator = new dtkWidgetsMenuNavigationFacade(this);

    this->addWidget(this->navigator);
    this->addWidget(this->slider);

    connect(navigator, &dtkWidgetsMenuNavigationFacade::clicked, [=] (void) -> void
    {
        if (this->slider->is_in_transition) 
            return;
        dtkWidgetsMenu *menu = nullptr;

        if (::stack.count())
            menu = ::stack.takeLast();

        this->switchToPrevSlide(menu);
    });
}

dtkWidgetsMenuBarContainer::~dtkWidgetsMenuBarContainer(void)
{

}

void dtkWidgetsMenuBarContainer::setCurrentIndex(int index, std::function<void ()>& callback)
{
    if (this->slider->is_in_transition)
        return;

    this->slider->enableSpying(false);
    this->slider->blockSignals(true);

    static std::function<void (void)> ca = [=] (void) -> void
    {
        this->slider->blockSignals(false);
        this->slider->enableSpying(true);
        callback();
    };

    std::function<void (void)> cb = [=] (void) -> void
    {
        this->slider->setCurrentIndex(index, ca);
    };

    if (::stack.count())
        this->switchToRoot(cb);
    else
        cb();
}

void dtkWidgetsMenuBarContainer::switchToRoot(std::function<void(void)> &callback)
{
    if (this->slider->is_in_transition)
        return;

    this->navigator->setMenu(0);

    this->slider->enableSpying(false);
    this->slider->blockSignals(true);

    static std::function<void(void)> cb = [=](void) -> void
    {
        while (!::stack.isEmpty())
            this->slider->remSlide(this->slides[::stack.takeLast()]);

        callback();

        this->slider->blockSignals(false);
        this->slider->enableSpying(false);
    };

    this->slider->slideTo(0, cb);
}

void dtkWidgetsMenuBarContainer::switchToPrevSlide(dtkWidgetsMenu *m)
{
    if (this->slider->is_in_transition)
        return;

    if (m)
        emit q->left(m);

    if (::stack.count())
        this->navigator->setMenu(::stack.last());
    else
        this->navigator->setMenu(0);

    std::function<void()> callback = [=] (void) -> void
    {
        this->slider->remSlide(this->slides[m]);
    };

    if (!m) {
        this->q->d->toggle->collapsed = !this->q->d->toggle->collapsed;
        this->q->d->toggle->touch();
        this->toggle();
    } else {
        this->slider->slideToPrevious(callback);
    }

    if(::stack.count())
        emit q->entered(::stack.last());
}

void dtkWidgetsMenuBarContainer::switchToNextSlide(dtkWidgetsMenu *m)
{
    if (this->slider->is_in_transition)
        return;

    if(::stack.count())
        emit q->left(::stack.last());

    this->slider->addSlide(fa::cogs, this->slides[m]);
    this->slider->slideToNext();

    this->navigator->setMenu(m);

    ::stack << m;

    emit q->entered(m);
}

void dtkWidgetsMenuBarContainer::touch(void)
{
    if (this->slider->is_in_transition)
        return;

    ::stack.clear();
    navigator->setMenu(0);
    this->master_slide->touch(this->master_menus, this);
}

void dtkWidgetsMenuBarContainer::touch(dtkWidgetsMenuBar *mb)
{
    if (this->slider->is_in_transition)
        return;

    ::stack.clear();
    navigator->setMenu(0);

    if(!this->master_slide) {

        this->master_slide = new dtkWidgetsMenuBarContainerSlide(this->q, mb, this);
        this->slider->addSlide(fa::cogs, this->master_slide);

    } else {

        qDeleteAll(this->slides);

        this->slides.clear();

        this->master_slide->touch(mb, this);
    }

    for (auto m : mb->menus()) {

        for (auto mm : m->menus()) {

            this->buildChildSlide(mm);
        }
    }
}

void dtkWidgetsMenuBarContainer::decr(void)
{
    if (this->slider->is_in_transition)
        return;

    this->slider->decr();

    ::stack.takeLast();
}

void dtkWidgetsMenuBarContainer::build(const QVector<dtkWidgetsMenu *>& menus)
{
    if(!this->master_slide) {
        this->master_slide = new dtkWidgetsMenuBarContainerSlide(q, this);
        this->slider->addSlide(fa::cogs, this->master_slide);
    }

    this->master_slide->touch(menus, this);

    this->master_menus = menus;
}

void dtkWidgetsMenuBarContainer::buildChildSlide(dtkWidgetsMenu *menu)
{
    dtkWidgetsMenuBarContainerSlide *slide = new dtkWidgetsMenuBarContainerSlide(q, menu, this);
    slide->setVisible(false);

    this->slides.insert(menu, slide);

    for (auto m : menu->menus())
        this->buildChildSlide(m);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBarPrivate
// ///////////////////////////////////////////////////////////////////

void dtkWidgetsMenuBarPrivate::touch(void)
{
    QLayoutItem *child; while ((child = this->layout->takeAt(0)) != 0) delete child->widget();

    for (dtkWidgetsMenu *m : menu_list) {

        dtkWidgetsMenuBarButton *b = new dtkWidgetsMenuBarButton(m->icon(), m->title(), q);
        b->menu = m;

        QObject::connect(b, &dtkWidgetsMenuBarButton::clicked, [=] (void) -> void {

            int index = 0;
            int target = 0;
            bool selected = b->selected();

            for (int i = 0; i < this->layout->count(); ++i) {

                if (dtkWidgetsMenuBarButton *button = dynamic_cast<dtkWidgetsMenuBarButton *>(this->layout->itemAt(i)->widget())) {
                    if (button == b) {
                        button->touch(true);
                        target = index;
                    } else {
                        button->touch(false);
                    }
                    ++index;
                }
            }

            std::function<void(void)> callback = [=](void) -> void {
                this->c->blockSignals(false);
                this->c->slider->blockSignals(false);
                q->emit clicked(target);
            };

            if(!this->c->width() || selected) {
                this->toggle->collapsed = !this->toggle->collapsed;
                this->toggle->touch();
                this->c->slider->blockSignals(true);
                this->c->blockSignals(true);
                this->c->toggle(callback);
            } else {
                callback();
            }
        });

        this->layout->addWidget(b);
    }

    this->toggle->touch();
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBar
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuBar::dtkWidgetsMenuBar(QWidget *parent) : QFrame(parent), d(new dtkWidgetsMenuBarPrivate)
{
    d->q = this;

    // QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    // effect->setBlurRadius(20);

    // this->setGraphicsEffect(effect);

    d->c = new dtkWidgetsMenuBarContainer(parent);
    d->c->q = this;
    d->c->toggle();
    d->c->toggle();

    connect(d->c->slider, SIGNAL(indexVisible(int)), d->c, SIGNAL(indexVisible(int)));

    d->s = new dtkWidgetsMenuSpy;
    d->s->setMenuBar(this);
    d->s->setMenuBarContainer(d->c);

    d->layout = new QVBoxLayout;
    d->layout->setContentsMargins(d->margin, d->margin, d->margin, d->margin);
    d->layout->setSpacing(d->margin);
    d->layout->setAlignment(Qt::AlignTop);

    QFrame *viewport = new QFrame(this);
    viewport->setLayout(d->layout);
    viewport->setFixedWidth(d->size + 2 * d->margin);

    d->scroll = new QScrollArea(this);
    d->scroll->setFrameShape(QFrame::NoFrame);
    d->scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->scroll->setWidgetResizable(true);
    d->scroll->setWidget(viewport);

    d->toggle = new dtkWidgetsMenuBarButtonCollapse(this);

    static std::function<void(void)> callback = [=] (void) -> void {

        d->c->blockSignals(false);
        d->c->slider->blockSignals(false);
    };

    connect(d->toggle, &dtkWidgetsMenuBarButtonCollapse::clicked, [=] (void) -> void {
        d->c->slider->blockSignals(true);
        d->c->blockSignals(true);
        d->c->toggle(callback);
    });

    this->setFixedWidth(d->size + 2 * d->margin);
}

dtkWidgetsMenuBar::~dtkWidgetsMenuBar(void)
{
    delete d;
}

dtkWidgetsMenu *dtkWidgetsMenuBar::addMenu(dtkWidgetsMenu *menu)
{
    if (menu && !d->menu_hash.contains(menu->title())) {
        menu->setParent(this);

        d->menu_hash.insert(menu->title(), menu);
        d->menu_list << menu;

        return menu;
    }

    return nullptr;
}

dtkWidgetsMenu *dtkWidgetsMenuBar::addMenu(int icon_id, const QString &title)
{
    dtkWidgetsMenu *menu = new dtkWidgetsMenu(icon_id, title);

    auto m = this->addMenu(menu);

    if (m != menu)
      delete menu;

    return m;
}

dtkWidgetsMenu *dtkWidgetsMenuBar::insertMenu(int pos, dtkWidgetsMenu *menu)
{
    if (pos < 0 || pos > d->menu_list.size() || !menu || d->menu_hash.contains(menu->title())) {
      return nullptr;
    }

    menu->setParent(this);

    d->menu_hash.insert(menu->title(), menu);
    d->menu_list.insert(pos, menu);

    return menu;
}

dtkWidgetsMenu *dtkWidgetsMenuBar::insertMenu(int pos, int icon_id, const QString &title)
{
    dtkWidgetsMenu *menu = new dtkWidgetsMenu(icon_id, title);

    auto m = this->insertMenu(pos, menu);

    if (m != menu)
        delete menu;

    return m;
}

void dtkWidgetsMenuBar::removeMenu(dtkWidgetsMenu *menu)
{
    if (!menu)
        return;

    this->removeMenu(menu->title());
}

void dtkWidgetsMenuBar::removeMenu(const QString &title)
{
    auto m = d->menu_hash.take(title);

    if (m) {
        d->menu_list.removeOne(m);
        d->touch();
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

void build(QMenu *menu, dtkWidgetsMenu *w_menu)
{
    foreach(QAction *action, menu->actions()) {

        dtkWidgetsMenu *w_s_menu = 0;

        if(QMenu *s_menu = action->menu()) {

            w_s_menu = w_menu->addMenu(fa::circle, action->text());

            build(s_menu, w_s_menu);
        } else {

            if(action->isSeparator()) {
                w_menu->addSeparator();
            } else {

                dtkWidgetsMenuItem *item = w_menu->addItem(fa::dashcube, action->text());

                QObject::connect(item, SIGNAL(clicked()), action, SLOT(trigger()));
            }
        }
    }
}

// /////////////////////////////////////////////////////////////////////////////

QList<dtkWidgetsMenu *> dtkWidgetsMenuBar::build(const QString& prefix, QMenuBar *bar)
{
    QList<dtkWidgetsMenu *> menus;

    foreach(QAction *action, bar->actions()) {

        if(action->text().isEmpty())
            continue;

        dtkWidgetsMenu *w_menu = new dtkWidgetsMenu(fa::circle, QString(prefix + "::" + action->text()));

        ::build(action->menu(), w_menu);

        menus << w_menu;
    }

    return menus;
}

dtkWidgetsMenu *dtkWidgetsMenuBar::menu(const QString& id)
{
    QStringList path = id.split(":", QString::SkipEmptyParts);

    if(path.isEmpty())
        return nullptr;

    if(!d->menu_hash.contains(path.first()))
        return nullptr;

    dtkWidgetsMenu *root = d->menu_hash[path.takeFirst()];
    dtkWidgetsMenu *meny = root;

    std::function<void (dtkWidgetsMenu *)> sniff = [&] (dtkWidgetsMenu *menu) -> void {

        if(path.isEmpty())
            return;

        QObject *object = menu->object(path.takeFirst());

        if (dtkWidgetsMenu *m = qobject_cast<dtkWidgetsMenu *>(object)) {
            meny = m;
            sniff(m);
        }
    };

    if(root)
        sniff(root);

    if (meny)
        return meny;
    else
        return nullptr;
}

dtkCoreParameter *dtkWidgetsMenuBar::parameter(const QString& id)
{
    QStringList path = id.split(":", QString::SkipEmptyParts);

    if(path.isEmpty())
        return nullptr;

    dtkWidgetsMenu *root = d->menu_hash[path.takeFirst()];
    dtkWidgetsMenuItemParameter *parameter = nullptr;

    std::function<void (dtkWidgetsMenu *)> sniff = [&] (dtkWidgetsMenu *menu) -> void {

        if(path.isEmpty())
            return;

        QObject *object = menu->object(path.takeFirst());

        if (dtkWidgetsMenu *m = qobject_cast<dtkWidgetsMenu *>(object))
            sniff(m);
        else
            parameter = qobject_cast<dtkWidgetsMenuItemParameter *>(object);
    };

    if(root)
        sniff(root);

    if (parameter)
        return parameter->parameter();
    else
        return nullptr;
}

dtkWidgetsParameter *dtkWidgetsMenuBar::parameterWidget(const QString& id)
{
    QStringList path = id.split(":", QString::SkipEmptyParts);

    if(path.isEmpty())
        return nullptr;

    dtkWidgetsMenu *root = d->menu_hash[path.takeFirst()];
    dtkWidgetsMenuItemParameter *parameter = nullptr;

    std::function<void (dtkWidgetsMenu *)> sniff = [&] (dtkWidgetsMenu *menu) -> void {

        if(path.isEmpty())
            return;

        QObject *object = menu->object(path.takeFirst());

        if (dtkWidgetsMenu *m = qobject_cast<dtkWidgetsMenu *>(object))
            sniff(m);
        else
            parameter = qobject_cast<dtkWidgetsMenuItemParameter *>(object);
    };

    if(root)
        sniff(root);

    if (parameter)
        return parameter->widget();
    else
        return nullptr;
}

void dtkWidgetsMenuBar::setCurrentIndex(int index)
{
    int n = 0;
    for (int i = 0; i < d->layout->count(); ++i) {
        if (dtkWidgetsMenuBarButton *button = dynamic_cast<dtkWidgetsMenuBarButton *>(d->layout->itemAt(i)->widget())) {
            button->touch(n == index);
            (n == index) ? d->scroll->ensureWidgetVisible(button) : (void)index;
            ++n;
        }
    }
}

void dtkWidgetsMenuBar::setCurrentIndex(int index, std::function<void (void)>& callback)
{
    this->setCurrentIndex(index);

    callback();
}

int dtkWidgetsMenuBar::size(void) const
{
  return d->menu_list.size();
}

QVector<dtkWidgetsMenu *> dtkWidgetsMenuBar::menus(void) const
{
    return d->menu_list;
}

void dtkWidgetsMenuBar::setInteractive(bool interactive)
{
    d->interactive = interactive;
}

void dtkWidgetsMenuBar::setStandalone(bool standalone)
{
    d->c->setVisible(!standalone);
    d->toggle->setVisible(!standalone);
}

void dtkWidgetsMenuBar::setFixedHeight(int h)
{
    QFrame::setFixedHeight(h);

    d->c->setFixedHeight(h);
}

void dtkWidgetsMenuBar::setWidth(int width)
{
    d->size = width;

    this->setFixedWidth(d->size + 2 * d->margin);
}

void dtkWidgetsMenuBar::setMargins(int margin)
{
    d->margin = margin;

    d->layout->setContentsMargins(d->margin, d->margin, d->margin, d->margin);
    d->layout->setSpacing(d->margin);

    this->setFixedWidth(d->size + 2 * d->margin);
}

void dtkWidgetsMenuBar::setOffset(int offset)
{
    d->offset = offset;
}

void dtkWidgetsMenuBar::resizeEvent(QResizeEvent *event)
{
    d->scroll->move(0, 20); // small vertical offset is needed to not overlap MainWindowButton on Mac (left-hand side)
    d->scroll->resize(event->size() - QSize(0, d->size + 2 * d->margin));

    d->toggle->move(0, event->size().height() - d->size - 2 * d->margin);

    d->c->move(this->rect().topRight() + QPoint(1 + d->offset, 0));

    QFrame::resizeEvent(event);
}

void dtkWidgetsMenuBar::mousePressEvent(QMouseEvent *event)
{
    if(!(event->buttons() & Qt::LeftButton))
        return;

    d->o_pos = event->globalPos();
}

void dtkWidgetsMenuBar::mouseMoveEvent(QMouseEvent *event)
{
    if(!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos().y() < ((d->size + d->margin) * this->size())) ||
        (event->pos().y() > d->c->size().height() - (d->size + d->margin)))
        return;

    d->c_pos = event->globalPos();

    d->d_pos = d->c_pos - d->o_pos;

    if (d->interactive)
        this->topLevelWidget()->move(this->topLevelWidget()->pos() + d->d_pos);

    d->o_pos = d->c_pos;
}

void dtkWidgetsMenuBar::touch(void)
{
    d->touch();

    d->c->touch(this);
    this->repaint();
}

void dtkWidgetsMenuBar::decr(void)
{
    d->c->decr();
}

void dtkWidgetsMenuBar::reset(void)
{
    d->c->slider->reset();
}

QDebug operator<<(QDebug debug, const dtkWidgetsMenuBar &c)
{
    debug << "MenuBar state in ";
    debug << "d->layout->count() = " << c.d->layout->count();
    debug << "::stack.count() = " << ::stack.count();
    //c.d->c->slider->print();
    return debug;
}

QDebug operator<<(QDebug debug, dtkWidgetsMenuBar *c)
{
    debug << "MenuBar state in ";
    debug << "d->layout->count() = " << c->d->layout->count();
    debug << "::stack.count() = " << ::stack.count();
    //c->d->c->slider->print();
    return debug;
}


// /////////////////////////////////////////////////////////////////////////////

#include "dtkWidgetsMenuBar.moc"

//
// dtkWidgetsMenuBar.cpp ends here
