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

#include "dtkIoExport.h"

#include <QtCore>
#include <dtkCore>

class dtkDistributedCommunicator;

// ///////////////////////////////////////////////////////////////////
// dtkIoDataModel declaration
// ///////////////////////////////////////////////////////////////////

class DTKIO_EXPORT dtkIoDataModel : public QObject
{
public:
    enum FileMode { Trunc = 0x001, NotExisting = 0x002, ReadOnly = 0x003, ReadWrite = 0x004 };

    enum DataType {
        Int = 1,
        LongLongInt = 2,
        Double = 3,
    };

public:
    dtkIoDataModel(void);
    virtual ~dtkIoDataModel(void);

public:
    virtual void setCommunicator(dtkDistributedCommunicator *comm) = 0;

public:
    virtual void fileOpen(const QString &name, FileMode mode) = 0;
    virtual void fileClose(void) = 0;

public:
    virtual void read(const QString &dataset_name, DataType type, void *values) = 0;

public:
    virtual void write(const QString &dataset_name, DataType type, int dimension, quint64 *shape,
                       void *values = NULL) = 0;

public:
    virtual void readByCoord(const QString &dataset_name, DataType type, quint64 nb_points,
                             quint64 *points_coord, void *values);

public:
    virtual void writeHyperslab(const QString &dataset_name, DataType type, quint64 *offset,
                                quint64 *stride, quint64 *count, quint64 *block,
                                quint64 *values_shape, void *values);
    virtual void writeByCoord(const QString &dataset_name, DataType type, quint64 nb_points,
                              quint64 *points_coord, void *values);
};

// ///////////////////////////////////////////////////////////////////
// Plugin system and declaration to QMetaType system
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(dtkIoDataModel *)
DTK_DECLARE_PLUGIN(dtkIoDataModel, DTKIO_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(dtkIoDataModel, DTKIO_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(dtkIoDataModel, DTKIO_EXPORT)

//
// dtkIoDataModel.h ends here
