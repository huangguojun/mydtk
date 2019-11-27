
#ifndef DTKLOG_EXPORT_H
#define DTKLOG_EXPORT_H

#ifdef DTKLOG_STATIC_DEFINE
#  define DTKLOG_EXPORT
#  define DTKLOG_NO_EXPORT
#else
#  ifndef DTKLOG_EXPORT
#    ifdef dtkLog_EXPORTS
        /* We are building this library */
#      define DTKLOG_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define DTKLOG_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef DTKLOG_NO_EXPORT
#    define DTKLOG_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DTKLOG_DEPRECATED
#  define DTKLOG_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef DTKLOG_DEPRECATED_EXPORT
#  define DTKLOG_DEPRECATED_EXPORT DTKLOG_EXPORT DTKLOG_DEPRECATED
#endif

#ifndef DTKLOG_DEPRECATED_NO_EXPORT
#  define DTKLOG_DEPRECATED_NO_EXPORT DTKLOG_NO_EXPORT DTKLOG_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DTKLOG_NO_DEPRECATED
#    define DTKLOG_NO_DEPRECATED
#  endif
#endif

#endif
