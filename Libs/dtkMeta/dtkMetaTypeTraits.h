// Version: $Id: 2ee46b85600638b2781e87569558e5bd9bd2c774 $
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

#include <type_traits>
#include <iterator>
#include <complex>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsCopyable
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsClonable : std::integral_constant < bool, (!std::is_abstract<T>::value&&
        std::is_copy_constructible<T>::value) > {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsNumeric definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsNumeric : std::integral_constant<bool, std::is_arithmetic<T>::value> {};

// Specialization for complex types

template< typename T > struct dtkMetaTypeIsNumeric< std::complex<T> > : std::integral_constant< bool, std::is_arithmetic<T>::value >::value {};
template< typename T > struct dtkMetaTypeIsNumeric< const std::complex<T> > : std::integral_constant< bool, std::is_arithmetic<T>::value >::value {};
template< typename T > struct dtkMetaTypeIsNumeric< volatile std::complex<T> > : std::integral_constant< bool, std::is_arithmetic<T>::value >::value {};
template< typename T > struct dtkMetaTypeIsNumeric< const volatile std::complex<T> > : std::integral_constant< bool, std::is_arithmetic<T>::value >::value {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsAssignable definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsAssignable : std::integral_constant < bool, dtkMetaTypeIsNumeric<T>::value || std::is_copy_assignable<T>::value > {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsAddAssignable definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsAddAssignable : std::integral_constant<bool, dtkMetaTypeIsNumeric<T>::value> {};

template<> struct dtkMetaTypeIsAddAssignable<QString> : std::true_type {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsSubAssignable definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsSubAssignable : std::integral_constant<bool, dtkMetaTypeIsNumeric<T>::value> {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsMulAssignable definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsMulAssignable : std::integral_constant<bool, dtkMetaTypeIsNumeric<T>::value> {};

// /////////////////////////////////////////////////////////////////
// dtkMetaTypeIsDivAssignable definition
// /////////////////////////////////////////////////////////////////

template< typename T > struct dtkMetaTypeIsDivAssignable : std::integral_constant<bool, dtkMetaTypeIsNumeric<T>::value> {};

// /////////////////////////////////////////////////////////////////
// Container traits
// /////////////////////////////////////////////////////////////////

// Container kind trait

template<typename T> struct dtkMetaTypeIsSequentialContainerPointer : std::false_type {};
template<typename T> struct dtkMetaTypeIsAssociativeContainerPointer : std::false_type {};

// Resizable trait

template< typename T > struct dtkMetaContainerIsResizable : std::false_type {};
template< typename T > struct dtkMetaContainerIsResizable< QVector<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsResizable< QVarLengthArray<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsResizable< QStack<T> > : std::true_type {};
template< typename T, class Allocator > struct dtkMetaContainerIsResizable< std::vector<T, Allocator> > : std::true_type {};
template< typename T, class Allocator > struct dtkMetaContainerIsResizable< std::list<T, Allocator> > : std::true_type {};

// Reservable trait

template< typename T > struct dtkMetaContainerIsReservable : std::false_type {};
template< typename T > struct dtkMetaContainerIsReservable< QVector<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsReservable< QStack<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsReservable< QVarLengthArray<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsReservable< QList<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsReservable< QQueue<T> > : std::true_type {};
template< typename T > struct dtkMetaContainerIsReservable< QSet<T> > : std::true_type {};
template< typename T, class Allocator > struct dtkMetaContainerIsReservable< std::vector<T, Allocator> > : std::true_type {};

// /////////////////////////////////////////////////////////////////
// Iterator traits
// /////////////////////////////////////////////////////////////////

template < typename Iterator, typename Category = typename std::iterator_traits<Iterator>::iterator_category > struct dtkMetaIteratorIsRandomAccess : std::false_type {};
template < typename Iterator > struct dtkMetaIteratorIsRandomAccess<Iterator, std::random_access_iterator_tag> : std::true_type {};

template< typename Iterator, typename Category = typename std::iterator_traits<Iterator>::iterator_category > struct dtkMetaIteratorIsBidirectional : dtkMetaIteratorIsRandomAccess<Iterator, Category> {};
template< typename Iterator > struct dtkMetaIteratorIsBidirectional<Iterator, std::bidirectional_iterator_tag> : std::true_type {};


//
// dtkMetaTypeTraits.h ends here
