// dtkIoDataModelPHdf5.cpp ---
//
// Version:
//
//

// Commentary:
//
//

// Change log:
//
//

#include <QFileInfo>

#include <dtkDistributed/dtkDistributedCommunicator.h>

#include "dtkIoDataModelPHdf5.h"

#include "hdf5.h"
#include "mpi.h"

// other available types : H5T_NATIVE_HBOOL //hbool_t
//    H5T_NATIVE_FLOAT
//        H5T_NATIVE_B64  // 64-bit buffer in memory
//        H5T_C_S1  // c string size in bytes



// /////////////////////////////////////////////////////////////////
// dtkIoDataModelPHdf5Private
// /////////////////////////////////////////////////////////////////
class dtkIoDataModelPHdf5Private
{
public:
    bool file_is_open;
    hid_t file_id;
    hid_t prop_list_id; //property list to do collective write. set in fileOpen
    dtkDistributedCommunicator *comm;
    herr_t status;
    QString type;

public:
    QHash<QString, hid_t> dataset_hash; // hash of the open datasets
    QHash<QString, hid_t> group_hash; // hash of the opened groups

public:
    bool fileExists(const QString& file);
    hid_t& datasetId(const QString &dataset_name, const dtkIoDataModel::DataType& type, const int& dim, quint64 *shape);

};

bool dtkIoDataModelPHdf5Private::fileExists(const QString& file) {
    QFileInfo checkFile(file);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

// create or open a dataset and return an identifier to it
hid_t& dtkIoDataModelPHdf5Private::datasetId(const QString& dataset_name,
                                             const dtkIoDataModel::DataType& type=dtkIoDataModel::Int,
                                             const int& dim=-1, quint64 *shape=nullptr)
{
    if(!file_is_open) {
        dtkError() << "file is not open! ";
    }

    if(!dataset_hash.contains(dataset_name)) {
        if(dim>0) {
            if(!comm->wid())
                dtkInfo() << "Dataset not existing, creating DataSet" << dataset_name;

            QStringList dataset_name_split = dataset_name.split( "/" );
            //erase first (root) and last (dataset) elements to only keep groups
            dataset_name_split.pop_front();
            dataset_name_split.pop_back();

            if ( dataset_name_split.length() > 0) {
                //the dataset is in a group, we need to open or create groups
                QString path = "";
                for(auto it: dataset_name_split)
                {
                    path = path % "/" % it;
                    if(!group_hash.contains(path)) {
                        if(H5Lexists(file_id, path.toUtf8().constData(), H5P_DEFAULT))  //The group exists
                            group_hash[path] = H5Gopen(file_id, path.toUtf8().constData(), H5P_DEFAULT);
                        else
                            group_hash[path] = H5Gcreate(file_id, path.toUtf8().constData(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                    }
                }
            }

            //now create the dataset
            hid_t dataspace = H5Screate_simple(dim, shape, NULL);

            switch(type) {
            case dtkIoDataModel::Int:
            {
                dataset_hash[dataset_name] = H5Dcreate(file_id, dataset_name.toUtf8().constData(), H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                break;
            }
            case dtkIoDataModel::LongLongInt:
            {
                dataset_hash[dataset_name] = H5Dcreate(file_id, dataset_name.toUtf8().constData(), H5T_NATIVE_LLONG, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                break;
            }
            case dtkIoDataModel::Double:
            {
                dataset_hash[dataset_name] = H5Dcreate(file_id, dataset_name.toUtf8().constData(), H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                break;
            }
            default:
                dtkError() <<" datatype not supported";
            };

            H5Sclose(dataspace);
        }
        else {
            //we just want to open a dataset
            if(!comm->wid())
                dtkInfo() << "opening Dataset" << dataset_name;
            dataset_hash[dataset_name] = H5Dopen(file_id, dataset_name.toUtf8().constData(), H5P_DEFAULT);
        }

    }
    //dataset already opened

    return dataset_hash[dataset_name];
}


// /////////////////////////////////////////////////////////////////
// dtkIoDataModelPHdf5
// /////////////////////////////////////////////////////////////////
dtkIoDataModelPHdf5::dtkIoDataModelPHdf5(void) : dtkIoDataModel(), d(new dtkIoDataModelPHdf5Private)
{
    d->type = "PHDF5";
    d->comm = nullptr;
    d->file_is_open = false;
}

dtkIoDataModelPHdf5::~dtkIoDataModelPHdf5(void)
{
    if(d->file_is_open)
        this->fileClose();

    delete d;

    d = nullptr;
}

// const dtkIoDataModelPHdf5 *dtkIoDataModelPHdf5::clone(void)
// {
//     other = new dtkIoDataModelPHdf5(void);

//     return other;
// }

// void dtkIoDataModelPHdf5::copy(dtkIoDataModelPHdf5 *other)
// {
//     d = other->d;
// }

void dtkIoDataModelPHdf5::setCommunicator(dtkDistributedCommunicator *comm)
{
    if(d->file_is_open) {
        qDebug() << " ERROR set the communicator before opening a file in dtkIoDataModelPHdf5";
        dtkError() <<" ERROR set the communicator before opening a file in dtkIoDataModelPHdf5";
    }
    else
        d->comm = comm;
}

void dtkIoDataModelPHdf5::fileOpen(const QString &file_name, FileMode mode)
{
    //if file is already open tell it to the user and return
    if(!d->file_is_open) {

        //if we didn't set the communicator error
        if(d->comm==nullptr)
            dtkError() << __func__ << "communicator not set";

        // H5P_FILE_ACCESS applies to H5Fcreate and H5Fopen
        d->prop_list_id = H5Pcreate(H5P_FILE_ACCESS);

        MPI_Info info = MPI_INFO_NULL;
        MPI_Comm comm = *static_cast<MPI_Comm *>(d->comm->data());
        H5Pset_fapl_mpio(d->prop_list_id, comm, info);

        switch (mode) {
        case dtkIoDataModel::Trunc:
            d->file_id = H5Fcreate (file_name.toUtf8().constData(), H5F_ACC_TRUNC,
                                   H5P_DEFAULT, d->prop_list_id);

            break;
        case dtkIoDataModel::NotExisting:
            d->file_id = H5Fcreate(file_name.toUtf8().constData(), H5F_ACC_EXCL,
                                   H5P_DEFAULT, d->prop_list_id);
            break;
        case dtkIoDataModel::ReadOnly:
            d->file_id = H5Fopen(file_name.toUtf8().constData(), H5F_ACC_RDONLY,
                                 d->prop_list_id);
            break;
        case dtkIoDataModel::ReadWrite:
            d->file_id = H5Fopen(file_name.toUtf8().constData(), H5F_ACC_RDWR,
                                 d->prop_list_id);
            break;
        default:
            dtkError() << "unsupported fileMode";
        };

        //close the property list for file
        H5Pclose(d->prop_list_id);
        if(d->file_id<0) {
            dtkError() << "error in fileOpen for file_name " << file_name;
        }
        else {
            //if the file is correctly open, create a propery list to collectively write datasets
            d->file_is_open = true;
            d->prop_list_id = H5Pcreate(H5P_DATASET_XFER);
            H5Pset_dxpl_mpio(d->prop_list_id, H5FD_MPIO_COLLECTIVE);
        }
    }
    else {
        qDebug() << "File" << file_name << "is already open, please close it before opening a new one";
    }
}

void dtkIoDataModelPHdf5::fileClose(void)
{
    //close all the open datasets
    {
        QHash<QString, hid_t>::iterator it;
        for(it = d->dataset_hash.begin(); it != d->dataset_hash.end(); ++it) {
            dtkDebug() << "closing dataset" << it.key();
            H5Dclose(it.value());
        }
        d->dataset_hash.clear();
    }

    //close all the open groups
    {
        QHash<QString, hid_t>::iterator it;
        for(it = d->group_hash.begin(); it != d->group_hash.end(); ++it) {
            dtkDebug() << "closing group" << it.key();
            H5Gclose(it.value());
        }
        d->group_hash.clear();
    }

    H5Pclose(d->prop_list_id);
    H5Fclose(d->file_id);
    d->file_is_open=false;
}


void dtkIoDataModelPHdf5::read(const QString& dataset_name, DataType type, void *values)
{
    switch(type) {
    case dtkIoDataModel::Int:
    {
        H5Dread(d->datasetId(dataset_name), H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, values);
        break;
    }
    case dtkIoDataModel::LongLongInt:
    {
        H5Dread(d->datasetId(dataset_name), H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, values);
        break;
    }
    case dtkIoDataModel::Double:
    {
        H5Dread(d->datasetId(dataset_name), H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, values);
        break;
    }
    default:
        dtkError() << "read method: Datatype not supported";
    };
}




/*! write data in the open file
 * dimensions
 */
void dtkIoDataModelPHdf5::write(const QString& dataset_name, DataType type, int dimension, quint64 *shape, void *values)
{
    d->status=0;
    hid_t dataset_id = d->datasetId(dataset_name, type, dimension, shape);
    if(values) {
        switch(type) {
        case dtkIoDataModel::Int:
        {
            d->status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, d->prop_list_id ,values);
            break;
        }
        case dtkIoDataModel::LongLongInt:
        {
            d->status = H5Dwrite(dataset_id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, d->prop_list_id ,values);
            break;
        }
        case dtkIoDataModel::Double:
        {
            d->status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, d->prop_list_id ,values);
            break;
        }
        default:
            dtkError() << "write method: Datatype not supported";
        };
    }
    if(d->status<0)
        dtkError() << "error writing" << dataset_name;
}


//hyperslab write
void dtkIoDataModelPHdf5::writeHyperslab(const QString& dataset_name, DataType type, quint64 *offset, quint64 *stride, quint64 *count, quint64 *block, quint64 *values_shape, void *values)
{
    d->status=0;
    hid_t dataset_id = d->datasetId(dataset_name);

    //the selection within the file dataset's dataspace
    hid_t file_dataspace = H5Dget_space(dataset_id);

    if(H5Sselect_hyperslab(file_dataspace, H5S_SELECT_SET, offset, stride, count, block)<0)
        dtkError() << "ERROR selecting hyperslab" << dataset_name;

    //set the dimensions of values. memory dataspace and the selection within it
    hid_t values_dataspace = H5Screate_simple(H5Sget_simple_extent_ndims(file_dataspace),
                                              values_shape, NULL);

    switch(type) {
    case dtkIoDataModel::Int:
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_INT, values_dataspace,
                             file_dataspace, d->prop_list_id, values);
        break;
    case dtkIoDataModel::LongLongInt:
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_LLONG, values_dataspace,
                             file_dataspace, d->prop_list_id, values);
        break;
    case dtkIoDataModel::Double:
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, values_dataspace,
                             file_dataspace, d->prop_list_id, values);
        break;
    default:
        dtkError() << "write method: Datatype not supported";
    };
    if(d->status<0)
        dtkError() << "error writing hyperslab" << dataset_name;

    H5Sclose(file_dataspace);
    H5Sclose(values_dataspace);
}


//point selection write
// assuming dataset and values have the same shape and the points values are contiguous in values
void dtkIoDataModelPHdf5::writeByCoord(const QString &dataset_name, DataType type, quint64 nb_points, quint64* points_coord, void *values)
{
    d->status=0;
    hid_t dataset_id = d->datasetId(dataset_name);

    //the selection within the file dataset's dataspace
    hid_t file_dataspace = H5Dget_space(dataset_id);
    if(H5Sselect_elements(file_dataspace, H5S_SELECT_SET, nb_points, points_coord)<0)
        dtkError() << "ERROR selecting hyperslab" << dataset_name;

    //set the dimensions of values. memory dataspace and the selection within it
    hid_t values_dataspace = H5Screate_simple(1, &nb_points, NULL);

    switch(type) {
    case dtkIoDataModel::Int:
        // TODO put d->prop_list_id instead of H5P_DEFAULT ????????
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_INT, values_dataspace, file_dataspace, d->prop_list_id, values);
        break;
    case dtkIoDataModel::LongLongInt:
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_LLONG, values_dataspace, file_dataspace, d->prop_list_id, values);
        break;
    case dtkIoDataModel::Double:
        d->status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, values_dataspace, file_dataspace, d->prop_list_id, values);
        break;
    default:
        dtkError() << "write method: Datatype not supported";
    };
    if(d->status<0)
        dtkError() << "error writing hyperslab" << dataset_name;

    H5Sclose(file_dataspace);
    H5Sclose(values_dataspace);
}
