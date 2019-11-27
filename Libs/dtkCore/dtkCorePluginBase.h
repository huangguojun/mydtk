#pragma once

#include <dtkCoreExport.h>

#include <QtCore>

class DTKCORE_EXPORT dtkCorePluginBase : public QObject
{
    Q_OBJECT

public:
    virtual ~dtkCorePluginBase(void) = default;

public:
    virtual void   initialize(void) = 0;
    virtual void uninitialize(void) = 0;
    virtual void      *create(void) { return nullptr; };
};
