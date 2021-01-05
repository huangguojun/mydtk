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

#include <dtkWidgetsExport>

#include <QtWidgets/QWidget>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameter
// ///////////////////////////////////////////////////////////////////

class dtkCoreParameter;

class DTKWIDGETS_EXPORT dtkWidgetsParameter : public QWidget
{
public:
    dtkWidgetsParameter(QWidget *);
    ~dtkWidgetsParameter(void) = default;

public:
    virtual bool connect(dtkCoreParameter *) = 0;

public:
    virtual dtkCoreParameter *parameter(void) const = 0;
};

Q_DECLARE_METATYPE(dtkWidgetsParameter *);

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterBase declaration
// ///////////////////////////////////////////////////////////////////

template<typename T>
class dtkWidgetsParameterBase : public dtkWidgetsParameter
{
public:
    dtkWidgetsParameterBase(QWidget *);
    ~dtkWidgetsParameterBase(void) = default;

    dtkCoreParameter *parameter(void) const final;

protected:
    T *m_parameter = nullptr;
};

// ///////////////////////////////////////////////////////////////////

template<typename T>
inline dtkWidgetsParameterBase<T>::dtkWidgetsParameterBase(QWidget *p) : dtkWidgetsParameter(p)
{
}

template<typename T>
inline dtkCoreParameter *dtkWidgetsParameterBase<T>::parameter(void) const
{
    return m_parameter;
}

//
// dtkWidgetsParameter.h ends here
