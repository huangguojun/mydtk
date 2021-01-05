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

#pragma once

#include <dtkWidgetsExport>

#include <QtCore>

#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>

class dtkCoreParameter;
class dtkWidgetsMenu;
class dtkWidgetsParameter;

// /////////////////////////////////////////////////////////////////////////////
// dtkWidgetsMenuBar
// /////////////////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenuBar : public QFrame
{
    Q_OBJECT

public:
    dtkWidgetsMenuBar(QWidget * = nullptr);
    ~dtkWidgetsMenuBar(void);

signals:
    void clicked(int);

    void entered(dtkWidgetsMenu *);
    void left(dtkWidgetsMenu *);

#pragma mark - Menu management

public:
    dtkWidgetsMenu *addMenu(dtkWidgetsMenu *);
    dtkWidgetsMenu *addMenu(int, const QString &);

    dtkWidgetsMenu *insertMenu(int, dtkWidgetsMenu *);
    dtkWidgetsMenu *insertMenu(int, int, const QString &);

    void removeMenu(dtkWidgetsMenu *);
    void removeMenu(const QString &);

// /////////////////////////////////////////////////////////////////////////////
#pragma mark - Convenience builders
    // /////////////////////////////////////////////////////////////////////////////

public:
    static QList<dtkWidgetsMenu *> build(const QString &prefix, QMenuBar *bar);

    // /////////////////////////////////////////////////////////////////////////////

#pragma mark - Convenience accessors

public:
    dtkWidgetsMenu *menu(const QString &id);
    dtkCoreParameter *parameter(const QString &id);
    dtkWidgetsParameter *parameterWidget(const QString &id);

public slots:
    void setCurrentIndex(int);
    void setCurrentIndex(int, std::function<void(void)> &);

public:
    int size(void) const;

public:
    QVector<dtkWidgetsMenu *> menus(void) const;

public slots:
    void setFixedHeight(int);

public:
    void setInteractive(bool);
    void setStandalone(bool);

public:
    void setWidth(int);
    void setMargins(int);
    void setOffset(int);

#pragma mark - Dynamism

public slots:
    void touch(void);
    void decr(void);

public:
    void reset(void);

protected:
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected:
    class dtkWidgetsMenuBarPrivate *d = nullptr;

protected:
    friend class dtkWidgetsMenuBarButton;
    friend class dtkWidgetsMenuBarButtonCollapse;
    friend class dtkWidgetsMenuBarContainer;
    friend class dtkWidgetsMenuBarContainerSlide;

private:
    friend QDebug operator<<(QDebug debug, const dtkWidgetsMenuBar &c);
    friend QDebug operator<<(QDebug debug, dtkWidgetsMenuBar *c);
};

QDebug operator<<(QDebug debug, const dtkWidgetsMenuBar &c);
QDebug operator<<(QDebug debug, dtkWidgetsMenuBar *c);
//
// dtkWidgetsMenuBar.h ends here
