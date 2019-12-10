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

#include "dtkFontOcticon.h"

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticonsCharIconPainter
// ///////////////////////////////////////////////////////////////////

class dtkFontOcticonsCharIconPainter: public dtkFontOcticonsIconPainter
{
public:
    virtual void paint(dtkFontOcticons *awesome, QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options)
    {
        Q_UNUSED(mode);
        Q_UNUSED(state);
        Q_UNUSED(options);

        painter->save();

        QColor color = options.value("color").value<QColor>();
        QString text = options.value("text").toString();

        if(mode == QIcon::Disabled) {
            color = options.value("color-disabled").value<QColor>();
            QVariant alt = options.value("text-disabled");
            if(alt.isValid()) {
                text = alt.toString();
            }
        } else if(mode == QIcon::Active) {
            color = options.value("color-active").value<QColor>();
            QVariant alt = options.value("text-active");
            if(alt.isValid()) {
                text = alt.toString();
            }
        } else if(mode == QIcon::Selected) {
            color = options.value("color-selected").value<QColor>();
            QVariant alt = options.value("text-selected");
            if(alt.isValid()) {
                text = alt.toString();
            }
        }

        painter->setPen(color);

        int drawSize = qRound(rect.height()*options.value("scale-factor").toFloat());

        painter->setFont(awesome->font(drawSize));
        painter->drawText(rect, text, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
        painter->restore();
    }
};

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticonsIconPainterIconEngine
// ///////////////////////////////////////////////////////////////////

class dtkFontOcticonsIconPainterIconEngine : public QIconEngine
{
public:
    dtkFontOcticonsIconPainterIconEngine(dtkFontOcticons *awesome, dtkFontOcticonsIconPainter *painter, const QVariantMap& options) : awesomeRef_(awesome), iconPainterRef_(painter), options_(options)
    {

    }

    virtual ~dtkFontOcticonsIconPainterIconEngine(void) {}

    dtkFontOcticonsIconPainterIconEngine *clone(void) const
    {
        return new dtkFontOcticonsIconPainterIconEngine(awesomeRef_, iconPainterRef_, options_);
    }

    virtual void paint(QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
    {
        Q_UNUSED(mode);
        Q_UNUSED(state);

        iconPainterRef_->paint(awesomeRef_, painter, rect, mode, state, options_);
    }

    virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
    {
        QPixmap pm(size);
        pm.fill(Qt::transparent);

        QPainter p(&pm);
        paint(&p, QRect(QPoint(0,0),size), mode, state);

        return pm;
    }

private:
    dtkFontOcticons *awesomeRef_;
    dtkFontOcticonsIconPainter *iconPainterRef_;
    QVariantMap options_;
};

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticons
// ///////////////////////////////////////////////////////////////////

dtkFontOcticons::dtkFontOcticons(QObject *parent) : QObject(parent)
{
    setDefaultOption("color", QColor(50,50,50));
    setDefaultOption("color-disabled", QColor(70,70,70,60));
    setDefaultOption("color-active", QColor(10,10,10));
    setDefaultOption("color-selected", QColor(10,10,10));
    setDefaultOption("scale-factor", 0.9);

    setDefaultOption("text", QVariant());
    setDefaultOption("text-disabled", QVariant());
    setDefaultOption("text-active", QVariant());
    setDefaultOption("text-selected", QVariant());

    fontIconPainter_ = new dtkFontOcticonsCharIconPainter();
}

dtkFontOcticons::~dtkFontOcticons(void)
{
    delete fontIconPainter_;
    qDeleteAll(painterMap_);
}

void dtkFontOcticons::init(const QString& fontname)
{
    fontName_ = fontname;
}

bool dtkFontOcticons::initFontOcticons()
{
    static int fontOcticonsFontId = -1;

    if(fontOcticonsFontId < 0) {

        QFile res(":dtkFontOcticon.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray fontData(res.readAll());
        res.close();

        fontOcticonsFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontOcticonsFontId);
    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        fontOcticonsFontId = -1;
        return false;
    }

    return true;
}

void dtkFontOcticons::setDefaultOption(const QString& name, const QVariant& value)
{
    defaultOptions_.insert(name, value);
}

QVariant dtkFontOcticons::defaultOption(const QString& name)
{
    return defaultOptions_.value(name);
}

static QVariantMap mergeOptions(const QVariantMap& defaults, const QVariantMap& override)
{
    QVariantMap result= defaults;
    if(!override.isEmpty()) {
        QMapIterator<QString,QVariant> itr(override);
        while(itr.hasNext()) {
            itr.next();
            result.insert(itr.key(), itr.value());
        }
    }
    return result;
}

QIcon dtkFontOcticons::icon(int character, const QVariantMap &options)
{
    QVariantMap optionMap = mergeOptions(defaultOptions_, options);
    optionMap.insert("text", QString(QChar(static_cast<int>(character))));

    return icon(fontIconPainter_, optionMap);
}

QIcon dtkFontOcticons::icon(dtkFontOcticonsIconPainter *painter, const QVariantMap& optionMap)
{
    dtkFontOcticonsIconPainterIconEngine *engine = new dtkFontOcticonsIconPainterIconEngine(this, painter, optionMap);

    return QIcon(engine);
}

void dtkFontOcticons::give(const QString& name, dtkFontOcticonsIconPainter *painter)
{
    delete painterMap_.value(name);

    painterMap_.insert(name, painter);
}

QFont dtkFontOcticons::font(int size)
{
    QFont font(fontName_);
    font.setPixelSize(size);
    return font;
}

dtkFontOcticons *dtkFontOcticons::instance(void)
{
    if(!s_instance)
        s_instance = new dtkFontOcticons;

    return s_instance;
}

dtkFontOcticons *dtkFontOcticons::s_instance = Q_NULLPTR;

//
// dtkFontOcticons.cpp ends here
