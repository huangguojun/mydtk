// dtkIoDataModelHdf5.cpp --- 
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

#include "dtkIoDataModelHdf5.h"

#include "H5Cpp.h"

// other available types : H5T_NATIVE_HBOOL //hbool_t
//    H5T_NATIVE_FLOAT
//        H5T_NATIVE_B64  // 64-bit buffer in memory
//        H5T_C_S1  // c string size in bytes



// /////////////////////////////////////////////////////////////////
// dtkIoDataModelHdf5Private
// /////////////////////////////////////////////////////////////////
class dtkIoDataModelHdf5Private
{
public:
    H5::H5File *file;
    herr_t status;
    QString type;

public:
    QHash<QString, H5::DataSet *> dataset_hash; // hash of the open datasets
    QHash<QString, H5::Group *> group_hash; // hash of the opened groups
    
public:
    bool fileExists(const QString& file);
    H5::DataSet* openDataset(const QString& dataset_name);
    H5::DataSet* createDataset(const QString &dataset_name, const dtkIoDataModel::DataType& type,
                               const int& dim, quint64 *shape);
    
};

bool dtkIoDataModelHdf5Private::fileExists(const QString& file) {
    QFileInfo checkFile(file);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

//return the dataset to read from if it exist in the file
H5::DataSet* dtkIoDataModelHdf5Private::openDataset(const QString& dataset_name) {
    
    if(file == nullptr)
        dtkError() << "file is not open! ";

    H5::DataSet *dataset;
    //check if the dataset is open. if not, open it
    if(!dataset_hash.contains(dataset_name)) {
        dtkInfo() << "opening dataset" << dataset_name;
        //open the dataset
        dataset = new H5::DataSet(file->openDataSet(dataset_name.toUtf8().constData()));
        
        if(dataset->getId()>0)
            dataset_hash[dataset_name] = dataset;
        else
            dtkError() << "Dataset" << dataset_name << "not existing, in file";
    }
    
    return dataset_hash[dataset_name];
}

// create a dataset or return the identifier if we already created it
H5::DataSet* dtkIoDataModelHdf5Private::createDataset(const QString& dataset_name,
                                                     const dtkIoDataModel::DataType& type,
                                                     const int& dim, quint64 *shape)
{
    if(file == nullptr) {
        dtkError() << "file is not open! ";
    }

    H5::DataSet *dataset;
    if(!dataset_hash.contains(dataset_name)) {
        dtkInfo() << "Dataset not existing, creating DataSet" << dataset_name;

        QStringList dataset_name_split = dataset_name.split( "/" );
        //erase first (root) and last (dataset) elements to only keep groups
        dataset_name_split.pop_front();
        dataset_name_split.pop_back();
        
        if ( dataset_name_split.length() > 0) {
            //the dataset is in a group, we need to open or create groups
            QString path = "/";
            for(auto it: dataset_name_split)
            {
                path = path % it;
                if(!group_hash.contains(path)) {
                    if(H5Lexists(file->getId(), path.toUtf8().constData(), H5P_DEFAULT))  //The group exists
                        group_hash[path] = new H5::Group (file->openGroup(path.toUtf8().constData()));
                    else
                        group_hash[path] = new H5::Group (file->createGroup(path.toUtf8().constData()));
                }
            }
        }

        //now create the dataset
        //TODO a cast ?
//        hsize_t h_shape[dim];
//        for(int i=0; i<dim; ++i)
//            h_shape[i]=shape[i];
        H5::DataSpace dataspace(dim, shape);
        
        switch(type) {
        case dtkIoDataModel::Int:
        {
            H5::DataType datatype( H5::PredType::NATIVE_INT);            
            //datatype.setOrder(H5T_ORDER_LE); //little endian order
            dataset_hash[dataset_name] = new H5::DataSet(file->createDataSet(dataset_name.toUtf8().constData(), datatype, dataspace));
            break;
        }
        case dtkIoDataModel::LongLongInt:
        {
            H5::DataType datatype( H5::PredType::NATIVE_LLONG);            
            //datatype.setOrder(H5T_ORDER_LE); //little endian order
            dataset_hash[dataset_name] =  new H5::DataSet(file->createDataSet(dataset_name.toUtf8().constData(), datatype, dataspace));
            break;
        }
        case dtkIoDataModel::Double:
        {
            H5::DataType datatype( H5::PredType::NATIVE_DOUBLE);            
            //datatype.setOrder(H5T_ORDER_LE); //little endian order
            dataset_hash[dataset_name] =  new H5::DataSet(file->createDataSet(dataset_name.toUtf8().constData(), datatype, dataspace));
            break;
        }
        default:
            dtkError() <<" datatype not supported";
        };       
    }
    
    return dataset_hash[dataset_name];
}

// /////////////////////////////////////////////////////////////////
// dtkIoDataModelHdf5
// /////////////////////////////////////////////////////////////////
dtkIoDataModelHdf5::dtkIoDataModelHdf5(void) : dtkIoDataModel(), d(new dtkIoDataModelHdf5Private)
{
    d->type = "HDF5";
    d->file = nullptr;
}

dtkIoDataModelHdf5::~dtkIoDataModelHdf5(void)
{
    if(d->file != nullptr)
        this->fileClose();
    
    delete d;

    d = NULL;
}

// const dtkIoDataModelHdf5 *dtkIoDataModelHdf5::clone(void)
// {
//     other = new dtkIoDataModelHdf5(void);

//     return other;
// }

// void dtkIoDataModelHdf5::copy(dtkIoDataModelHdf5 *other)
// {
//     d = other->d;
// }


void dtkIoDataModelHdf5::fileOpen(const QString &file_name, const dtkIoDataModel::FileMode& mode)
{
    //if file is already open tell it to the user and return 
    if(d->file == nullptr) {
        switch (mode) {
        case dtkIoDataModel::Trunc:
            d->file = new H5::H5File(file_name.toUtf8().constData(), H5F_ACC_TRUNC);
            break;
        case dtkIoDataModel::NotExisting:
            d->file = new H5::H5File(file_name.toUtf8().constData(), H5F_ACC_EXCL);
            break;
        case dtkIoDataModel::ReadOnly:
            d->file = new H5::H5File(file_name.toUtf8().constData(), H5F_ACC_RDONLY);
            break;
        case dtkIoDataModel::ReadWrite:
            d->file = new H5::H5File(file_name.toUtf8().constData(), H5F_ACC_RDWR);
            break;
        default:
            dtkError() << "unsupported fileMode";
        };

        if(!d->file) {
            dtkError() << "error in fileOpen for file_name " << file_name;
        }
    }
    else {
        qDebug() << "File" << file_name << "is already open, please close it before opening a new one";
    }
}

void dtkIoDataModelHdf5::fileClose(void)
{
    //close all the open datasets
    {
        QHash<QString, H5::DataSet *>::iterator it;
        for(it = d->dataset_hash.begin(); it != d->dataset_hash.end(); ++it) {
            qDebug() << "closing dataset" << it.key();
            it.value()->close();
        }
        d->dataset_hash.clear();
    }
    
    //close all the open groups
    {
        QHash<QString, H5::Group *>::iterator it;
        for(it = d->group_hash.begin(); it != d->group_hash.end(); ++it) {
            qDebug() << "closing group" << it.key();
        it.value()->close();
        }
        d->group_hash.clear();
    }
    
//    for(auto it: d->dataset_hash.keys()) {
//        qDebug() << "closing dataset" << it;
//        H5Dclose(d->dataset_hash.value(it));
//    }
    
    d->file->close();
    delete d->file;
    d->file = nullptr;
}


void dtkIoDataModelHdf5::read(const QString &dataset_name, const dtkIoDataModel::DataType& type, void *values)
{
    switch(type) {
    case dtkIoDataModel::Int:
    {
        H5::DataType datatype( H5::PredType::NATIVE_INT);      
        d->openDataset(dataset_name)->read(values, datatype);
        break;
    }
    case dtkIoDataModel::LongLongInt:
    {
        H5::DataType datatype( H5::PredType::NATIVE_LLONG);
        d->openDataset(dataset_name)->read(values, datatype);
        break;
    }
    case dtkIoDataModel::Double:
    {
        H5::DataType datatype( H5::PredType::NATIVE_DOUBLE);            
        d->openDataset(dataset_name)->read(values, datatype);
        break;
    }
    default:
        dtkError() << "read method: Datatype not supported";
    };
}


                                  

/*! write data in the open file
 * dimensions 
 */
void dtkIoDataModelHdf5::write(const QString& dataset_name, const dtkIoDataModel::DataType& type,
                               const int& dimension, quint64 *shape, void *values)
{
    switch(type) {
    case dtkIoDataModel::Int:
    {
        H5::DataType datatype( H5::PredType::NATIVE_INT);      
        d->createDataset(dataset_name, type, dimension, shape)->write(values, datatype);
        break;
    }
    case dtkIoDataModel::LongLongInt:
    {
        H5::DataType datatype( H5::PredType::NATIVE_LLONG);
        d->createDataset(dataset_name, type, dimension, shape)->write(values, datatype);
        break;
    }
    case dtkIoDataModel::Double:
    {
        H5::DataType datatype( H5::PredType::NATIVE_DOUBLE);            
        d->createDataset(dataset_name, type, dimension, shape)->write(values, datatype);
        break;
    }
    default:
        dtkError() << "write method: Datatype not supported";
    };
}

//hyperslab write
void dtkIoDataModelHdf5::writeHyperslab(const QString &dataset_name, const dtkIoDataModel::DataType& type, quint64 *offset, quint64 *stride, quint64 *count, quint64 *block, quint64 *values_shape, void *values)
{   
    H5::DataSet *dataset = d->openDataset(dataset_name);

    //the selection within the file dataset's dataspace
    H5::DataSpace dataspace = dataset->getSpace();
	dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

    //set the dimensions of values. memory dataspace and the selection within it
    H5::DataSpace memspace(dataspace.getSimpleExtentNdims(), values_shape);    

    switch(type) {
    case dtkIoDataModel::Int:
        dataset->write(values, H5::PredType::NATIVE_INT, memspace, dataspace);
        break;
    case dtkIoDataModel::LongLongInt:
        dataset->write(values, H5::PredType::NATIVE_LLONG, memspace, dataspace);
        break;
    case dtkIoDataModel::Double:
        dataset->write(values, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
        break;
    default:
        dtkError() << "write method: Datatype not supported";
    };
        
       
}


//point selection write
// assuming dataset and values have the same shape and the points values are contiguous in values
void dtkIoDataModelHdf5::writeByCoord(const QString &dataset_name, const dtkIoDataModel::DataType& type, const quint64& nb_points, quint64* points_coord, void *values) 
{   
    H5::DataSet *dataset = d->openDataset(dataset_name);

    //the selection within the file dataset's dataspace
    H5::DataSpace file_dataspace = dataset->getSpace();
    file_dataspace.selectElements( H5S_SELECT_SET, nb_points, points_coord);

    //set the dimensions of values. memory dataspace and the selection within it
    H5::DataSpace values_dataspace(1, &nb_points);    

    switch(type) {
    case dtkIoDataModel::Int:
        dataset->write(values, H5::PredType::NATIVE_INT, values_dataspace, file_dataspace);
        break;
    case dtkIoDataModel::LongLongInt:
        dataset->write(values, H5::PredType::NATIVE_LLONG, values_dataspace, file_dataspace);
        break;
    case dtkIoDataModel::Double:
        dataset->write(values, H5::PredType::NATIVE_DOUBLE, values_dataspace, file_dataspace);
        break;
    default:
        dtkError() << "write method: Datatype not supported";
    };
}

