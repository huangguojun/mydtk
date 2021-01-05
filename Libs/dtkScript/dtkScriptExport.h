
#ifndef DTKSCRIPT_EXPORT_H
#define DTKSCRIPT_EXPORT_H

#ifdef DTKSCRIPT_STATIC_DEFINE
#    define DTKSCRIPT_EXPORT
#    define DTKSCRIPT_NO_EXPORT
#else
#    ifndef DTKSCRIPT_EXPORT
#        ifdef dtkScript_EXPORTS
/* We are building this library */
#            define DTKSCRIPT_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKSCRIPT_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKSCRIPT_NO_EXPORT
#        define DTKSCRIPT_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKSCRIPT_DEPRECATED
#    define DTKSCRIPT_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKSCRIPT_DEPRECATED_EXPORT
#    define DTKSCRIPT_DEPRECATED_EXPORT DTKSCRIPT_EXPORT DTKSCRIPT_DEPRECATED
#endif

#ifndef DTKSCRIPT_DEPRECATED_NO_EXPORT
#    define DTKSCRIPT_DEPRECATED_NO_EXPORT DTKSCRIPT_NO_EXPORT DTKSCRIPT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKSCRIPT_NO_DEPRECATED
#        define DTKSCRIPT_NO_DEPRECATED
#    endif
#endif

#endif
