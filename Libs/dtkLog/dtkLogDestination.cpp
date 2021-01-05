// Version: $Id: 9a3eee85b6827bde1c735f5d7d341ac3656c53d4 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkLogDestination.h"
#include "dtkLogModel.h"

// /////////////////////////////////////////////////////////////////
// dtkLogDestination
// /////////////////////////////////////////////////////////////////

dtkLogDestination::~dtkLogDestination(void) {}

// /////////////////////////////////////////////////////////////////
// dtkLogDestinationConsole
// /////////////////////////////////////////////////////////////////

void dtkLogDestinationConsole::write(const QString &message)
{
    fprintf(stderr, "%s\n", qPrintable(message));
    fflush(stderr);
}

// /////////////////////////////////////////////////////////////////
// dtkLogDestinationFile
// /////////////////////////////////////////////////////////////////

class dtkLogDestinationFilePrivate
{
public:
    QFile file;
    qlonglong max_file_size;

public:
    QTextStream stream;
};

dtkLogDestinationFile::dtkLogDestinationFile(const QString &path, qlonglong max_file_size)
    : d(new dtkLogDestinationFilePrivate)
{
    d->file.setFileName(path);
    d->max_file_size = max_file_size;

    QFileInfo info(path);

    QDir dir(info.absoluteDir());

    if (!dir.exists()) {
        QString name = dir.path();
        dir.mkpath(name);
    }

    if (!d->file.open(QFile::WriteOnly | QFile::Text | QIODevice::Append))
        qDebug() << "Unable to open" << path << "for writing";

    d->stream.setDevice(&(d->file));
}

dtkLogDestinationFile::~dtkLogDestinationFile(void)
{
    delete d;

    d = NULL;
}

void dtkLogDestinationFile::setMaxFileSize(qlonglong size)
{
    d->max_file_size = size;
}

void dtkLogDestinationFile::write(const QString &message)
{
    if (d->file.size() + message.size() > d->max_file_size) {
        qDebug() << "Max log file size reached" << d->max_file_size << ", rotate log file";
        d->file.flush();
        d->file.close();
        QString path = d->file.fileName();
        QString backup = path + ".old";

        // remove old backup
        if (QFile::exists(backup))
            QFile::remove(backup);

        QFile::rename(path, backup);

        if (!d->file.open(QFile::WriteOnly | QFile::Text | QIODevice::Append))
            qDebug() << "Unable to open" << path << "for writing";
    }

    d->stream << message << endl;
    d->stream.flush();
}

// /////////////////////////////////////////////////////////////////
// dtkLogDestinationModel
// /////////////////////////////////////////////////////////////////

class dtkLogDestinationModelPrivate
{
public:
    dtkLogModel *model;
};

dtkLogDestinationModel::dtkLogDestinationModel(dtkLogModel *model)
    : d(new dtkLogDestinationModelPrivate)
{
    d->model = model;
}

dtkLogDestinationModel::~dtkLogDestinationModel(void)
{
    delete d;

    d = NULL;
}

void dtkLogDestinationModel::write(const QString &message)
{
    d->model->append(message);
}

//
// dtkLogDestination.cpp ends here
