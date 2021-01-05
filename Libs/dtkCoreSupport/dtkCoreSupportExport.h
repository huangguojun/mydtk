
#ifndef DTKCORESUPPORT_EXPORT_H
#define DTKCORESUPPORT_EXPORT_H

#ifdef DTKCORESUPPORT_STATIC_DEFINE
#    define DTKCORESUPPORT_EXPORT
#    define DTKCORESUPPORT_NO_EXPORT
#else
#    ifndef DTKCORESUPPORT_EXPORT
#        ifdef dtkCoreSupport_EXPORTS
/* We are building this library */
#            define DTKCORESUPPORT_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKCORESUPPORT_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKCORESUPPORT_NO_EXPORT
#        define DTKCORESUPPORT_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKCORESUPPORT_DEPRECATED
#    define DTKCORESUPPORT_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKCORESUPPORT_DEPRECATED_EXPORT
#    define DTKCORESUPPORT_DEPRECATED_EXPORT DTKCORESUPPORT_EXPORT DTKCORESUPPORT_DEPRECATED
#endif

#ifndef DTKCORESUPPORT_DEPRECATED_NO_EXPORT
#    define DTKCORESUPPORT_DEPRECATED_NO_EXPORT DTKCORESUPPORT_NO_EXPORT DTKCORESUPPORT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKCORESUPPORT_NO_DEPRECATED
#        define DTKCORESUPPORT_NO_DEPRECATED
#    endif
#endif

#endif
