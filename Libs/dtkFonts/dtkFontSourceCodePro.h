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

class DTKFONTS_EXPORT dtkFontSourceCodePro : public QObject
{
    Q_OBJECT

public:
    static dtkFontSourceCodePro *instance(void);

protected:
    static dtkFontSourceCodePro *s_instance;

private:
     dtkFontSourceCodePro(QObject *parent = 0);
    ~dtkFontSourceCodePro(void) = default;

public:
    bool initFontSourceCodePro(void);

    QFont font(int size);

    QString fontName(void) { return fontName_ ; }

private:
    QString fontName_;
};

//
// dtkFontSourceCodePro.h ends here
