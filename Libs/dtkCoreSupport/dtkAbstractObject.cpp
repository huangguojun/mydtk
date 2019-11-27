/* dtkAbstractObject.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Feb 28 17:54:04 2009 (+0100)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkAbstractObject.h"
#include "dtkAbstractObject_p.h"

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// dtkAbstractObject implementation
// /////////////////////////////////////////////////////////////////

//! Constructs an object with parent \a parent
/*!
 *  The parent of an object may be viewed as the object's owner. The
 *  destructor of a parent object destroys all child objects. Setting
 *  parent to 0 constructs an object with no parent.
 *  The initial reference count is set to 0, and DeferredDeletion is enabled.
 */
dtkAbstractObject::dtkAbstractObject(dtkAbstractObject *parent) : QObject(parent), d_ptr(new dtkAbstractObjectPrivate(this))
{
    d_ptr->count = 0;

    d_ptr->isDeferredDeletionEnabled = true;
}

//! Copy constructor.
/*!
 *
 */
dtkAbstractObject::dtkAbstractObject(const dtkAbstractObject& other) : QObject(other.parent()), d_ptr(new dtkAbstractObjectPrivate(*other.d_ptr, this))
{
    d_ptr->count = 0;

    d_ptr->isDeferredDeletionEnabled = true;
}

//! Destroys the object, deleting all its child objects.
/*!
 *  The reference count is checked, and a warning message is output if the reference count is not valid.
 *  There are two valid reference count values at destruction time :<br>
 *  0 : The object used reference counting, and is no longer referred by any object.<br>
 *  1 : This occurs if the object was never reference counted, and delete(Later) was manually called.<br>
 */
dtkAbstractObject::~dtkAbstractObject(void)
{
    if ( d_ptr->count.load() != 0) {
        dtkDebug() << "Warning : deleting object of type " << this->metaObject()->className() << " with non-zero reference count";
    }

    delete d_ptr;

    d_ptr = NULL;
}

//! Returns a new dtkAbstractObject that is a copy of this.
/*!
 *  This method can be overloaded through the hierarchy enabling a
 *  deep copy of this. Note that, using covariance of returned type,
 *  the returned argument can be of the more derived type.
 *
 *  Example:
 *  \code
 *  class xyzObject : public dtkAbstractObject
 *  {
 *    ...
 *    xyzObject *clone(void); // Covariant returned argument
 *    ...
 *  };
 *
 *  xyzObject *xyzObject::clone(void)
 *  {
 *     return new xyzObject(*this);
 *  }
 *  \endcode
 */
dtkAbstractObject *dtkAbstractObject::clone(void)
{
    return new dtkAbstractObject(*this);
}

//! Assignement operator.
/*!
 *
 */
dtkAbstractObject& dtkAbstractObject::operator = (const dtkAbstractObject& other)
{
    this->copy(other);

    return (*this);
}

//! Enables to make a deep copy of the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the assignement operator which delegates
 *  the copy process. When re-implementing this method into a derived
 *  class of dtkAbstractObject, one must called first the copy method
 *  of the parent to ensure that all the attributes are really copied.
 *
 *  Nevertheless, some caution must be taken to avoid slicing problem
 *  as shown in the following example.
 *
 *  Example:
 *  \code
 *  class xyzObject : public dtkAbstractObject
 *  {
 *    ...
 *    void copy(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  void xyzObject::copy(const dtkAbstractObject& other)
 *  {
 *     // copy of the parent attributes
 *     dtkAbstractObject::copy(other);
 *
 *     // copy of the xyzObject attributes
 *     if (other.identifier() == this->identifier()) {
 *        // cast other into xyzObject and do the copy
 *     } else {
 *        dtkWarn() << "other is not of same type than this, slicing is occuring.";
 *     }
 *  }
 *  \endcode
 */
void dtkAbstractObject::copy(const dtkAbstractObject& other)
{
    if (this == &other)
        return;

    this->setParent(other.parent());

    d_ptr->values     = other.d_ptr->values;
    d_ptr->properties = other.d_ptr->properties;
    d_ptr->metadatas  = other.d_ptr->metadatas;

    d_ptr->count = 0;
    d_ptr->isDeferredDeletionEnabled = true;
}

//! Comparison operator.
/*!
 *
 */
bool dtkAbstractObject::operator == (const dtkAbstractObject& other) const
{
    return this->isEqual(other);
}

//! Enables to make a deep comparison between all the attributes through the class
//! hierarchy.
/*!
 *  This method is called by the comparator operator == which
 *  delegates the comparison. When re-implementing this method into a
 *  derived class of dtkAbstractObject, one must called the isEqual
 *  method of the parent to ensure the comparison through all the
 *  attributes.
 *
 *  Example:
 *  \code
 *  class xyzObject : public dtkAbstractObject
 *  {
 *    ...
 *    bool isEqual(const dtkAbstractObject& other);
 *    ...
 *  };
 *
 *  bool xyzObject::isEqual(const dtkAbstractObject& other)
 *  {
 *     // comparison of the parent attributes
 *     if (!dtkAbstractObject::isEqual(other))
 *        return false;
 *
 *     // comparison of the types.
 *     if (this->identifier() != other.identifier())
 *        return false;
 *
 *     // comparison of the xyzObject attributes after casting of other.
 *     ...
 *
 *     // comparison of parent attributes
 *    if (!dtkAbstractObject::isEqual(other))
 *       return false;
 *
 *    return true;
 *  }
 *  \endcode
 */
bool dtkAbstractObject::isEqual(const dtkAbstractObject& other) const
{
    if (this == &other)
        return true;

    if (d_ptr->values != other.d_ptr->values         ||
            d_ptr->properties != other.d_ptr->properties ||
            d_ptr->metadatas != other.d_ptr->metadatas)
        return false;

    return true;
}

QString dtkAbstractObject::description(void) const
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return "";
}

QString dtkAbstractObject::identifier(void) const
{
    //DTK_DEFAULT_IMPLEMENTATION;

    return "";
}

QString dtkAbstractObject::name(void) const
{
    return this->objectName();
}

//! Reference count.
/*!
 *  Returns the current reference count.
 */

int dtkAbstractObject::count(void) const
{
    return d_ptr->count.load();
}

//! Retain reference count.
/*!
 *  This method increments the reference counter once.
 */

int dtkAbstractObject::retain(void) const
{
    return d_ptr->count.fetchAndAddOrdered(1) + 1;
}

//! Release reference count.
/*!
 *  This method decrements the reference counter once. Should the
 *  count be null, the object is scheduled for deletion. Note it sends
 *  the destroyed signal just before being actually deleted.
 *  The method of deletion depends on isDeferredDeletionEnabled().
 *  If it is true (default) the object will be deleted using "this->deleteLater();",
 *  otherwise "delete this;" is used.
 */

int dtkAbstractObject::release(void) const
{
    int newCount = d_ptr->count.fetchAndAddOrdered(-1) - 1;

    if (!(newCount)) {

        if (d_ptr->isDeferredDeletionEnabled)
            const_cast<dtkAbstractObject *>(this)->deleteLater();
        else
            delete this;
    }

    return newCount;
}

/*!
 * \brief Enable / disable use of this->deleteLater() when reference count reaches 0.
 * \param value Changes what happens when the objects reference count reaches 0.
 *  For most objects this should be set to true, as it allows existing events
 *  to be processed which may use this object.
 *  \sa dtkAbstractObject::~dtkAbstractObject();
 *  \sa QObject::deleteLater();
 */
void dtkAbstractObject::enableDeferredDeletion(bool value)
{
    d_ptr->isDeferredDeletionEnabled = value;
}

bool dtkAbstractObject::isDeferredDeletionEnabled(void) const
{
    return d_ptr->isDeferredDeletionEnabled;
}

void dtkAbstractObject::addProperty(const QString& key, const QStringList& values)
{
    d_ptr->values.insert(key, values);
}

void dtkAbstractObject::addProperty(const QString& key, const QString& value)
{
    d_ptr->values.insert(key, QStringList() << value);
}

void dtkAbstractObject::setProperty(const QString& key, const QString& value)
{
    if (!d_ptr->values.contains(key)) {
        dtkDebug() << this->metaObject()->className() << " has no such property:" << key;
        return;
    }

    if (!d_ptr->values.value(key).contains(value)) {
        dtkDebug() << this->metaObject()->className() << " has no such value:" << value << " for key: " << key;
        return;
    }

    d_ptr->properties.insert(key, value);

    onPropertySet(key, value);

    emit propertySet(key, value);
}

QStringList dtkAbstractObject::propertyList(void) const
{
    return d_ptr->properties.keys();
}

QStringList dtkAbstractObject::propertyValues(const QString& key) const
{
    if (d_ptr->values.contains(key))
        return d_ptr->values[key];

    return QStringList();
}

bool dtkAbstractObject::hasProperty(const QString& key) const
{
    return d_ptr->values.contains(key);
}

QString dtkAbstractObject::property(const QString& key) const
{
    if (!d_ptr->values.contains(key)) {
        dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
        return QString();
    }

    return d_ptr->properties.value(key);
}

void dtkAbstractObject::addMetaData(const QString& key, const QStringList& values)
{
    QStringList currentValues = d_ptr->metadatas.value(key);

    d_ptr->metadatas.insert(key, currentValues + values);

    foreach (QString value, values)
        onMetaDataSet(key, value);

    foreach (QString value, values)
        emit metaDataSet(key, value);
}

void dtkAbstractObject::addMetaData(const QString& key, const QString& value)
{
    QStringList currentValues = d_ptr->metadatas.value(key);

    d_ptr->metadatas.insert(key, currentValues << value);

    onMetaDataSet(key, value);

    emit metaDataSet(key, value);
}

void dtkAbstractObject::setMetaData(const QString& key, const QStringList& values)
{
    d_ptr->metadatas.insert(key, values);

    foreach (QString value, values)
        onMetaDataSet(key, value);

    foreach (QString value, values)
        emit metaDataSet(key, value);
}

void dtkAbstractObject::setMetaData(const QString& key, const QString& value)
{
    d_ptr->metadatas.insert(key, QStringList() << value);

    onMetaDataSet(key, value);

    emit metaDataSet(key, value);
}

QStringList dtkAbstractObject::metaDataList(void) const
{
    return d_ptr->metadatas.keys();
}

QStringList dtkAbstractObject::metaDataValues(const QString& key) const
{
    if (d_ptr->metadatas.contains(key))
        return d_ptr->metadatas[key];

    return QStringList();
}

bool dtkAbstractObject::hasMetaData(const QString& key) const
{
    return d_ptr->metadatas.contains(key);
}

void dtkAbstractObject::onPropertySet(const QString& key, const QString& value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}

void dtkAbstractObject::onMetaDataSet(const QString& key, const QString& value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}

QString dtkAbstractObject::metadata(const QString& key) const
{
    if (!d_ptr->metadatas.contains(key)) {
        dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
        return QString();
    }

    return d_ptr->metadatas.value(key).first();
}

QStringList dtkAbstractObject::metadatas(const QString& key) const
{
    if (!d_ptr->metadatas.contains(key)) {
        dtkDebug() << this->metaObject()->className() << "has no such property:" << key;
        return QStringList();
    }

    return d_ptr->metadatas.value(key);
}

void dtkAbstractObject::copyMetaDataFrom(const dtkAbstractObject *obj)
{
    if (!obj)
        return;

    foreach (QString key, obj->metaDataList())
        this->setMetaData(key, obj->metadatas(key));
}

// /////////////////////////////////////////////////////////////////
// dtkAbstractObject documentation
// /////////////////////////////////////////////////////////////////

/*! \class dtkAbstractObject
 *
 *  \brief The dtkAbstractObject is the base class of all dtk abstract
 *  concepts.
 *
 *  The dtkAbstractObject is the root of the data, process and view
 *  taxomonies. It provides two main functionalities: a property
 *  system and a meta data system.
 *
 *  A property is a key that can correspond to a predefined set of
 *  values. It is no possible to assign a value that has not been
 *  declared together with the property. A property has a unique
 *  value.
 *
 *  \code
 *  object->addProperty("key", QStringList() << "value1" << "value2");
 *  object->setProperty("key", "value3");   // Wrong, "value3" has not been assigned to "key".
 *  object->setProperty("key", "value1");   // Right, "value1" has been assigned to "key".
 *  \endcode
 *
 *  The previous code snippet emits the signal propertySet and calls
 *  the virtual method onPropertySet so that one can override its
 *  behaviour when specializing any dtk concept.
 *
 *  A meta data is somehow similar to a property except that there is
 *  no constraint for the values one can assign to it. Also, a meta
 *  data is not unique, one can assign many values to it.
 *
 *  \code
 *  object->addMetaData("key", QStringList() << "value1" << "value2");
 *  object->setMetaData("key", "value3");   // Right, "value3" is now mapped to "key".
 *  object->setMetaData("key", "value4");   // Right, "value4" is now mapped to "key".
 *  object->addMetaData("key", QStringList() << "value1" << "value2");
 *
 *  dtkOutput() << object->metaData("key"); // ("value4", "value1", "value2")
 *  \endcode
 *
 *  The previous code snippet emits the signal metaDataSet and calls
 *  the virtual method onMetaDataSet so that one can override its
 *  behaviour when specializing any dtk concept.
 */
