
#ifndef DTKCORE_EXPORT_H
#define DTKCORE_EXPORT_H

#ifdef DTKCORE_STATIC_DEFINE
#    define DTKCORE_EXPORT
#    define DTKCORE_NO_EXPORT
#else
#    ifndef DTKCORE_EXPORT
#        ifdef dtkCore_EXPORTS
/* We are building this library */
#            define DTKCORE_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKCORE_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKCORE_NO_EXPORT
#        define DTKCORE_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKCORE_DEPRECATED
#    define DTKCORE_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKCORE_DEPRECATED_EXPORT
#    define DTKCORE_DEPRECATED_EXPORT DTKCORE_EXPORT DTKCORE_DEPRECATED
#endif

#ifndef DTKCORE_DEPRECATED_NO_EXPORT
#    define DTKCORE_DEPRECATED_NO_EXPORT DTKCORE_NO_EXPORT DTKCORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKCORE_NO_DEPRECATED
#        define DTKCORE_NO_DEPRECATED
#    endif
#endif

#endif
