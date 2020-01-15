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

#include <type_traits>
#include <iterator>
#include <complex>
#include <vector>
#include <list>
#include <memory>

#include <QtCore>

namespace dtk {


    // is_qobject : detects whether a class inherits from QObject or not
    namespace detail
    {
        template <typename T>
        struct is_qobject_impl : std::is_base_of<QObject, std::remove_pointer_t<std::decay_t<T>>>
        {
        };
    }

    template <typename T>
    using is_qobject = detail::is_qobject_impl<T>;


    // is_instantiable : detects whether an object of the class can be instantiated or not
    namespace detail
    {
        template <typename T>
        struct is_instantiable_impl : std::conditional_t<!std::is_abstract<std::remove_pointer_t<std::decay_t<T>>>::value, std::is_constructible<std::remove_pointer_t<std::decay_t<T>>>, std::false_type>
        {
        };
    }

    template <typename T>
    using is_instantiable = detail::is_instantiable_impl<T>;


    // is_clonable : detects whether an object of the class is clonable or not
    namespace detail
    {
        template <typename T>
        struct is_clonable_impl : std::conditional_t<dtk::is_instantiable<T>::value, std::is_copy_constructible<std::remove_pointer_t<std::decay_t<T>>>, std::false_type>
        {
        };
    }

    template <typename T>
    using is_clonable = detail::is_clonable_impl<T>;


    // is_copyable : detects whether an object of the class can be copied or not
    namespace detail
    {
        template <typename T>
        struct is_copyable_impl : std::is_copy_assignable<std::remove_pointer_t<std::decay_t<T>>>
        {
        };
    }

    template <typename T>
    using is_copyable = detail::is_copyable_impl<T>;


    // is_numeric : detects whether the arithmetic operations +, -, *, / can be applied or not
    namespace detail
    {
        template <typename T>
        struct is_numeric_impl : std::is_arithmetic<T>
        {
        };

        template <typename T>
        struct is_numeric_impl<std::complex<T>> : std::is_arithmetic<T>
        {
        };
    }

    template <typename T>
    using is_numeric = detail::is_numeric_impl<std::remove_cv_t<T>>;


    // is_assignable : detects whether the type supports assignment operator = or not
    namespace detail
    {
        template <typename T>
        struct is_assignable_impl : std::conditional_t<dtk::is_numeric<T>::value, std::true_type, std::is_copy_assignable<T>>
        {
        };
    }

    template <typename T>
    using is_assignable = detail::is_assignable_impl<T>;

    template <class T, class R = void>
    using enable_assignment = typename std::enable_if<is_assignable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_assignment = typename std::enable_if<!is_assignable<T>::value, R>::type;


    // is_add_assignable : detects whether the type supports operator += or not
    namespace detail
    {
        template <typename T>
        struct is_add_assignable_impl : dtk::is_numeric<T>
        {
        };

        template <>
        struct is_add_assignable_impl<QString> : std::true_type
        {
        };
    }

    template <typename T>
    using is_add_assignable = detail::is_add_assignable_impl<T>;

    template <class T, class R = void>
    using enable_add_assignment = typename std::enable_if<is_add_assignable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_add_assignment = typename std::enable_if<!is_add_assignable<T>::value, R>::type;


    // is_sub_assignable : detects whether the type supports operator -= or not
    namespace detail
    {
        template <typename T>
        struct is_sub_assignable_impl : dtk::is_numeric<T>
        {
        };
    }

    template <typename T>
    using is_sub_assignable = detail::is_sub_assignable_impl<T>;

    template <class T, class R = void>
    using enable_sub_assignment = typename std::enable_if<is_sub_assignable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_sub_assignment = typename std::enable_if<!is_sub_assignable<T>::value, R>::type;


    // is_mul_assignable : detects whether the type supports operator *= or not
    namespace detail
    {
        template <typename T>
        struct is_mul_assignable_impl : dtk::is_numeric<T>
        {
        };
    }

    template <typename T>
    using is_mul_assignable = detail::is_mul_assignable_impl<T>;

    template <class T, class R = void>
    using enable_mul_assignment = typename std::enable_if<is_mul_assignable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_mul_assignment = typename std::enable_if<!is_mul_assignable<T>::value, R>::type;


    // is_div_assignable : detects whether the type supports operator /= or not
    namespace detail
    {
        template <typename T>
        struct is_div_assignable_impl : dtk::is_numeric<T>
        {
        };
    }

    template <typename T>
    using is_div_assignable = detail::is_div_assignable_impl<T>;

    template <class T, class R = void>
    using enable_div_assignment = typename std::enable_if<is_div_assignable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_div_assignment = typename std::enable_if<!is_div_assignable<T>::value, R>::type;


    // is_bidirectional : detects whether the category of the iterator is bidirectional_iterator_tag or not
    namespace detail
    {
        template <typename T>
        struct is_bidirectional_impl : std::is_same<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>
        {
        };
    }

    template <typename T>
    using is_bidirectional = detail::is_bidirectional_impl<T>;


    // is_random_access : detects whether the category of the iterator is random_access_iterator_tag or not
    namespace detail
    {
        template <typename T>
        struct is_random_access_impl : std::is_same<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>
        {
        };
    }

    template <typename T>
    using is_random_access = detail::is_random_access_impl<T>;


    // is_sequential_container_pointer : detects whether the type is a pointer to a sequential container or not
    template <typename T>
    struct is_sequential_container_pointer : std::false_type
    {
    };


    // is_associative_container_pointer : detects whether the type is a pointer to an associative container or not
    template <typename T>
    struct is_associative_container_pointer : std::false_type
    {
    };


    // is_container_value_type_meta_type : detects whether the value type of a container is registered to the QMetaType system or not
    namespace detail
    {
        template <typename T>
        struct is_container_value_type_meta_type_impl : std::conditional_t<QMetaTypeId2<typename std::remove_pointer_t<T>::value_type>::Defined,
                                                                           std::true_type,
                                                                           std::false_type>
        {
        };
    }

    template <typename T>
    using is_container_value_type_meta_type = detail::is_container_value_type_meta_type_impl<T>;


    // is_resizable : detects whether the type supports resize operation or not
    template <typename T>
    struct is_resizable : std::false_type
    {
    };

    template <typename T>
    struct is_resizable<QVector<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_resizable<QVarLengthArray<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_resizable<QStack<T>> : std::true_type
    {
    };

    template <typename T, typename Allocator>
    struct is_resizable<std::vector<T, Allocator>> : std::true_type
    {
    };

    template <typename T, typename Allocator>
    struct is_resizable<std::list<T, Allocator>> : std::true_type
    {
    };

    template <class T, class R = void>
    using enable_resize = typename std::enable_if<is_resizable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_resize = typename std::enable_if<!is_resizable<T>::value, R>::type;


    // is_reservable : detects whether the type supports reserve operation or not
    template <typename T>
    struct is_reservable : std::false_type
    {
    };

    template <typename T>
    struct is_reservable<QVector<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_reservable<QVarLengthArray<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_reservable<QStack<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_reservable<QList<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_reservable<QQueue<T>> : std::true_type
    {
    };

    template <typename T>
    struct is_reservable<QSet<T>> : std::true_type
    {
    };

    template <typename T, typename Allocator>
    struct is_reservable<std::vector<T, Allocator>> : std::true_type
    {
    };

    template <class T, class R = void>
    using enable_reserve = typename std::enable_if<is_reservable<T>::value, R>::type;

    template <class T, class R = void>
    using disable_reserve = typename std::enable_if<!is_reservable<T>::value, R>::type;

}

//
// dtkCoreTypeTraits.h ends here
