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

#include "dtkTheme.h"
#include "dtkThemesEngine.h"

#include <QtGui>

#include <dtkFonts>

// /////////////////////////////////////////////////////////////////////////////
// dtkThemePrivate
// /////////////////////////////////////////////////////////////////////////////

class dtkThemePrivate
{
public:
    QString sheet;
};

// /////////////////////////////////////////////////////////////////////////////
// dtkTheme
// /////////////////////////////////////////////////////////////////////////////

dtkTheme::dtkTheme(const QString &sheet) : dtkTheme(sheet, dtkTheme::styleBase()) {}

dtkTheme::dtkTheme(const QString &sheet, QStyle *style) : QProxyStyle(style)
{
    d = new dtkThemePrivate;
    d->sheet = sheet;
}

void dtkTheme::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option,
                             QPainter *painter, const QWidget *widget) const
{
    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QStyle *dtkTheme::styleBase(QStyle *style) const
{
    static QStyle *base = !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;

    return base;
}

QStyle *dtkTheme::baseStyle(void) const
{
    return styleBase();
}

void dtkTheme::polish(QPalette &palette)
{
    palette.setColor(QPalette::Window, dtkThemesEngine::instance()->color("@bg"));
    palette.setColor(QPalette::WindowText, dtkThemesEngine::instance()->color("@fg"));

    palette.setColor(QPalette::Background, dtkThemesEngine::instance()->color("@bg"));
    palette.setColor(QPalette::Foreground, dtkThemesEngine::instance()->color("@fg"));

    palette.setColor(QPalette::Base, dtkThemesEngine::instance()->color("@bgalt"));
    palette.setColor(QPalette::AlternateBase,
                     dtkThemesEngine::instance()->color("@bgalt").lighter());
    palette.setColor(QPalette::Text, dtkThemesEngine::instance()->color("@fg"));

    palette.setColor(QPalette::ToolTipBase, dtkThemesEngine::instance()->color("@base2"));
    palette.setColor(QPalette::ToolTipText, dtkThemesEngine::instance()->color("@base8"));

    palette.setColor(QPalette::Button, dtkThemesEngine::instance()->color("@base1"));
    palette.setColor(QPalette::ButtonText, dtkThemesEngine::instance()->color("@base8"));

    palette.setColor(QPalette::BrightText, dtkThemesEngine::instance()->color("@red"));

    palette.setColor(QPalette::Light, dtkThemesEngine::instance()->color("@base4"));
    palette.setColor(QPalette::Midlight, dtkThemesEngine::instance()->color("@base3"));
    palette.setColor(QPalette::Dark, dtkThemesEngine::instance()->color("@base0"));
    palette.setColor(QPalette::Mid, dtkThemesEngine::instance()->color("@base4").lighter());
    palette.setColor(QPalette::Shadow, dtkThemesEngine::instance()->color("@base0"));

    palette.setColor(QPalette::Highlight, dtkThemesEngine::instance()->color("@hl"));
    palette.setColor(QPalette::HighlightedText, dtkThemesEngine::instance()->color("@fg"));

    palette.setColor(QPalette::Link, dtkThemesEngine::instance()->color("@blue"));
    palette.setColor(QPalette::LinkVisited, dtkThemesEngine::instance()->color("@magenta"));

    palette.setColor(QPalette::NoRole, dtkThemesEngine::instance()->color("@bg"));

    palette.setBrush(QPalette::Window, QBrush(dtkThemesEngine::instance()->color("@bg")));
    palette.setBrush(QPalette::WindowText, QBrush(dtkThemesEngine::instance()->color("@fg")));

    palette.setBrush(QPalette::Background, QBrush());
    palette.setBrush(QPalette::Foreground, QBrush());

    palette.setBrush(QPalette::Base, QBrush(dtkThemesEngine::instance()->color("@bgalt")));
    palette.setBrush(QPalette::AlternateBase,
                     QBrush(dtkThemesEngine::instance()->color("@bgalt").lighter()));
    palette.setBrush(QPalette::Text, QBrush(dtkThemesEngine::instance()->color("@fg")));

    palette.setBrush(QPalette::ToolTipBase, QBrush(dtkThemesEngine::instance()->color("@base2")));
    palette.setBrush(QPalette::ToolTipText, QBrush(dtkThemesEngine::instance()->color("@base8")));

    palette.setBrush(QPalette::Button, QBrush(dtkThemesEngine::instance()->color("@base1")));
    palette.setBrush(QPalette::ButtonText, QBrush(dtkThemesEngine::instance()->color("@base8")));

    palette.setBrush(QPalette::BrightText, QBrush(dtkThemesEngine::instance()->color("@red")));

    palette.setBrush(QPalette::Light, QBrush(dtkThemesEngine::instance()->color("@base4")));
    palette.setBrush(QPalette::Midlight, QBrush(dtkThemesEngine::instance()->color("@base3")));
    palette.setBrush(QPalette::Dark, QBrush(dtkThemesEngine::instance()->color("@base0")));
    palette.setBrush(QPalette::Mid, QBrush(dtkThemesEngine::instance()->color("@base0").lighter()));
    palette.setBrush(QPalette::Shadow, QBrush(dtkThemesEngine::instance()->color("@base0")));

    palette.setBrush(QPalette::Highlight, QBrush(dtkThemesEngine::instance()->color("@hl")));
    palette.setBrush(QPalette::HighlightedText, QBrush(dtkThemesEngine::instance()->color("@fg")));

    palette.setBrush(QPalette::Link, QBrush(dtkThemesEngine::instance()->color("@blue")));
    palette.setBrush(QPalette::LinkVisited, QBrush(dtkThemesEngine::instance()->color("@magenta")));

    palette.setBrush(QPalette::NoRole, QBrush(dtkThemesEngine::instance()->color("@fg")));
}

void dtkTheme::polish(QApplication *application)
{
    if (!application)
        return;

    application->setStyleSheet(d->sheet);
}

void dtkTheme::touch(QWidget *w)
{
    this->QProxyStyle::unpolish(w);
    w->setStyleSheet(d->sheet);
    this->QProxyStyle::polish(w);
}

QPixmap dtkTheme::generatedIconPixmap(QIcon::Mode mode, const QPixmap &pixmap,
                                      const QStyleOption *option) const
{
    return QProxyStyle::generatedIconPixmap(mode, pixmap, option);
}

QIcon dtkTheme::standardIcon(StandardPixmap icon, const QStyleOption *option,
                             const QWidget *widget) const
{
    return QProxyStyle::standardIcon(icon, option, widget);
}

//
// dtkTheme.cpp ends here
