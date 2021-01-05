/* dtkIoDataModelHdf5.h ---
 *
 * Version:
 *
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include "dtkIoDataModel.h"
#include <dtkCore>

class dtkDistributedCommunicator;
class dtkIoDataModelHdf5Private;

// ///////////////////////////////////////////////////////////////////
// dtkIoDataModelHdf5 declaration
// ///////////////////////////////////////////////////////////////////

class dtkIoDataModelHdf5 : public dtkIoDataModel
{
public:
    dtkIoDataModelHdf5(void);
    ~dtkIoDataModelHdf5(void);

public:
    void setCommunicator(dtkDistributedCommunicator *comm) override;

public:
    void fileOpen(const QString &name, FileMode mode) override;
    void fileClose(void) override;

public:
    void read(const QString &dataset_name, DataType type, void *values) override;

public:
    void write(const QString &dataset_name, DataType type, int dimension, quint64 *shape,
               void *values) override;

public:
    void writeHyperslab(const QString &dataset_name, DataType type, quint64 *offset,
                        quint64 *stride, quint64 *count, quint64 *block, quint64 *values_shape,
                        void *values) override;
    void writeByCoord(const QString &dataset_name, DataType type, quint64 nb_points,
                      quint64 *points_coord, void *values = nullptr) override;

public:
    dtkIoDataModelHdf5Private *d;
};

// ///////////////////////////////////////////////////////////////////
// Creator
// ///////////////////////////////////////////////////////////////////

static dtkIoDataModel *dtkIoDataModelHdf5Creator(void)
{
    return new dtkIoDataModelHdf5();
}

//
// dtkIoDataModelHdf5.h ends here
