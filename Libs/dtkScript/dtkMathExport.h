
#ifndef DTKMATH_EXPORT_H
#define DTKMATH_EXPORT_H

#ifdef DTKMATH_STATIC_DEFINE
#    define DTKMATH_EXPORT
#    define DTKMATH_NO_EXPORT
#else
#    ifndef DTKMATH_EXPORT
#        ifdef dtkMath_EXPORTS
/* We are building this library */
#            define DTKMATH_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKMATH_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKMATH_NO_EXPORT
#        define DTKMATH_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKMATH_DEPRECATED
#    define DTKMATH_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKMATH_DEPRECATED_EXPORT
#    define DTKMATH_DEPRECATED_EXPORT DTKMATH_EXPORT DTKMATH_DEPRECATED
#endif

#ifndef DTKMATH_DEPRECATED_NO_EXPORT
#    define DTKMATH_DEPRECATED_NO_EXPORT DTKMATH_NO_EXPORT DTKMATH_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKMATH_NO_DEPRECATED
#        define DTKMATH_NO_DEPRECATED
#    endif
#endif

#endif
