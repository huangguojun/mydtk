
#ifndef DTKIO_EXPORT_H
#define DTKIO_EXPORT_H

#ifdef DTKIO_STATIC_DEFINE
#  define DTKIO_EXPORT
#  define DTKIO_NO_EXPORT
#else
#  ifndef DTKIO_EXPORT
#    ifdef dtkIo_EXPORTS
        /* We are building this library */
#      define DTKIO_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DTKIO_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DTKIO_NO_EXPORT
#    define DTKIO_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DTKIO_DEPRECATED
#  define DTKIO_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DTKIO_DEPRECATED_EXPORT
#  define DTKIO_DEPRECATED_EXPORT DTKIO_EXPORT DTKIO_DEPRECATED
#endif

#ifndef DTKIO_DEPRECATED_NO_EXPORT
#  define DTKIO_DEPRECATED_NO_EXPORT DTKIO_NO_EXPORT DTKIO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DTKIO_NO_DEPRECATED
#    define DTKIO_NO_DEPRECATED
#  endif
#endif

#endif /* DTKIO_EXPORT_H */
