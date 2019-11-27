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

#pragma once

%module dtkwidgets

%include "dtk_base.i"

%include "qt_core.i"
%include "qt_widgets.i"

%{

#include <dtkWidgets/dtkViewController.h>
#include <dtkWidgets/dtkViewManager.h>
#include <dtkWidgets/dtkViewWidget.h>

%}

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  DTKWIDGETS_EXPORT
#define DTKWIDGETS_EXPORT

// ///////////////////////////////////////////////////////////////////
// Conversion functions
// ///////////////////////////////////////////////////////////////////

%inline %{

void dtkSetParent(PyObject *source, QWidget *parent)
{
    Py_XINCREF(source);

    QWidget *s = 0; void *s_p = 0; int r = 0;

    r = SWIG_ConvertPtr(source, &s_p, SWIGTYPE_p_QWidget, 0);

    if (SWIG_IsOK(r)) {
        s = reinterpret_cast<QWidget *>(s_p);
        s->setParent(parent);
    }
}

void dtkSetLayout(QWidget *widget, PyObject *layout)
{
    Py_XINCREF(layout);

    QLayout *l = 0; void *l_p = 0; int r = 0;

    r = SWIG_ConvertPtr(layout, &l_p, SWIGTYPE_p_QLayout, 0);

    if (SWIG_IsOK(r)) {
        l = reinterpret_cast<QLayout *>(l_p);
        widget->setLayout(l);
    }
}

%}

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%include <dtkWidgets/dtkViewController.h>
%include <dtkWidgets/dtkViewManager.h>
%include <dtkWidgets/dtkViewWidget.h>

//
// dtkWidgets.i ends here
