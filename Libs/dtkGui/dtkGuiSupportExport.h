
#ifndef DTKGUISUPPORT_EXPORT_H
#define DTKGUISUPPORT_EXPORT_H

#ifdef DTKGUISUPPORT_STATIC_DEFINE
#    define DTKGUISUPPORT_EXPORT
#    define DTKGUISUPPORT_NO_EXPORT
#else
#    ifndef DTKGUISUPPORT_EXPORT
#        ifdef dtkGuiSupport_EXPORTS
/* We are building this library */
#            define DTKGUISUPPORT_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKGUISUPPORT_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKGUISUPPORT_NO_EXPORT
#        define DTKGUISUPPORT_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKGUISUPPORT_DEPRECATED
#    define DTKGUISUPPORT_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKGUISUPPORT_DEPRECATED_EXPORT
#    define DTKGUISUPPORT_DEPRECATED_EXPORT DTKGUISUPPORT_EXPORT DTKGUISUPPORT_DEPRECATED
#endif

#ifndef DTKGUISUPPORT_DEPRECATED_NO_EXPORT
#    define DTKGUISUPPORT_DEPRECATED_NO_EXPORT DTKGUISUPPORT_NO_EXPORT DTKGUISUPPORT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKGUISUPPORT_NO_DEPRECATED
#        define DTKGUISUPPORT_NO_DEPRECATED
#    endif
#endif

#endif
