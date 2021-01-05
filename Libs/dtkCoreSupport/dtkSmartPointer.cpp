/* dtkSmartPointer.cpp ---
 *
 * Author: John Stark
 * Copyright (C) 2011 - John Stark, Inria.
 * Created: Wed May 25 17:00:57 2011 (+0100)
 * Version: $Id: ac2d906d03b1c1b48c2be818e62e0963f8f774cd $
 * Last-Updated: Tue Apr 24 19:54:19 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 2
 */

/* Commentary:
 * This file only provides documentation - the implementation of the class
 * is in the header.
 */

#include "dtkSmartPointer.h"

/* Change log:
 *
 */

/*! \class      dtkSmartPointer
 *  \brief      A smart pointer for reference counting dtk objects.
 *
 * A smart pointer for automatic reference counting of anything
 * derived from dtkAbstractObject.The class can also be used with any class
 * which implements retain() and release().
 *
 * This class automatically increases the reference count of pointers assigned
 * to it, and decreases the reference count when it goes out of scope.
 *
 * Warning : dtkAbstractObject currently uses this->deleteLater() to delete
 * itself when the reference count reaches zero. This means the object will
 * not be deleted until an event loop is reached.
 *
 * Code example :
 * \code
 *  dtkSmartPointer< dtkAbstractData > myInstance;
 *  myInstance = factory->createSmartPointer( "MyDataTypeName" );
 * \endcode
 * Here, the dtkSmartPointer takes ownership of the object created by the
 factory. It will be
 * deleted when the dtkSmartPointer goes out of scope, unless another object
 takes it.
 *
 * It is also safe to use dtkSmartPointer in containers from Qt :
 * \code
 *  typedef QHash<QString, dtkSmartPointer< dtkAbstractData > > DataContainer;
 *  DataContainer myHashMapContainer;
 *  myHashMapContainer["bar"] = myInstance;
 *  myHashMapContainer["foo"] = myOtherInstance;
 *  myHashMapContainer["gee"] = myOtherInstance2;
 *  myHashMapContainer.clear();
 * \endcode
 *
 * And STL containers :
 * \code
 * typedef std::vector< dtkSmartPointer< dtkAbstractData > > StlContainer;
 * StlContainer myVector;
 * \endcode

 */

/*! \fn         dtkSmartPointer::ObjectType;
    \brief      Access to the managed type, typedef for T.
*/

/*! \fn         dtkSmartPointer::dtkSmartPointer()
    \brief      Default constructor. Managed pointer is initialized to NULL.
*/

/*! \fn         dtkSmartPointer::dtkSmartPointer( const dtkSmartPointer<T>& p)
    \brief      Copy constructor. Managed pointer is initialized with object
                from p. The reference count of p is increased by 1.
    \param      p : SmartPointer that is copied.
*/

/*! \fn        template < TR > dtkSmartPointer< T >::dtkSmartPointer(const
   dtkSmartPointer< TR > &p) \brief      Constructor from SmartPointer of other
   type. Managed pointer is initialized with object from p using dynamic_cast.
                The reference count of p is increased by 1 if the cast succeeds.
    \param      p : SmartPointer that is copied.
*/

/*! \fn         dtkSmartPointer::dtkSmartPointer( T *p )
    \brief      Construct from raw pointer. Managed pointer is initialized with
                p. The reference count of p is increased by 1.
    \param      p : Pointer used to initialize this.
*/

/*! \fn         dtkSmartPointer::~dtkSmartPointer()
    \brief      Destructor.
                The reference count of p is decreased by 1, which may lead to
                its (eventual) deletion.
*/

/*! \fn         T* dtkSmartPointer::operator->() const
    \brief      Overload the -> operator. Allows dtkSmartPointer to be used
                with same syntax as a normal pointer.
    \return     A pointer to the managed object
*/

/*! \fn         T& dtkSmartPointer::operator T*() const
    \brief      Overload the T* operator. Allows dtkSmartPointer to be used
                with same syntax as a normal pointer in some cases.
    \return     A reference to the managed object
*/

/*! \fn         bool dtkSmartPointer::isNull() const
    \brief      Test if the underlying pointer is NULL.
    \return     True if the underlying pointer is NULL.
*/

/*! \fn         int dtkSmartPointer::refCount(void) const
    \brief      Get the reference count of the pointer.
    \return     The count for the pointer, or 0 if it is NULL.
    \sa         int dtkAbstractObject::count() const
*/

/*! \fn         bool dtkSmartPointer< T >::operator==(TR r) const
    \fn         bool dtkSmartPointer< T >::operator==( const dtkSmartPointer< TR
   > &r ) const \fn         bool dtkSmartPointer< T >::operator==(const
   dtkSmartPointer &r) const \brief      Compare with another pointer.
                Explicitly templating for dtkSmartPointer<TR> means that no
   temporary needs to be created - and hence no additional retain / release.
    \return     True if static_cast< T* >(r) == d.
*/

/*! \fn         bool dtkSmartPointer< T >::operator!=(TR r) const
    \fn         bool dtkSmartPointer< T >::operator!=( const dtkSmartPointer< TR
   > &r ) const \fn         bool dtkSmartPointer< T >::operator!=(const
   dtkSmartPointer &r) const Explicitly templating for dtkSmartPointer<TR> means
   that no temporary needs to be created - and hence no additional retain /
   release. \brief      Compare with another pointer. \return     True if
   static_cast< T* >(r) != d.
*/

/*! \fn         bool dtkSmartPointer< T >::operator<(const dtkSmartPointer &r)
   const \fn         bool dtkSmartPointer< T >::operator<=(const dtkSmartPointer
   &r) const \fn         bool dtkSmartPointer< T >::operator>(const
   dtkSmartPointer &r) const \fn         bool dtkSmartPointer< T
   >::operator>=(const dtkSmartPointer &r) const \brief      Compare with
   another pointer. \return     Result of pointer comparison.
*/

/*! \fn         T* dtkSmartPointer::data()
    \brief      Return pointer.
    \return     The pointer (our d)
*/

/*! \fn         T* dtkSmartPointer::constData() const
    \brief      Return const pointer.
    \return     The pointer (our d)
*/

/*! \fn         T& dtkSmartPointer< T >::operator*()
    \fn         const T& dtkSmartPointer< T >::operator*() const
    \brief      Return object as a reference. (const and non-const versions
                available)
    \return     The managed pointer (our d)
*/

/*! \fn         dtkSmartPointer& dtkSmartPointer< T >::operator=(const
   dtkSmartPointer& r) \fn         dtkSmartPointer& dtkSmartPointer< T
   >::operator=( T *r ) \brief      Overload operator assignment. Decrease
   reference count of existing pointer if not NULL. Assigns value from parameter
   given. Increases reference count of assigned pointer if not NULL. \return
   *this
*/

/*! \fn         template<TR> dtkSmartPointer& dtkSmartPointer< T
   >::operator=(const dtkSmartPointer<TR>& r) \brief      Overload operator
   assignment with dtkSmartPointer of other type. Managed pointer is assigned
   using dynamic_cast of pointer in r. The reference count of p is increased by
   1 if the cast succeeds. \param      r : SmartPointer that is copied. \return
   *this.
*/

/*! \fn         void dtkSmartPointer::swap( dtkSmartPointer &other)
    \brief      Swap this instance's shared data pointer with the shared data
                pointer in other. Reference counts are unchanged.
    \param      other : dtkSmartPointer to swap with.
*/

/*! \fn         dtkSmartPointer& dtkSmartPointer::takePointer(T *r)
    \brief      Assign without incrementing reference count of assigned object.
    \param      r object to take, can be NULL.
    \return     *this
*/

/*! \fn         T* dtkSmartPointer::releasePointer()
    \brief      Release the pointer without decreasing the reference count.
                The internal pointer (d) is set to NULL;
    \return     The contents of d before it was set to NULL.
*/

/*! \fn         void dtkSmartPointer::retain()
    \brief      Private function that increases reference count by calling
                d->retain() if d is not NULL.
*/

/*! \fn         void dtkSmartPointer::release()
    \brief      Private function that decreases reference count by calling
                d->release() if d is not NULL.
*/

/*! \fn         uint qHash(const dtkSmartPointer<T> &key)
    \brief      Template which allows dtkSmartPointer to be used in Qt's hash
                based containers.
*/
