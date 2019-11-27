
#ifndef DTKCOMPOSER_EXPORT_H
#define DTKCOMPOSER_EXPORT_H

#ifdef DTKCOMPOSER_STATIC_DEFINE
#  define DTKCOMPOSER_EXPORT
#  define DTKCOMPOSER_NO_EXPORT
#else
#  ifndef DTKCOMPOSER_EXPORT
#    ifdef dtkComposer_EXPORTS
        /* We are building this library */
#      define DTKCOMPOSER_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DTKCOMPOSER_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DTKCOMPOSER_NO_EXPORT
#    define DTKCOMPOSER_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DTKCOMPOSER_DEPRECATED
#  define DTKCOMPOSER_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DTKCOMPOSER_DEPRECATED_EXPORT
#  define DTKCOMPOSER_DEPRECATED_EXPORT DTKCOMPOSER_EXPORT DTKCOMPOSER_DEPRECATED
#endif

#ifndef DTKCOMPOSER_DEPRECATED_NO_EXPORT
#  define DTKCOMPOSER_DEPRECATED_NO_EXPORT DTKCOMPOSER_NO_EXPORT DTKCOMPOSER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DTKCOMPOSER_NO_DEPRECATED
#    define DTKCOMPOSER_NO_DEPRECATED
#  endif
#endif

#endif
