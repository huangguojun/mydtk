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

#include <dtkFontsExport>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace ai {

  enum DTKFONTS_EXPORT icon {
     apache = 0xe909,
     atom = 0xe917,
     aws = 0xe90c,
     bower = 0xe918,
     c = 0xe915,
     cline = 0xe90f,
     clojure = 0xe919,
     clojureline = 0xe91a,
     coffeescript = 0xe914,
     cplusplus = 0xe913,
     cplusplusline = 0xe910,
     csharp = 0xe911,
     csharpline = 0xe912,
     css3 = 0xe91b,
     css3alt = 0xe91c,
     d3 = 0xe90e,
     dlang = 0xe935,
     elixir = 0xe936,
     erlang = 0xe934,
     git = 0xe907,
     go = 0xe91d,
     googledrive = 0xe91e,
     grunt = 0xe90d,
     gruntline = 0xe91f,
     gulp = 0xe920,
     haskell = 0xe921,
     html5 = 0xe932,
     jasmine = 0xe904,
     java = 0xe922,
     javascript = 0xe906,
     javascriptbadge = 0xe923,
     javascriptshield = 0xe924,
     less = 0xe90b,
     nginx = 0xe933,
     nodejs = 0xe925,
     perl = 0xe905,
     perldocs = 0xe926,
     postgresql = 0xe938,
     prolog = 0xe927,
     python = 0xe928,
     react = 0xe929,
     ruby = 0xe92a,
     rubyalt = 0xe92b,
     rust = 0xe92c,
     sass = 0xe92d,
     scala = 0xe908,
     script = 0xe90a,
     spring = 0xe937,
     stylus = 0xe92e,
     svg = 0xe903,
     dtkVisualizationift = 0xe92f,
     terminal = 0xe930,
     terminalalt = 0xe931,
     batterycharging = 0xe939
  };
}

// ///////////////////////////////////////////////////////////////////
// dtkFontAllTheIconsIconPainter
// ///////////////////////////////////////////////////////////////////

class dtkFontAllTheIconsIconPainter;

// ///////////////////////////////////////////////////////////////////
// dtkFontAllTheIcons
// ///////////////////////////////////////////////////////////////////

class DTKFONTS_EXPORT dtkFontAllTheIcons : public QObject
{
    Q_OBJECT

public:
    static dtkFontAllTheIcons *instance(void);

protected:
    static dtkFontAllTheIcons *s_instance;

private:
    explicit dtkFontAllTheIcons(QObject *parent = 0);
    virtual ~dtkFontAllTheIcons(void);

public:
    void init(const QString& fontname);
    bool initFontAllTheIcons(void);

    void setDefaultOption(const QString& name, const QVariant& value);
    QVariant defaultOption(const QString& name);

    QIcon icon(int character, const QVariantMap& options = QVariantMap());
    QIcon icon(dtkFontAllTheIconsIconPainter *painter, const QVariantMap& optionMap = QVariantMap());

    void give(const QString& name, dtkFontAllTheIconsIconPainter *painter);

    QFont font(int size);

    QString fontName(void) { return fontName_ ; }

private:
    QString fontName_;
    QHash<QString, dtkFontAllTheIconsIconPainter*> painterMap_;
    QVariantMap defaultOptions_;
    dtkFontAllTheIconsIconPainter* fontIconPainter_;
};

// ///////////////////////////////////////////////////////////////////
// dtkFontAllTheIconsIconPainter
// ///////////////////////////////////////////////////////////////////

class DTKFONTS_EXPORT dtkFontAllTheIconsIconPainter
{
public:
    virtual ~dtkFontAllTheIconsIconPainter(void) {}

public:
    virtual void paint(dtkFontAllTheIcons *awesome, QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options) = 0;
};

//
// dtkFontAllTheIconsIcons.h ends here
