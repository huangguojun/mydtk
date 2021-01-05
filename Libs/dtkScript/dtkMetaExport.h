
#ifndef DTKMETA_EXPORT_H
#define DTKMETA_EXPORT_H

#ifdef DTKMETA_STATIC_DEFINE
#    define DTKMETA_EXPORT
#    define DTKMETA_NO_EXPORT
#else
#    ifndef DTKMETA_EXPORT
#        ifdef dtkMeta_EXPORTS
/* We are building this library */
#            define DTKMETA_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKMETA_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKMETA_NO_EXPORT
#        define DTKMETA_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKMETA_DEPRECATED
#    define DTKMETA_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKMETA_DEPRECATED_EXPORT
#    define DTKMETA_DEPRECATED_EXPORT DTKMETA_EXPORT DTKMETA_DEPRECATED
#endif

#ifndef DTKMETA_DEPRECATED_NO_EXPORT
#    define DTKMETA_DEPRECATED_NO_EXPORT DTKMETA_NO_EXPORT DTKMETA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKMETA_NO_DEPRECATED
#        define DTKMETA_NO_DEPRECATED
#    endif
#endif

#endif
