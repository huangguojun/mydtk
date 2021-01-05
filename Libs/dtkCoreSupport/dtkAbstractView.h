/* dtkAbstractView.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 16:00:26 2008 (+0100)
 * Version: $Id: 6f843e298dc41472567a7fbf2558604785e9c0ac $
 * Last-Updated: jeu. mars 26 11:33:52 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 344
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKABSTRACTVIEW_H
#define DTKABSTRACTVIEW_H

#include "dtkAbstractObject.h"

#include <QtWidgets/QWidget>

class dtkAbstractViewPrivate;
class dtkAbstractViewAnimator;
class dtkAbstractViewNavigator;
class dtkAbstractViewInteractor;
class dtkAbstractData;

#include <dtkMathSupport/dtkQuaternion.h>
#include <dtkMathSupport/dtkVector3D.h>

class DTKCORESUPPORT_EXPORT dtkAbstractView : public dtkAbstractObject
{
    Q_OBJECT

public:
    dtkAbstractView(dtkAbstractView *parent = 0);
    dtkAbstractView(const dtkAbstractView &other);
    virtual ~dtkAbstractView(void);

public:
    virtual dtkAbstractView *clone(void);

public:
    dtkAbstractView &operator=(const dtkAbstractView &other);

protected:
    virtual void copy(const dtkAbstractObject &other);

public:
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractView &viewer);
    friend DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, dtkAbstractView *viewer);

signals:
    void closed(void);
    void focused(void);
    void nameChanged(void);

public slots:
    virtual void link(dtkAbstractView *other);
    virtual void unlink(dtkAbstractView *other);

public slots:
    virtual void select(dtkAbstractData *data);
    virtual void unselect(dtkAbstractData *data);

public slots:
    virtual void setStereo(bool on);

public slots:
    virtual void setView(void *view);

public slots:
    virtual void setData(dtkAbstractData *data);
    virtual void setData(dtkAbstractData *data, int inputId);

public slots:
    virtual void setBackgroundColor(int red, int green, int blue);
    virtual void setBackgroundColor(double red, double green, double blue);

public slots:
    virtual void *view(void);
    virtual void *data(void);
    virtual void *data(int channel);

public slots:
    virtual bool stereo(void);

public slots:
    virtual void clear(void);
    virtual void reset(void);
    virtual void update(void);

public slots:
    virtual QWidget *widget(void);

public slots:
    virtual void close(void);

public slots:
    void showFullScreen(void);
    void showMinimized(void);
    void showMaximized(void);
    void showNormal(void);
    void show(void);

public slots:
    void resize(int width, int height);

public slots:
    void addAnimator(dtkAbstractViewAnimator *animator);
    void addNavigator(dtkAbstractViewNavigator *navigator);
    void addInteractor(dtkAbstractViewInteractor *interactor);

public slots:
    void enableAnimator(const QString &animator);
    void disableAnimator(const QString &animator);
    void enableNavigator(const QString &navigator);
    void disableNavigator(const QString &navigator);
    void enableInteractor(const QString &interactor);
    void disableInteractor(const QString &interactor);

public slots:
    dtkAbstractViewAnimator *animator(const QString &type);
    dtkAbstractViewNavigator *navigator(const QString &type);
    dtkAbstractViewInteractor *interactor(const QString &type);

public slots:
    QList<dtkAbstractViewAnimator *> animators(void) const;
    QList<dtkAbstractViewNavigator *> navigators(void) const;
    QList<dtkAbstractViewInteractor *> interactors(void) const;

public slots:
    virtual void initialize(void);
    virtual void uninitialize(void);

public:
    QString name(void) const;
    void setName(const QString &name);

    // /////////////////////////////////////////////////////////////////
    // New VR API
    // /////////////////////////////////////////////////////////////////

public:
    virtual void setHeadPosition(dtkVector3DReal position);
    virtual void setHeadOrientation(dtkQuaternionReal orientation);

public:
    virtual void setUpperLeft(dtkVector3DReal position);
    virtual void setLowerLeft(dtkVector3DReal position);
    virtual void setLowerRight(dtkVector3DReal position);

    // /////////////////////////////////////////////////////////////////

private:
    DTK_DECLARE_PRIVATE(dtkAbstractView);
};

DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, const dtkAbstractView &viewer);
DTKCORESUPPORT_EXPORT QDebug operator<<(QDebug debug, dtkAbstractView *viewer);

#endif
