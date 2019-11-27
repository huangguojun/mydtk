/* dtkGlobal.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu Oct 16 09:54:33 2008 (+0200)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <QtCore>

#include <dtkLog>

#include <dtkConfig.h>

// /////////////////////////////////////////////////////////////////
// Output colors
// /////////////////////////////////////////////////////////////////

#define DTK_COLOR_FG_BLACK    "\033[00;30m"
#define DTK_COLOR_FG_RED      "\033[00;31m"
#define DTK_COLOR_FG_GREEN    "\033[00;32m"
#define DTK_COLOR_FG_YELLOW   "\033[00;33m"
#define DTK_COLOR_FG_BLUE     "\033[00;34m"
#define DTK_COLOR_FG_MAGENTA  "\033[00;35m"
#define DTK_COLOR_FG_CYAN     "\033[00;36m"
#define DTK_COLOR_FG_WHITE    "\033[00;37m"

#define DTK_COLOR_FG_LTBLACK   "\033[01;30m"
#define DTK_COLOR_FG_LTRED     "\033[01;31m"
#define DTK_COLOR_FG_LTGREEN   "\033[01;32m"
#define DTK_COLOR_FG_LTYELLOW  "\033[01;33m"
#define DTK_COLOR_FG_LTBLUE    "\033[01;34m"
#define DTK_COLOR_FG_LTMAGENTA "\033[01;35m"
#define DTK_COLOR_FG_LTCYAN    "\033[01;36m"
#define DTK_COLOR_FG_LTWHITE   "\033[01;37m"

#define DTK_COLOR_BG_BLACK    "\033[00;40m"
#define DTK_COLOR_BG_RED      "\033[00;41m"
#define DTK_COLOR_BG_GREEN    "\033[00;42m"
#define DTK_COLOR_BG_YELLOW   "\033[00;43m"
#define DTK_COLOR_BG_BLUE     "\033[00;44m"
#define DTK_COLOR_BG_MAGENTA  "\033[00;45m"
#define DTK_COLOR_BG_CYAN     "\033[00;46m"
#define DTK_COLOR_BG_WHITE    "\033[00;47m"

#define DTK_COLOR_FG_BD       "\033[01m"
#define DTK_COLOR_FG_UL       "\033[04m"
#define DTK_NO_COLOR          "\033[00m"

// /////////////////////////////////////////////////////////////////
// Default implementation warning
// /////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#  define DTK_PRETTY_FUNCTION __FUNCSIG__
#elif defined __GNUG__
#  define DTK_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#  define DTK_PRETTY_FUNCTION __func__
#endif

#define DTK_DEFAULT_IMPLEMENTATION                                      \
    dtkDebug()                                                          \
    << "Using default implementation of"                                \
    << DTK_PRETTY_FUNCTION                                              \
    << "for"                                                            \
    << this->metaObject()->className()

#define DTK_DEFAULT_IMPLEMENTATION_NO_MOC                               \
    dtkDebug()                                                          \
    << "Using default implementation of"                                \
    << DTK_PRETTY_FUNCTION

#define DTK_UNUSED(variable) Q_UNUSED(variable)

#define DTK_SUCCEED 0
#define DTK_FAILURE 1

#define _DTK_STRINGIZE( x ) #x
#define DTK_STRINGIZE(x) _DTK_STRINGIZE(x)

#define DTK_LINK_TEXT __FILE__ "(" DTK_STRINGIZE(__LINE__) ") : "

#ifdef _MSC_VER
#  define DTK_COMPILER_WARNING(str) (DTK_LINK_TEXT "WARNING: " str)
#else
#  define DTK_COMPILER_WARNING(str) ("WARNING: " str)
#endif

#ifdef DTK_BUILD_32
#  define dtkxarch_int qint32
#  define dtkxarch_uint quint32
#elif defined DTK_BUILD_64
#  define dtkxarch_int qint64
#  define dtkxarch_uint quint64
#endif

#define dtkxarch_ptr quintptr

////////////////////////////////////////////////////
// Macro declaring private class in public class
////////////////////////////////////////////////////

#define DTK_DECLARE_PRIVATE(Class) \
    protected: \
    Class(Class##Private& dd, QObject *parent); \
    Class(Class##Private& dd, const Class& other); \
    inline Class##Private* d_func(void) { return reinterpret_cast<Class##Private *>(dtkAbstractObject::d_func()); } \
    inline const Class##Private* d_func(void) const { return reinterpret_cast<const Class##Private *>(dtkAbstractObject::d_func()); } \
    private: \
    friend class Class##Private;

////////////////////////////////////////////////////
// Macro implementing inline constructor of public class
////////////////////////////////////////////////////

#define DTK_IMPLEMENT_PRIVATE(Class, Parent) \
    inline Class::Class(Class##Private& dd, QObject *p) : Parent(dd, p) { }  \
    inline Class::Class(Class##Private& dd, const Class& o) : Parent(dd, o) { }

////////////////////////////////////////////////////
// Macro declaring public class in private one
////////////////////////////////////////////////////

#define DTK_DECLARE_PUBLIC(Class)                                  \
    inline Class* q_func() { return reinterpret_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return reinterpret_cast<const Class *>(q_ptr); } \
    friend class Class;

// /////////////////////////////////////////////////////////////////
// Macros retieving either d-pointer or q-pointer in the right type
// /////////////////////////////////////////////////////////////////

#define DTK_D(Class) Class##Private *const d = d_func()
#define DTK_Q(Class) Class *const q = q_func()

// /////////////////////////////////////////////////////////////////
// dtkTypeInfo
// /////////////////////////////////////////////////////////////////

template <typename T> class dtkTypeInfo
{
public:
    enum {
        dtkObjectPointer = false,
        dtkAbstractObjectPointer = false,
        dtkMatrixRealPointer = false
    };
};

// Specialize to avoid sizeof(void) warning

template<> class dtkTypeInfo<void *>
{
public:
    enum {
        dtkObjectPointer = false,
        dtkAbstractObjectPointer = false,
        dtkMatrixRealPointer = false
    };
};

// Partial Specialization for pointer type

class dtkAbstractObject;
template <typename T> class dtkMatrix;

template <typename T> class dtkTypeInfo<T *>
{
public:
    typedef int  yes_type;
    typedef char  no_type;

public:
    static yes_type checkObject(dtkAbstractObject *);
    static yes_type checkObject(dtkMatrix<qreal> *);
    static no_type  checkObject(...);

public:
    static yes_type checkAbstractObject(dtkAbstractObject *);
    static no_type  checkAbstractObject(...);

public:
    static yes_type checkAbstractMatrix(dtkMatrix<qreal> *);
    static no_type  checkAbstractMatrix(...);

public:
    enum {
        dtkObjectPointer         = (sizeof(checkObject(static_cast<T *>(0))) == sizeof(yes_type)),
        dtkAbstractObjectPointer = (sizeof(checkAbstractObject(static_cast<T *>(0))) == sizeof(yes_type)),
        dtkMatrixRealPointer     = (sizeof(checkAbstractMatrix(static_cast<T *>(0))) == sizeof(yes_type))
    };
};


#if defined(Q_OS_WIN) && !defined(__MINGW32__)
# define  sleep(t)  Sleep((t) * 1000)
# define msleep(t)  Sleep(t)
#else
# include <unistd.h>
# define msleep(t)  usleep((t) * 1000)
#endif

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

#include <fstream>

inline bool dtkIsBinary(const QString& path)
{
    int c; std::ifstream a(path.toUtf8().constData());

    while (((c = a.get()) != EOF) && (c <= 127)) {
        ;
    }

    return (c != EOF);
}

inline QString dtkReadFile(const QString& path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly))
        qDebug() << "Unable to read file" << path;

    QString contents = file.readAll();

    file.close();

    return contents;
}

inline bool dtkApplicationArgumentsContain(int argc, char **argv, QString value)
{
    for (int i = 1; i < argc; i++)
        if (QString(argv[i]) == value)
            return true;

    return false;
}

inline bool dtkApplicationArgumentsContain(QCoreApplication *application, QString value)
{
    return application->arguments().contains(value);
}

inline QString dtkApplicationArgumentsValue(int argc, char **argv, QString key)
{
    for (int i = 1; i < argc; i++) {
        if (QString(argv[i]) == key) {
            if (i + 1 < argc)
                return QString(argv[i + 1]);
            else
                return QString();
        }
    }

    return QString();
}

inline QString dtkApplicationArgumentsValue(QCoreApplication *application, QString key)
{
    QStringList args = application->arguments();
    int i = args.indexOf(key);

    if ( i > -1 && ( (i + 1) < args.count()) ) {
        return args.at(i + 1);
    } else {
        return QString();
    }
}

// /////////////////////////////////////////////////////////////////
// Hash functions
// /////////////////////////////////////////////////////////////////

inline uint qHash(const QStringList& key)
{
    uint hash = 0;

    foreach (QString string, key)
        hash = hash ^qHash(string);

    return hash;
}
