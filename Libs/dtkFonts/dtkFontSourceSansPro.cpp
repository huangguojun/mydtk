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

#include "dtkFontSourceSansPro.h"

dtkFontSourceSansPro *dtkFontSourceSansPro::instance(void)
{
    if(!s_instance)
        s_instance = new dtkFontSourceSansPro;

    return s_instance;
}

dtkFontSourceSansPro::dtkFontSourceSansPro(QObject *parent) : QObject(parent)
{

}

bool dtkFontSourceSansPro::initFontSourceSansPro(void)
{
    static int fontSourceSansProFontId = -1;

    if(fontSourceSansProFontId < 0) {

        QFile res(":dtkFontSourceSansPro.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font source sans pro font could not be loaded!";
            return false;
        }

        QByteArray fontData(res.readAll());
        res.close();

        fontSourceSansProFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontSourceSansProFontId);
    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Font source sans pro font is empty?!";
        fontSourceSansProFontId = -1;
        return false;
    }

    return true;
}

QFont dtkFontSourceSansPro::font(int size)
{
    QFont font(fontName_);
    font.setPixelSize(size);
    return font;
}

dtkFontSourceSansPro *dtkFontSourceSansPro::s_instance = Q_NULLPTR;

//
// dtkFontSourceSansPro.cpp ends here
