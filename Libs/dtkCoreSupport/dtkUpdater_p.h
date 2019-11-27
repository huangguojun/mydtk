/* dtkUpdater_p.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Jul 20 12:41:15 2009 (+0200)
 * Version: $Id: 993d32b2661c7c7b0bf741b0170f9adcee9ecebc $
 * Last-Updated: mar. févr.  4 11:20:02 2014 (+0100)
 *           By: Nicolas Niclausse
 *     Update #: 10
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKUPDATER_P_H
#define DTKUPDATER_P_H

//  W A R N I N G
//
// This file is not part of the dtk API.  It exists for the convenience
// of dtkUpdater. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtCore>
#include <QtNetwork>

class dtkUpdaterPrivate : public QObject
{
    Q_OBJECT

public:
    void downl(const QUrl& url);
    void extract(void);

public slots:
    void onRequestFinished(QNetworkReply *reply);

public:
    QNetworkAccessManager *http;
    QFile *cfgFile;
    QFile *binFile;
    QUrl cfgUrl;
    int binId;
    QUrl binUrl;
};

#endif
