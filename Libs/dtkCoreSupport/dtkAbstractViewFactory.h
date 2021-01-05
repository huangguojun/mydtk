// Version: $Id: 210b426265ad2b7de1de4c8ec8108ee9109e81ae $
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

#include "dtkAbstractFactory.h"

class dtkAbstractView;
class dtkAbstractViewAnimator;
class dtkAbstractViewNavigator;
class dtkAbstractViewInteractor;
class dtkAbstractViewFactoryPrivate;
template<class T>
class dtkSmartPointer;

class DTKCORESUPPORT_EXPORT dtkAbstractViewFactory : public dtkAbstractFactory
{
    Q_OBJECT

public:
    typedef dtkAbstractView *(*dtkAbstractViewCreator)(void);
    typedef dtkAbstractViewAnimator *(*dtkAbstractViewAnimatorCreator)(void);
    typedef dtkAbstractViewNavigator *(*dtkAbstractViewNavigatorCreator)(void);
    typedef dtkAbstractViewInteractor *(*dtkAbstractViewInteractorCreator)(void);

    typedef QPair<QString, QStringList> dtkAbstractViewTypeHandler;

public:
    static dtkAbstractViewFactory *instance(void);

    bool registerViewType(const QString &type, dtkAbstractViewCreator func);
    bool registerViewType(const QString &type, dtkAbstractViewCreator func,
                          const QString &interface_name);
    bool registerViewAnimatorType(const QString &type, const QStringList &handled,
                                  dtkAbstractViewAnimatorCreator func);
    bool registerViewNavigatorType(const QString &type, const QStringList &handled,
                                   dtkAbstractViewNavigatorCreator func);
    bool registerViewInteractorType(const QString &type, const QStringList &handled,
                                    dtkAbstractViewInteractorCreator func);

    unsigned int size(const QString &type) const;

    QStringList viewNames(void) const;

    QList<dtkAbstractView *> views(void);

    dtkAbstractView *view(const QString &name);

    QList<QString> creators(void) const;
    QList<dtkAbstractViewTypeHandler> animators(void) const;
    QList<dtkAbstractViewTypeHandler> interactors(void) const;
    QList<dtkAbstractViewTypeHandler> navigators(void) const;

    dtkSmartPointer<dtkAbstractView> createSmartPointer(const QString &type);
    dtkSmartPointer<dtkAbstractViewAnimator> animatorSmartPointer(const QString &type);
    dtkSmartPointer<dtkAbstractViewNavigator> navigatorSmartPointer(const QString &type);
    dtkSmartPointer<dtkAbstractViewInteractor> interactorSmartPointer(const QString &type);

public:
    void destroy(dtkAbstractView *view);

public:
    QStringList implementations(void);
    QStringList implementations(const QString &interface_name);
    QStringList interfaces(void);

signals:
    void created(dtkAbstractView *view, const QString &type);
    void cleared(void);

public slots:
    dtkAbstractView *create(const QString &type);
    dtkAbstractViewAnimator *animator(const QString &type);
    dtkAbstractViewNavigator *navigator(const QString &type);
    dtkAbstractViewInteractor *interactor(const QString &type);

public slots:
    void clear(void);

protected:
    dtkAbstractViewFactory(void);
    ~dtkAbstractViewFactory(void);

private:
    static dtkAbstractViewFactory *s_instance;

private:
    dtkAbstractViewFactoryPrivate *d;
};

//
// dtkAbstractViewFactory.h ends here
