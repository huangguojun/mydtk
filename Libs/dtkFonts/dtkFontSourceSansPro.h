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

class DTKFONTS_EXPORT dtkFontSourceSansPro : public QObject
{
    Q_OBJECT

public:
    static dtkFontSourceSansPro *instance(void);

protected:
    static dtkFontSourceSansPro *s_instance;

private:
    dtkFontSourceSansPro(QObject *parent = 0);
    ~dtkFontSourceSansPro(void) = default;

public:
    bool initFontSourceSansPro(void);

    QFont font(int size);

    QString fontName(void) { return fontName_; }

private:
    QString fontName_;
};

//
// dtkFontSourceSansPro.h ends here
