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

class QWidget;

class DTKTHEMES_EXPORT dtkThemesEngine : public QObject
{
    Q_OBJECT

public:
    static dtkThemesEngine *instance(void);

signals:
    void changed(void);

public:
    const QStringList themes(void);

public slots:
    void apply(const QString& theme = QString());

public slots:
    void polish(QWidget *);

public:
    const QColor  color(const QString& key) const;
    const QString value(const QString& key) const;

protected:
    static dtkThemesEngine *s_instance;

private:
     dtkThemesEngine(QObject *parent = nullptr);
    ~dtkThemesEngine(void);

private:
    class dtkThemesEnginePrivate *d;
};

//
// dtkThemesEngine.h ends here
