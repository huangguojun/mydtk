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

#include "dtkFontSourceCodePro.h"

dtkFontSourceCodePro *dtkFontSourceCodePro::instance(void)
{
    if(!s_instance)
        s_instance = new dtkFontSourceCodePro;

    return s_instance;
}

dtkFontSourceCodePro::dtkFontSourceCodePro(QObject *parent) : QObject(parent)
{

}

bool dtkFontSourceCodePro::initFontSourceCodePro(void)
{
    static int fontSourceCodeProFontId = -1;

    if(fontSourceCodeProFontId < 0) {

        QFile res(":dtkFontSourceCodePro.ttf");

        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font awesome font could not be loaded!";
            return false;
        }

        QByteArray fontData(res.readAll());
        res.close();

        fontSourceCodeProFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontSourceCodeProFontId);

    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Source code font is empty?!";
        fontSourceCodeProFontId = -1;
        return false;
    }

    return true;
}

QFont dtkFontSourceCodePro::font(int size)
{
    QFont font(fontName_ + " " + "Regular");
    font.setPixelSize(size);

    return font;
}

dtkFontSourceCodePro *dtkFontSourceCodePro::s_instance = Q_NULLPTR;

//
// dtkFontSourceCodePro.cpp ends here
