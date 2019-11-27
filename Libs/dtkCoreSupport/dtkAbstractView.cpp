/* dtkAbstractView.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Nov  7 16:01:09 2008 (+0100)
 * Version: $Id: 2be54d33dba23b57e928752ad87e2baaed4de4eb $
 * Last-Updated: jeu. mars 26 11:34:11 2015 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 356
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractData.h"
#include "dtkAbstractView.h"
#include "dtkAbstractView_p.h"
#include "dtkAbstractViewAnimator.h"
#include "dtkAbstractViewNavigator.h"
#include "dtkAbstractViewInteractor.h"

// #include <dtkMath/dtkVector3D.h>
// #include <dtkMath/dtkQuaternion.h>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkAbstractView implementation
// /////////////////////////////////////////////////////////////////

dtkAbstractView::dtkAbstractView(dtkAbstractView *parent) : dtkAbstractObject(*new dtkAbstractViewPrivate, parent)
{
    DTK_D(dtkAbstractView);

    d->stereo = false;
}

dtkAbstractView::dtkAbstractView(const dtkAbstractView& other) : dtkAbstractObject(*new dtkAbstractViewPrivate(*other.d_func()), other)
{

}

dtkAbstractView::~dtkAbstractView(void)
{

}

//! Returns a new dtkAbstractView that is a copy of this.
/*!
 *  This method can be overloaded through the hierarchy enabling a
 *  deep copy of this. Note that, using covariance of returned type,
 *  the returned argument can be of the more derived type.
 *
 *  Example:
 *  \code
 *  class xyzView : public dtkAbstractView
 *  {
 *    ...
 *    xyzView *clone(void); // Covariant returned argument
 *    ...
 *  };
 *
 *  xyzView *xyzView::clone(void)
 *  {
 *     return new xyzView(*this);
 *  }
 *  \endcode
 */
dtkAbstractView *dtkAbstractView::clone(void)
{
    return new dtkAbstractView(*this);
}


dtkAbstractView& dtkAbstractView::operator=(const dtkAbstractView& other)
{
    this->copy(other);

    return (*this);
}

//! Enables to make a deep copy of the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the assignement operator which delegates
 *  the copy process. When re-implementing this method into a derived
 *  class of dtkAbstractView, one must called first the copy method
 *  of the parent to ensure that all the attributes are really copied.
 *
 *  Nevertheless, some caution must be taken to avoid slicing problem
 *  as shown in the following example.
 *
 *  Example:
 *  \code
 *  class xyzView : public dtkAbstractView
 *  {
 *    ...
 *    void copy(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  void xyzView::copy(const dtkAbstractObject& other)
 *  {
 *     // copy of the parent attributes
 *     dtkAbstractView::copy(other);
 *
 *     // copy of the xyzView attributes
 *     if (other.identifier() == this->identifier()) {
 *        // cast other into xyzView and do the copy
 *     } else {
 *        dtkWarn() << "other is not of same type than this, slicing is occuring.";
 *     }
 *  }
 *  \endcode
 */
void dtkAbstractView::copy(const dtkAbstractObject& other)
{
    dtkAbstractObject::copy(other);

    if (this->identifier() == other.identifier()) {

        const dtkAbstractView& view = reinterpret_cast<const dtkAbstractView&>(other);

        DTK_D(dtkAbstractView);

        d->animators   = view.d_func()->animators;
        d->navigators  = view.d_func()->navigators;
        d->interactors = view.d_func()->interactors;

    } else {
        dtkWarn() << "Other is not of same type than this, slicing is occuring.";
    }
}

void dtkAbstractView::initialize(void)
{

}

void dtkAbstractView::uninitialize(void)
{

}

QString dtkAbstractView::name(void) const
{
    DTK_D(const dtkAbstractView);

    return d->name;
}

void dtkAbstractView::setName(const QString& name)
{
    DTK_D(dtkAbstractView);

    d->name = name;

    emit nameChanged();
}

void dtkAbstractView::link(dtkAbstractView *other)
{
    Q_UNUSED(other);
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::unlink(dtkAbstractView *other)
{
    Q_UNUSED(other);
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::select(dtkAbstractData *data)
{
    Q_UNUSED(data);
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::unselect(dtkAbstractData *data)
{
    Q_UNUSED(data);
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setStereo(bool on)
{
    DTK_D(dtkAbstractView);

    d->stereo = on;
}

void dtkAbstractView::setView(void *)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setData(dtkAbstractData *data)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    DTK_D(dtkAbstractView);

    foreach (dtkAbstractViewAnimator *animator, d->animators)
        if (animator->enabled())
            animator->setData(data);

    foreach (dtkAbstractViewInteractor *interactor, d->interactors)
        if (interactor->enabled())
            interactor->setData(data);
}

void dtkAbstractView::setData(dtkAbstractData *data, int channel)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(data);
    Q_UNUSED(channel);
}

void dtkAbstractView::setBackgroundColor(int red, int green, int blue)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
}

void dtkAbstractView::setBackgroundColor(double red, double green, double blue)
{
    //DTK_DEFAULT_IMPLEMENTATION;
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
}

void *dtkAbstractView::view(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *dtkAbstractView::data(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *dtkAbstractView::data(int channel)
{
    Q_UNUSED(channel);
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

bool dtkAbstractView::stereo(void)
{
    DTK_D(dtkAbstractView);

    return d->stereo;
}

void dtkAbstractView::clear(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::reset(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::update(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;
}

QWidget *dtkAbstractView::widget(void)
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void dtkAbstractView::close(void)
{
    emit closed();
}

void dtkAbstractView::showFullScreen(void)
{
    if (QWidget *widget = this->widget())
        widget->showFullScreen();
}

void dtkAbstractView::showMinimized(void)
{
    if (QWidget *widget = this->widget())
        widget->showMinimized();
}

void dtkAbstractView::showMaximized(void)
{
    if (QWidget *widget = this->widget())
        widget->showMaximized();
}

void dtkAbstractView::showNormal(void)
{
    if (QWidget *widget = this->widget())
        widget->showNormal();
}

void dtkAbstractView::show(void)
{
    if (QWidget *widget = this->widget())
        widget->show();
}

void dtkAbstractView::resize(int width, int height)
{
    if (QWidget *widget = this->widget())
        widget->resize(width, height);
}

void dtkAbstractView::addAnimator(dtkAbstractViewAnimator *animator)
{
    if (animator->identifier().isEmpty()) {
        dtkDebug() << "No identifier specified for animator. Not add to" << this->identifier();
        return;
    }

    DTK_D(dtkAbstractView);

    d->animators.insert(animator->identifier(), animator);
}

void dtkAbstractView::addNavigator(dtkAbstractViewNavigator *navigator)
{
    if (navigator->identifier().isEmpty()) {
        dtkDebug() << "No identifier specified for navigator. Not add to" << this->identifier();
        return;
    }

    DTK_D(dtkAbstractView);

    d->navigators.insert(navigator->identifier(), navigator);
}

void dtkAbstractView::addInteractor(dtkAbstractViewInteractor *interactor)
{
    if (interactor->identifier().isEmpty()) {
        dtkDebug() << "No identifier specified for interactor. Not add to" << this->identifier();
        return;
    }

    if (interactor) {
        DTK_D(dtkAbstractView);

        d->interactors.insert(interactor->identifier(), interactor);
    }
}

void dtkAbstractView::enableAnimator(const QString& animator)
{
    DTK_D(dtkAbstractView);

    if (d->animators.contains(animator)) {
        d->animators.value(animator)->setView(this);
        d->animators.value(animator)->enable();
    } else
        dtkDebug() << identifier() << " has no such animator: " << animator;
}

void dtkAbstractView::disableAnimator(const QString& animator)
{
    DTK_D(dtkAbstractView);

    if (d->animators.contains(animator))
        d->animators.value(animator)->disable();
}

void dtkAbstractView::enableNavigator(const QString& navigator)
{
    DTK_D(dtkAbstractView);

    if (d->navigators.contains(navigator)) {
//      d->navigators.value(navigator)->setView(this);
        d->navigators.value(navigator)->enable();
    } else
        dtkDebug() << identifier() << " has no such navigator: " << navigator;
}

void dtkAbstractView::disableNavigator(const QString& navigator)
{
    DTK_D(dtkAbstractView);

    if (d->navigators.contains(navigator))
        d->navigators.value(navigator)->disable();
}

void dtkAbstractView::enableInteractor(const QString& interactor)
{
    DTK_D(dtkAbstractView);

    if (d->interactors.contains(interactor)) {
        d->interactors.value(interactor)->setView(this);
        d->interactors.value(interactor)->enable();
    } else
        dtkDebug() << identifier() << " has no such interactor: " << interactor;
}

void dtkAbstractView::disableInteractor(const QString& interactor)
{
    DTK_D(dtkAbstractView);

    if (d->interactors.contains(interactor))
        d->interactors.value(interactor)->disable();
}

dtkAbstractViewAnimator *dtkAbstractView::animator(const QString& type)
{
    DTK_D(dtkAbstractView);

    if (d->animators.contains(type))
        return d->animators.value(type);

    return NULL;
}

dtkAbstractViewNavigator *dtkAbstractView::navigator(const QString& type)
{
    DTK_D(dtkAbstractView);

    if (d->navigators.contains(type))
        return d->navigators.value(type);

    return NULL;
}

dtkAbstractViewInteractor *dtkAbstractView::interactor(const QString& type)
{
    DTK_D(dtkAbstractView);

    if (d->interactors.contains(type))
        return d->interactors.value(type);

    return NULL;
}

QList<dtkAbstractViewAnimator *> dtkAbstractView::animators(void) const
{
    DTK_D(const dtkAbstractView);

    QList<dtkAbstractViewAnimator *> ret;
#if QT_VERSION > 0x0406FF
    ret.reserve(d->animators.size());
#endif

    foreach ( dtkSmartPointer<dtkAbstractViewAnimator> value, d->animators )
        ret.push_back(value.data());

    return ret;
}

QList<dtkAbstractViewNavigator *> dtkAbstractView::navigators(void) const
{
    DTK_D(const dtkAbstractView);

    QList<dtkAbstractViewNavigator *> ret;
#if QT_VERSION > 0x0406FF
    ret.reserve(d->navigators.size());
#endif

    foreach ( dtkSmartPointer<dtkAbstractViewNavigator> value, d->navigators )
        ret.push_back(value.data());

    return ret;
}

QList<dtkAbstractViewInteractor *> dtkAbstractView::interactors(void) const
{
    DTK_D(const dtkAbstractView);

    QList<dtkAbstractViewInteractor *> ret;
#if QT_VERSION > 0x0406FF
    ret.reserve(d->interactors.size());
#endif

    foreach ( dtkSmartPointer<dtkAbstractViewInteractor> value, d->interactors )
        ret.push_back(value.data());

    return ret;
}

void dtkAbstractView::setHeadPosition(dtkVector3DReal position)
{
    Q_UNUSED(position);

    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setHeadOrientation(dtkQuaternionReal orientation)
{
    Q_UNUSED(orientation);

    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setUpperLeft(dtkVector3DReal position)
{
    Q_UNUSED(position);

    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setLowerLeft(dtkVector3DReal position)
{
    Q_UNUSED(position);

    //DTK_DEFAULT_IMPLEMENTATION;
}

void dtkAbstractView::setLowerRight(dtkVector3DReal position)
{
    Q_UNUSED(position);

    //DTK_DEFAULT_IMPLEMENTATION;
}

// /////////////////////////////////////////////////////////////////
// Debug stream operators
// /////////////////////////////////////////////////////////////////

QDebug operator<<(QDebug debug, const dtkAbstractView& viewer)
{
    debug.nospace() << viewer.identifier();

    return debug.space();
}

QDebug operator<<(QDebug debug, dtkAbstractView *viewer)
{
    debug.nospace() << viewer->identifier();

    return debug.space();
}
