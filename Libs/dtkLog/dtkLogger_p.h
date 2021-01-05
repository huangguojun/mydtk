/* dtkLogger_p.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Thu Mar  1 17:26:54 2012 (+0100)
 * Version: $Id: 6994c18ecbe75c1d832eff193bf9bafa724017a6 $
 * Last-Updated: Mon Mar 18 12:25:46 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 50
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKLOGGER_P_H
#define DTKLOGGER_P_H

#include "dtkLog.h"
#include "dtkLogDestination.h"

#include <QtCore>

#include <iostream>
#include <streambuf>

class dtkLogViewList;

// ///////////////////////////////////////////////////////////////////////
class redirectStream : public std::basic_streambuf<char>
{
public:
    redirectStream(std::ostream &stream, dtkLog::Level level) : m_stream(stream), m_level(level)
    {
        m_old_buf = stream.rdbuf();
        stream.rdbuf(this);
    }

    ~redirectStream() { m_stream.rdbuf(m_old_buf); }

protected:
    // This is called when a std::endl has been inserted into the stream
    virtual int_type overflow(int_type v)
    {
        if (v == '\n') {
            dtkLog(m_level) << "";
        }

        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {
        QString str(p);
        dtkLog(m_level) << str;
        return n;
    }

private:
    std::ostream &m_stream;
    std::streambuf *m_old_buf;
    dtkLog::Level m_level;
};

class dtkLoggerPrivate
{
public:
    dtkLog::Level level;
    QHash<dtkLogDestinationPointer, dtkLog::Level> levels;

public:
    dtkLogDestinationPointer console;
    QHash<QString, dtkLogDestinationPointer> files;
    QHash<dtkLogModel *, dtkLogDestinationPointer> models;

public:
    QList<dtkLogDestinationPointer> destinations;

public:
    QMutex mutex;

public:
    redirectStream *cerr_stream;
    redirectStream *cout_stream;
};

#endif
