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

class dtkCoreParameter;

class dtkWidgetsMenuItem;
class dtkWidgetsMenuFacade;
class dtkWidgetsMenuHeaderFacade;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace dtkWidgetsMenuHelper {

DTKWIDGETS_EXPORT QWidget *build(QObject *source);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsMenu : public QObject
{
    Q_OBJECT

public:
    dtkWidgetsMenu(int, const QString &, QObject *parent = nullptr);
    ~dtkWidgetsMenu(void);

#pragma mark - Menu management

public:
    dtkWidgetsMenu *addMenu(dtkWidgetsMenu *menu);
    dtkWidgetsMenu *addMenu(int, const QString &title);

    dtkWidgetsMenuItem *addItem(dtkWidgetsMenuItem *item);
    dtkWidgetsMenuItem *addItem(int, const QString &title);

    dtkWidgetsMenu *insertMenu(int, dtkWidgetsMenu *menu);
    dtkWidgetsMenu *insertMenu(int, int, const QString &title);

    dtkWidgetsMenuItem *insertItem(int, dtkWidgetsMenuItem *item);
    dtkWidgetsMenuItem *insertItem(int, int, const QString &title);

    dtkWidgetsMenuItem *addSeparator(void);

    dtkWidgetsMenuItem *addParameter(dtkWidgetsMenuItem *item);
    dtkWidgetsMenuItem *addParameter(const QString &, dtkCoreParameter *,
                                     const QString &representation);

    void removeItem(dtkWidgetsMenuItem *item);
    void removeMenu(dtkWidgetsMenu *menu);

#pragma mark -

public slots:
    void clear(void);

public:
    QString title(void) const;

    QVector<dtkWidgetsMenu *> menus(void) const;

    dtkWidgetsMenu *menu(const QString &) const;

    QObject *object(const QString &);

    QVector<QObject *> objects(void) const;

public:
    int icon(void) const;

protected:
    class dtkWidgetsMenuPrivate *d;
};

//
// dtkWidgetsMenu.h ends here
