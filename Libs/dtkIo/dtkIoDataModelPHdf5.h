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

#include <dtkCore>

#include "dtkIoDataModel.h"

class dtkDistributedCommunicator;
class dtkIoDataModelPHdf5Private;

// ///////////////////////////////////////////////////////////////////
// dtkIoDataModelPHdf5 declaration
// ///////////////////////////////////////////////////////////////////

class dtkIoDataModelPHdf5 : public dtkIoDataModel
{
public:
     dtkIoDataModelPHdf5(void);
    ~dtkIoDataModelPHdf5(void);

public:
    void setCommunicator(dtkDistributedCommunicator *comm) override;

public:
    void fileOpen(const QString& name, FileMode mode) override;
    void fileClose(void) override;

public:
    void read(const QString& dataset_name, DataType type, void *values) override;

public:
    void write(const QString& dataset_name, DataType type, int dimension, quint64 *shape, void *values = nullptr) override;

public:
    void writeHyperslab(const QString& dataset_name, DataType type, quint64 *offset, quint64 *stride, quint64 *count, quint64 *block, quint64 *values_shape, void *values)  override;
    void writeByCoord(const QString& dataset_name, DataType type, quint64 nb_points, quint64* points_coord, void *values);

public:
    dtkIoDataModelPHdf5Private *d;
};

// ///////////////////////////////////////////////////////////////////
// Creator
// ///////////////////////////////////////////////////////////////////

static dtkIoDataModel *dtkIoDataModelPHdf5Creator(void)
{
    return new dtkIoDataModelPHdf5();
}

//
// dtkIoDataModelPHdf5.h ends here
