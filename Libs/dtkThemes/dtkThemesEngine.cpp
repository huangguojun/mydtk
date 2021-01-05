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

#include "dtkThemesEngine.h"
#include "dtkTheme.h"
#include "dtkThemesParser.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkThemesEnginePrivate
{
public:
    dtkTheme *theme = nullptr;
    dtkThemesParser *parser = nullptr;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkThemesEngine *dtkThemesEngine::instance(void)
{
    if (!s_instance)
        s_instance = new dtkThemesEngine;

    return s_instance;
}

const QStringList dtkThemesEngine::themes(void)
{
    QDir ressources(":dtk-themes/mixins");

    return ressources.entryList(QStringList() << "*.qless")
            .replaceInStrings("dtkTheme", "")
            .replaceInStrings(".qless", "");
}

void dtkThemesEngine::apply(const QString &theme)
{
    if (!d->parser)
        d->parser = new dtkThemesParser(this);

    QString chosen = theme;

    if (chosen.isEmpty()) {

        QSettings settings;

        chosen = settings.value("theme", this->themes().first()).toString();
    }

    QString sheet = d->parser->parse(QString(":dtk-themes/mixins/dtkTheme%1.qless").arg(chosen));

    d->theme = new dtkTheme(sheet);

    qApp->setStyle(d->theme);

    {
        QSettings settings;

        settings.setValue("theme", chosen);
    }

    emit changed();
}

void dtkThemesEngine::polish(QWidget *w)
{
    if (!d->theme)
        return;

    d->theme->touch(w);
}

const QColor dtkThemesEngine::color(const QString &key) const
{
    return d->parser->color(key);
}

const QString dtkThemesEngine::value(const QString &key) const
{
    return d->parser->values()[key];
}

dtkThemesEngine *dtkThemesEngine::s_instance = nullptr;

dtkThemesEngine::dtkThemesEngine(QObject *parent) : QObject(parent)
{
    d = new dtkThemesEnginePrivate;
}

dtkThemesEngine::~dtkThemesEngine(void)
{
    delete d;
}

//
// dtkThemesEngine.cpp ends here
