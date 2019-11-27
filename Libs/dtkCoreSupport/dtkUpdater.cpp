/* dtkUpdater.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jul 20 11:15:27 2009 (+0200)
 * Version: $Id: fe4ad51f24ef3e23493e389c81769c7af1cc03d5 $
 * Last-Updated: mar. févr.  4 11:30:25 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 50
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <QtNetwork>

#include "dtkGlobal.h"
#include "dtkUpdater_p.h"
#include "dtkUpdater.h"

// /////////////////////////////////////////////////////////////////
// dtkUpdaterPrivate
// /////////////////////////////////////////////////////////////////

void dtkUpdaterPrivate::onRequestFinished(QNetworkReply *reply)
{

    if (reply->url() == cfgUrl) { // WARN: the url may not be the same ...

        if (!cfgFile->open(QFile::ReadWrite)) {
            qDebug() << "Unable to open config file  for writing.";
            return;
        }

        cfgFile->write(reply->readAll());
        cfgFile->flush();
        cfgFile->close();

        if (!cfgFile->openMode() == QIODevice::NotOpen)
            cfgFile->close();

        if (!cfgFile->open(QFile::ReadOnly | QFile::Text))
            qDebug() << "Unable to open stream for reading.";

        QXmlStreamReader reader(cfgFile);

        while (!reader.atEnd()) {
            reader.readNext();

            if (reader.isStartElement() && reader.attributes().hasAttribute("version") && reader.attributes().value("version").toString() > qApp->applicationVersion()) {
                reader.readNext();

                if (reader.isCharacters()) {
                    binUrl.setUrl(reader.text().toString());
                }
            }
        }

        if (!cfgFile->openMode() == QIODevice::NotOpen)
            cfgFile->close();

        if (reader.error())
            qDebug() << reader.error() << reader.errorString();

        if (binUrl.isEmpty()) {
            qDebug() << "You are up to date at version" << qApp->applicationVersion();
            return;
        } else {
            qDebug() << "Updates are available (you have " << qApp->applicationVersion() << "), would you like to download ?";
        }

        char c = getchar(); getchar();

        if (c == 'y')
            downl(binUrl);

    }  else {

        binFile->write(reply->readAll());
        binFile->flush();
        binFile->close();

        qDebug() << "Download completed, would you like to install ?";

        char c = getchar(); getchar();

        if (c == 'y')
            extract();
    }
}

void dtkUpdaterPrivate::downl(const QUrl& url)
{
    if (!binFile->open(QFile::ReadWrite))
        qDebug() << "Unable to open binary file for saving";

    http->get(QNetworkRequest(url));

}

void dtkUpdaterPrivate::extract(void)
{
    QProcess process;
#if defined (Q_OS_UNIX)
    QString input = "/tmp/bin";
    QString output = qApp->applicationDirPath();
# if defined (Q_OS_MAC)
    output += "/../../..";
# endif
    process.start("tar", QStringList() << "-xzf" << input << "-C" << output);
#else
    ;
#endif

    if (process.waitForFinished())
        qDebug() << "Updates have been installed successfully";
}

// /////////////////////////////////////////////////////////////////
// dtkUpdater
// /////////////////////////////////////////////////////////////////

dtkUpdater::dtkUpdater(const QUrl& cfgUrl)
{
    QFile::remove("/tmp/cfg");
    QFile::remove("/tmp/bin");

    d          = new dtkUpdaterPrivate;
    d->http    = new QNetworkAccessManager;
    d->cfgFile = new QFile("/tmp/cfg");
    d->binFile = new QFile("/tmp/bin");

    QObject::connect(d->http, SIGNAL(finished(QNetworkReply *)), d, SLOT(onRequestFinished(QNetworkReply *)));

    d->http->get(QNetworkRequest(cfgUrl));
}

dtkUpdater::~dtkUpdater(void)
{
    QFile::remove("/tmp/cfg");
    QFile::remove("/tmp/bin");
}
