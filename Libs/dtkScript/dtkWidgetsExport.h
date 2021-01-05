
#ifndef DTKWIDGETS_EXPORT_H
#define DTKWIDGETS_EXPORT_H

#ifdef DTKWIDGETS_STATIC_DEFINE
#    define DTKWIDGETS_EXPORT
#    define DTKWIDGETS_NO_EXPORT
#else
#    ifndef DTKWIDGETS_EXPORT
#        ifdef dtkWidgets_EXPORTS
/* We are building this library */
#            define DTKWIDGETS_EXPORT __attribute__((visibility("default")))
#        else
/* We are using this library */
#            define DTKWIDGETS_EXPORT __attribute__((visibility("default")))
#        endif
#    endif

#    ifndef DTKWIDGETS_NO_EXPORT
#        define DTKWIDGETS_NO_EXPORT __attribute__((visibility("hidden")))
#    endif
#endif

#ifndef DTKWIDGETS_DEPRECATED
#    define DTKWIDGETS_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef DTKWIDGETS_DEPRECATED_EXPORT
#    define DTKWIDGETS_DEPRECATED_EXPORT DTKWIDGETS_EXPORT DTKWIDGETS_DEPRECATED
#endif

#ifndef DTKWIDGETS_DEPRECATED_NO_EXPORT
#    define DTKWIDGETS_DEPRECATED_NO_EXPORT DTKWIDGETS_NO_EXPORT DTKWIDGETS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#    ifndef DTKWIDGETS_NO_DEPRECATED
#        define DTKWIDGETS_NO_DEPRECATED
#    endif
#endif

#endif
