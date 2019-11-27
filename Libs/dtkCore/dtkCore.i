// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

%module(directors="1") dtkcore

%include <dtk_base.i>
%include <qrunnable.i>

%{

#include <dtkCore/dtkCorePlugin.h>
#include <dtkCore/dtkCorePluginBase.h>
#include <dtkCore/dtkCorePluginManager.h>
#include <dtkCore/dtkCorePluginFactory.h>
#include <dtkCore/dtkObjectManager.h>
#include <dtkCore/dtkArray.h>

%}

%feature("director");

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  DTKCORE_EXPORT
#define DTKCORE_EXPORT

%pythonappend dtkObjectManager::value(const QString &key) const  %{
method_name = "to" + val.typeName().strip("*")
try:
    method = getattr(val, method_name)
    return method()
except:
    print "can't convert to real type, return a QVariant"
    return val
%}

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%include <dtkCore/dtkCorePluginBase.h>
%include <dtkCore/dtkCorePluginManager.h>
%include <dtkCore/dtkCorePluginFactory.h>
%include <dtkCore/dtkArray.h>
%include <dtkCore/dtkObjectManager.h>

%template(dtkArrayInt)    dtkArray<long long>;
%template(dtkArrayDouble) dtkArray<double>;

//
// dtkCore.i ends here
