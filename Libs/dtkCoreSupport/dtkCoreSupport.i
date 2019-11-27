/* Version: $Id$
 *
 */

/* Commentary:
 *
 */

/* Change Log:
 *
 */

/* Code: */

#pragma once

%module coresupport

%include "carrays.i"

%array_class(double, doubleArray);

%{

#include <QtDebug>
#include <QtCore>

#include <dtkCoreSupport/dtkAbstractData.h>
#include <dtkCoreSupport/dtkAbstractDataFactory.h>
#include <dtkCoreSupport/dtkAbstractDataReader.h>
#include <dtkCoreSupport/dtkAbstractDataWriter.h>
#include <dtkCoreSupport/dtkAbstractDataConverter.h>
#include <dtkCoreSupport/dtkAbstractDataSerializer.h>
#include <dtkCoreSupport/dtkAbstractObject.h>
#include <dtkCoreSupport/dtkAbstractProcess.h>
#include <dtkCoreSupport/dtkAbstractProcessFactory.h>
#include <dtkCoreSupport/dtkAbstractView.h>
#include <dtkCoreSupport/dtkAbstractViewAnimator.h>
#include <dtkCoreSupport/dtkAbstractViewFactory.h>
#include <dtkCoreSupport/dtkAbstractViewInteractor.h>
#include <dtkCoreSupport/dtkAbstractViewNavigator.h>
#include <dtkCoreSupport/dtkPlugin.h>
#include <dtkCoreSupport/dtkPluginManager.h>
#include <dtkCoreSupport/dtkSmartPointer.h>

#include <string>
%}

// /////////////////////////////////////////////////////////////////
// Preprocessing setup
// /////////////////////////////////////////////////////////////////

#pragma SWIG nowarn=389, 401, 509, 801, 472, 362, 503, 516, 842, 845

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef Q_OBJECT
#undef signals
#undef slots

#define Q_OBJECT
#define signals public
#define slots

#undef  Q_PROPERTY(Type type MODE mode)
#define Q_PROPERTY(Type type MODE mode)

#undef  Q_DECLARE_INTERFACE(IFace, IId)
#define Q_DECLARE_INTERFACE(IFace, IId)

#undef  Q_DECLARE_METATYPE(Type type)
#define Q_DECLARE_METATYPE(Type type)

#undef  DTKCORESUPPORT_EXPORT
#define DTKCORESUPPORT_EXPORT

#undef  DTK_DEPRECATED
#define DTK_DEPRECATED

// /////////////////////////////////////////////////////////////////
// Ignore rules for operators
// /////////////////////////////////////////////////////////////////

%ignore operator>>;
%ignore operator<<;
%ignore operator==;
%ignore operator[];
%ignore operator!=;
%ignore operator*=;
%ignore operator/=;
%ignore operator bool;
%ignore operator int;
%ignore operator float;
%ignore operator double;
%ignore operator double *;

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkAbstractObject signals
// /////////////////////////////////////////////////////////////////

%ignore addProperty(const QString& key, const QStringList& values); // No scripter should add properties dynamically
%ignore addProperty(const QString& key, const QString& value);      // No scripter should add properties dynamically

%ignore propertySet(const QString& key, const QString& value);
%ignore metaDataSet(const QString& key, const QString& value);

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkAbstractDataReader|Writer|Process
// /////////////////////////////////////////////////////////////////

%ignore started(const QString& message);
%ignore elapsed(const QString& duration);
%ignore progressed(int step);
%ignore progressed(const QString& message);
%ignore success();
%ignore failure();
%ignore finished();
%ignore success();
%ignore failure();
%ignore canceled();

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkAbstractView signals and deprecated
// /////////////////////////////////////////////////////////////////

%ignore closed();
%ignore focused();
%ignore nameChanged();

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkAbstractViewFactory signals
// /////////////////////////////////////////////////////////////////

%ignore cleared();

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkAbstractViewInteractor signals
// /////////////////////////////////////////////////////////////////

%ignore updated();

// /////////////////////////////////////////////////////////////////
// Ignore rules for factories
// /////////////////////////////////////////////////////////////////

%ignore created(dtkAbstractData *data, const QString& type);
%ignore created(dtkAbstractProcess *process, const QString& type);
%ignore created(dtkAbstractView *view, const QString& type);

// /////////////////////////////////////////////////////////////////
// Ignore rules for dtkPluginManager
// /////////////////////////////////////////////////////////////////

%ignore   loaded(const QString& path);
%ignore unloaded(const QString& path);
%ignore   loadError(const QString& path);

// /////////////////////////////////////////////////////////////////
// Typemaps
// /////////////////////////////////////////////////////////////////

#ifdef SWIGPYTHON

%typecheck(SWIG_TYPECHECK_STRING) char * {
    $1 = PyString_Check($input) ? 1 : 0;
}

%typemap(typecheck)       QString  = char *;
%typemap(typecheck) const QString& = char *;

// Python -> C++

%typemap(in) QString {
    if (PyString_Check($input)) {
        $1 = QString(PyString_AsString($input));
    } else {
        qDebug("QString expected");
    }
}

%typemap(in) const QString& {
    if (PyString_Check($input)) {
        char *t = PyString_AsString($input);
        $1 = new QString(t);
    } else {
        qDebug("QString expected");
    }
}

%typemap(in) QStringList {
    if (PyList_Check($input)) {
        int i = 0;
        int end = PyList_Size($input);
        for(i;i!=end; ++i) {
            $1 << QString(PyString_AsString(PyList_GET_ITEM($input, i)));
        }
    } else {
        qDebug("QStringList expected");
    }
}

%typemap(in) const QStringList& {
    if (PyList_Check($input)) {
        int i = 0;
        int end = PyList_Size($input);
        $1 = new QStringList;
        for(i;i!=end; ++i) {
            char *t = PyString_AsString(PyList_GET_ITEM($input, i));
            (*$1) << QString(t);
        }
    } else {
        qDebug("QStringList expected");
    }
}

// C++ -> Python

%typemap(out) QString {
    $result = PyString_FromString($1.toUtf8().constData());
}

%typemap(out) const QString& {
    $result = PyString_FromString($1.toUtf8().constData());
}

%define %QList_typemapsPtr(DATA_TYPE)

%typemap(out) QList<DATA_TYPE> {
    $result = PyList_New($1.size());
    int i = 0;
    QList<DATA_TYPE>::iterator it = $1.begin();
    QList<DATA_TYPE>::iterator end = $1.end();
    for(;it!=end; ++it, ++i)  {
        PyObject* obj = SWIG_NewPointerObj((*it), $descriptor(DATA_TYPE), 0|0);
        PyList_SET_ITEM($result, i, obj);
    }
}

%enddef // %QList_typemapsPtr()

%QList_typemapsPtr(dtkPlugin *)

%define %QList_typemaps(DATA_TYPE)

%typemap(out) QList<DATA_TYPE> {
    $result = PyList_New($1.size());
    int i = 0;
    QList<DATA_TYPE>::iterator it = $1.begin();
    QList<DATA_TYPE>::iterator end = $1.end();
    for(;it!=end; ++it, ++i)  {
        DATA_TYPE *newItem = new DATA_TYPE(*it);
        PyObject* obj = SWIG_NewPointerObj(newItem, $descriptor(DATA_TYPE*), 0|0);
        PyList_SET_ITEM($result, i, obj);
    }
}

%enddef // %QList_typemaps()

%typemap(out) QStringList {
    $result = PyList_New($1.size());
    int i = 0;
    QStringList::iterator it = $1.begin();
    QStringList::iterator end = $1.end();
    for(;it!=end; ++it, ++i) {
        PyObject* st = PyString_FromString((*it).toUtf8().constData());
        PyList_SET_ITEM($result, i, st);
    }
}

%typemap(out) QList<QString> {
    $result = PyList_New($1.size());
    int i = 0;
    QStringList::iterator it = $1.begin();
    QStringList::iterator end = $1.end();
    for(;it!=end; ++it, ++i) {
        PyObject* st = PyString_FromString((*it).toUtf8().constData());
        PyList_SET_ITEM($result, i, st);
    }
}

template <class T1, class T2> class QPair
{
public:
    T1 first;
    T2 second;
};

%define %QPair_typemaps(DATA_TYPE_1, DATA_TYPE_2)

%typemap(out) QPair<DATA_TYPE_1, DATA_TYPE_2> {
    $result = PyTuple_New(2);
    PyObject* obj1 = SWIG_NewPointerObj(*$1.first, $descriptor(DATA_TYPE_1), 0|0);
    PyObject* obj2 = SWIG_NewPointerObj(*$1.second, $descriptor(DATA_TYPE_2), 0|0);
    PyTuple_SET_ITEM($result, 0, obj1);
    PyTuple_SET_ITEM($result, 1, obj2);
}

%enddef // %QPair_typemaps()

%template(QPairStrStrList) QPair<QString, QStringList>;

%QPair_typemaps(QString, QStringList)

%QList_typemaps(dtkAbstractDataFactory::dtkAbstractDataTypeHandler)

#elif SWIGTCL

// Tcl -> C++

%typemap(in) QString {
    $1 = QString(Tcl_GetString($input));
}

%typemap(in) const QString& {
    char *t = Tcl_GetString($input);
    $1 = new QString(t);
}

// C++ -> Tcl

%typemap(out) QString {
    Tcl_SetStringObj($result, $1.toUtf8().constData(), $1.size());
}

%typemap(out) const QString& {
    Tcl_SetStringObj($result, $1.toUtf8().constData(), $1.size());
}

#elif SWIGCSHARP

// /////////////////////////////////////////////////////////////////
// C#
// /////////////////////////////////////////////////////////////////

%typemap(ctype) QString "char *"
%typemap(imtype) QString "string"
%typemap(cstype) QString "string"
%typemap(csdirectorin) QString "$iminput"
%typemap(csdirectorout) QString "$cscall"

%typemap(in, canthrow=1) QString {
    if (!$input) {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null string", 0);
        return $null;
    }

    $1 = QString($input);
}

%typemap(out) QString {
    $result = SWIG_csharp_string_callback($1.toUtf8().constData());
}

%typemap(directorout, canthrow=1) QString {
    if (!$input) {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null string", 0);
        return $null;
    }

    $result = string($input);
}

%typemap(directorin) QString {
    $input = SWIG_csharp_string_callback($1.toUtf8().constData());
}

%typemap(csin) QString "$csinput"

%typemap(csout, excode=SWIGEXCODE) QString {
    string ret = $imcall;$excode;
    return ret;
}

%typemap(csvarin, excode=SWIGEXCODE2) QString {
    set {
      $imcall;$excode
    }
}

%typemap(csvarout, excode=SWIGEXCODE2) QString {
    get {
      string ret = $imcall;$excode
      return ret;
    }
}

%typemap(typecheck) QString = char *;

%typemap(throws, canthrow=1) QString {
    SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.c_str());
    return $null;
}

%typemap(ctype) const QString & "char *"
%typemap(imtype) const QString & "string"
%typemap(cstype) const QString & "string"
%typemap(csdirectorin) const QString & "$iminput"
%typemap(csdirectorout) const QString & "$cscall"

%typemap(in, canthrow=1) const QString & {
    if (!$input) {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null string", 0);
        return $null;
    }
    QString $1_str($input);
    $1 = &$1_str;
}

%typemap(out) const QString & {
    $result = SWIG_csharp_string_callback($1->toUtf8().constData());
}

%typemap(csin) const QString & "$csinput"

%typemap(csout, excode=SWIGEXCODE) const QString & {
    string ret = $imcall;$excode
    return ret;
}

%typemap(directorout, canthrow=1, warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const QString & {
    if (!$input) {
        SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpArgumentNullException, "null string", 0);
        return $null;
   }
   static string $1_str;
   $1_str = $input;
   $result = &$1_str;
}

%typemap(directorin) const QString & {
    $input = SWIG_csharp_string_callback($1.toUtf8().constData());
}

%typemap(csvarin, excode=SWIGEXCODE2) const QString & {
    set {
      $imcall;$excode
    }
}

%typemap(csvarout, excode=SWIGEXCODE2) const QString & {
    get {
        string ret = $imcall;$excode;
        return ret;
    }
}

%typemap(typecheck) const QString & = char *;

%typemap(throws, canthrow=1) const QString & {
    SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.c_str());
    return $null;
}

#endif

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%include "dtkAbstractObject.h"
%include "dtkAbstractData.h"
%include "dtkAbstractDataFactory.h"
%include "dtkAbstractDataReader.h"
%include "dtkAbstractDataWriter.h"
%include "dtkAbstractDataConverter.h"
%include "dtkAbstractProcess.h"
%include "dtkAbstractProcessFactory.h"
%include "dtkAbstractView.h"
%include "dtkAbstractViewFactory.h"
%include "dtkAbstractViewAnimator.h"
%include "dtkAbstractViewInteractor.h"
%include "dtkAbstractViewNavigator.h"
%include "dtkPlugin.h"
%include "dtkPluginManager.h"

/* dtkCoreSupport.i ends here */
