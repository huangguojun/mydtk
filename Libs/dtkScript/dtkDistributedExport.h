
#ifndef DTKDISTRIBUTED_EXPORT_H
#define DTKDISTRIBUTED_EXPORT_H

#ifdef DTKDISTRIBUTED_STATIC_DEFINE
#    define DTKDISTRIBUTED_EXPORT
#    define DTKDISTRIBUTED_NO_EXPORT
#else
#    ifndef DTKDISTRIBUTED_EXPORT
#        ifdef dtkDistributed_EXPORTS
/* We are building this library */
#            define DTKDISTRIBUTED_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKDISTRIBUTED_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKDISTRIBUTED_NO_EXPORT
#        define DTKDISTRIBUTED_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKDISTRIBUTED_DEPRECATED
#    define DTKDISTRIBUTED_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKDISTRIBUTED_DEPRECATED_EXPORT
#    define DTKDISTRIBUTED_DEPRECATED_EXPORT DTKDISTRIBUTED_EXPORT DTKDISTRIBUTED_DEPRECATED
#endif

#ifndef DTKDISTRIBUTED_DEPRECATED_NO_EXPORT
#    define DTKDISTRIBUTED_DEPRECATED_NO_EXPORT DTKDISTRIBUTED_NO_EXPORT DTKDISTRIBUTED_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKDISTRIBUTED_NO_DEPRECATED
#        define DTKDISTRIBUTED_NO_DEPRECATED
#    endif
#endif

#endif
