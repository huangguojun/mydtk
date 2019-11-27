
#ifndef DTKMATHSUPPORT_EXPORT_H
#define DTKMATHSUPPORT_EXPORT_H

#ifdef DTKMATHSUPPORT_STATIC_DEFINE
#  define DTKMATHSUPPORT_EXPORT
#  define DTKMATHSUPPORT_NO_EXPORT
#else
#  ifndef DTKMATHSUPPORT_EXPORT
#    ifdef dtkMathSupport_EXPORTS
        /* We are building this library */
#      define DTKMATHSUPPORT_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DTKMATHSUPPORT_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DTKMATHSUPPORT_NO_EXPORT
#    define DTKMATHSUPPORT_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DTKMATHSUPPORT_DEPRECATED
#  define DTKMATHSUPPORT_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DTKMATHSUPPORT_DEPRECATED_EXPORT
#  define DTKMATHSUPPORT_DEPRECATED_EXPORT DTKMATHSUPPORT_EXPORT DTKMATHSUPPORT_DEPRECATED
#endif

#ifndef DTKMATHSUPPORT_DEPRECATED_NO_EXPORT
#  define DTKMATHSUPPORT_DEPRECATED_NO_EXPORT DTKMATHSUPPORT_NO_EXPORT DTKMATHSUPPORT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DTKMATHSUPPORT_NO_DEPRECATED
#    define DTKMATHSUPPORT_NO_DEPRECATED
#  endif
#endif

#endif
