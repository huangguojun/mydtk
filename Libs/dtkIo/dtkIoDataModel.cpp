// dtkIoDataModel.cpp ---
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

#include "dtkIoDataModel.h"

// /////////////////////////////////////////////////////////////////
// dtkIoDataModel implementation
// /////////////////////////////////////////////////////////////////

/*!
  \class dtkIoDataModel
  \inmodule dtkIo
  \brief dtkIoDataModel is the interface for input/output Data Model (ASCII,
  Hdf5, ..).

  long description

  \code
  dtkIoDataModel *data_model = dtkIo::dataModel::pluginFactory().create("Hdf5");
  QString file_name = "fileCreationTest.h5";
  data_model->fileOpen(file_name, dtkIoDataModel::Trunc);
  data_model->fileClose();
  \endcode

*/

/*! \enum dtkIoDataModel::FileMode

    This enum is used with fileOpen() to describe the mode in which a
    file is opened.

    \value Trunc       If the file already exists, the file is opened with
                       read-write access, and new data will overwrite any
                       existing data. Else, it is created and opened with
                       read-write access.

    \value NotExisting If the file already exists, openFile() will
                       fail. Else, it is created and opened with
                       read-write access.

    \value ReadOnly    An existing file is opened with read-only access.
                       If the file does not exist, openFile() fails.

    \value ReadWrite   An existing file is opened with read-write access.
                       If the file does not exist, openFile() fails.
*/

/*! \enum dtkIoDataModel::DataType

    These are the buit-in types supported by dtkIoDataModel.

    \value Int         \c int

    \value LongLongInt \c {Long long int}

    \value Double      \c double
*/

/*!
   Constructor.
*/
dtkIoDataModel::dtkIoDataModel(void) : QObject() {}

/*!
   Destructor.
*/
dtkIoDataModel::~dtkIoDataModel(void) {}

/*! \fn void dtkIoDataModel::setCommunicator(dtkDistributedCommunicator *comm)

  When dealing with parallel IO, this methods enables to set the IO communicator
  \a comm.

*/

/*! \fn void dtkIoDataModel::fileOpen(const QString& name, FileMode mode)

  Opens a file \a name using the fileMode \a mode.

  \sa fileCose

 */

/*! \fn void dtkIoDataModel::fileClose(void)

   Closes the open file.

   \sa fileOpen
*/

/*! \fn void dtkIoDataModel::read(const QString& dataset_name, DataType type,
  void *values)

  Read a dataset from its name \a dataset_name and \a type into buffer
  \a values. Buffer \a values has to be allocated before

*/

/*! \fn void dtkIoDataModel::write(const QString& dataset_name, DataType type,
  int dimension, quint64 *shape, void *values)

  Writes a dataset from its name \a dataset_name and type \a type into the open
  file. If you call this function with nullptr in \a values, it will create a
  dataset but will not write any data in it. \list \li \a type : type of data
  \li \a dimension : number of dimensions of the data to be written and
  dimension of shape \li \a shape : number of elements in each dimension : ex \c
  {dimension = 2} , \c {shape = {3,4} }. \li \a values : data to be written.
  \endlist

  \sa writeHyperslab, writeByCoord

*/

/*!

  Read using coordinates.
  \list
  \li \a dataset_name : name of the dataset
  \li \a type : type of the dataset
  \li \a nb_points : number of values to write
  \li \a points_coord : pointer to a 2D array \c {[NB_POINTS][DIMENSIONS]}. \c
  {points_coord[0][0] = 1}, \c {points_coord[0][1] = 2} means that the point \c
  {0} to write have coordinates \c {{1,2}} in the file \li \a values : buffer
  for values to read . Values need to be contiguous \endlist

  \sa write, writeHyperslab
*/
void dtkIoDataModel::readByCoord(const QString &dataset_name, DataType type, quint64 nb_points,
                                 quint64 *points_coord, void *values)
{
    qDebug() << Q_FUNC_INFO << "Not implemented.";
}

/*!

  Writes using a hyperslab but doesn't support overlapping blocks!

  Writes a subset of a dataset from its name \a dataset_name and \a
  type into an existing dataset in the open file. \a count, \a
  offset, \a stride and \a block must be of the same size as the
  dimension of the dataspace. if \c {dim = 2} , each of these
  parameters must be a 1-dimension array of size 2.

  \list
  \li \a offset : offset of the starting element
  \li \a stride : determine how many elements to move from between blocks to
  write. Stride with value \c {1} means that all elements are contiguous, while
  \c {2} skips one element out of two. \li \a count : number of blocks to select
  from the dataspace in each dimension \li \a block : array to determine the
  size of the element block selected from the dataspace \li \a values_shape :
  number of elements in each dimension of values. Must be of the dimension of
  the dataset dataspace \li \a values : values to be written \endlist

  \sa write, writeByCoord

*/
void dtkIoDataModel::writeHyperslab(const QString &dataset_name, DataType type, quint64 *offset,
                                    quint64 *stride, quint64 *count, quint64 *block,
                                    quint64 *values_shape, void *values)
{
    qDebug() << Q_FUNC_INFO << "Not implemented.";
}

/*!

  Writes using coordinates.
  \list
  \li \a dataset_name : name of the dataset
  \li \a type : type of the dataset
  \li \a nb_points : number of values to write
  \li \a points_coord : pointer to a 2D array \c {[NB_POINTS][DIMENSIONS]}. \c
  {points_coord[0][0] = 1}, \c {points_coord[0][1] = 2} means that the point \c
  {0} to write have coordinates \c {{1,2}} in the file \li \a values : values to
  write . Values to write need to be contiguous \endlist

  \sa write, writeHyperslab
*/
void dtkIoDataModel::writeByCoord(const QString &dataset_name, DataType type, quint64 nb_points,
                                  quint64 *points_coord, void *values)
{
    qDebug() << Q_FUNC_INFO << "Not implemented.";
}
