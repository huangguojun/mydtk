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

#include <QtWidgets>

class DTKTHEMES_EXPORT dtkTheme : public QProxyStyle
{
    Q_OBJECT

public:
    dtkTheme(const QString&);
    dtkTheme(const QString&, QStyle *style);

public:
    QStyle *baseStyle(void) const;

public:
    void drawPrimitive(QStyle::PrimitiveElement, const QStyleOption *, QPainter *, const QWidget* widget) const override;

public:
    void polish(QPalette &) override;
    void polish(QApplication *) override;

public:
    void touch(QWidget *);

public:
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const override;

public:
    QIcon standardIcon(StandardPixmap, const QStyleOption *, const QWidget *) const override;

private:
    QStyle *styleBase(QStyle *style = nullptr) const;

private:
    class dtkThemePrivate *d;
};

//
// dtkTheme.h ends here
