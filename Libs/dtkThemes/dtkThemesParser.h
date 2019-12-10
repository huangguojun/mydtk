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

#include <dtkThemesExport>

#include <QtCore>
#include <QtGui>

class DTKTHEMES_EXPORT dtkThemesParser : public QObject
{
    Q_OBJECT

public:
     dtkThemesParser(QObject *parent = nullptr);
     dtkThemesParser(const QString& path, QObject *parent = nullptr);
    ~dtkThemesParser(void);

public slots:
    QString parse(const QString&);

public:
    const QColor color(const QString& key) const;

public:
    const QHash<QString, QString>& values(void) const;

private:
    class dtkThemesParserPrivate *d;
};

//
// dtkThemesParser.h ends here
