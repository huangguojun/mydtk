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

#include "dtkThemesWidget.h"

#include <dtkThemes>

#include <QtCore>
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class dtkThemesWidgetPrivate : public QObject
{
    Q_OBJECT

public slots:
    void update(void);

public:
    QGridLayout *layout;

public:
    dtkThemesParser *parser = nullptr;
};

void dtkThemesWidgetPrivate::update(void)
{
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 13; j++) {

            QString key;

            switch(i) {
            case 0:
                switch(j) {
                case  0: key = "@base0"; break;
                case  1: key = "@base1"; break;
                case  2: key = "@base2"; break;
                case  3: key = "@base3"; break;
                case  4: key = "@base4"; break;
                case  5: key = "@base5"; break;
                case  6: key = "@base6"; break;
                case  7: key = "@base7"; break;
                case  8: key = "@base8"; break;
                case  9: key = "@bg";    break;
                case 10: key = "@bgalt"; break;
                case 11: key = "@fg";    break;
                case 12: key = "@fgalt"; break;
                default: break;
                };
                break;
            case 1:
                switch(j) {
                case  0: key = "@grey";     break;
                case  1: key = "@red";      break;
                case  2: key = "@orange";   break;
                case  3: key = "@green";    break;
                case  4: key = "@teal";     break;
                case  5: key = "@yellow";   break;
                case  6: key = "@blue";     break;
                case  7: key = "@darkblue"; break;
                case  8: key = "@magenta";  break;
                case  9: key = "@violet";   break;
                case 10: key = "@cyan";     break;
                case 11: key = "@darkcyan"; break;
                case 12: key = "@hl";       break;
                default: break;
                };
                break;
            default:
                break;
            };

            QLabel *label = static_cast<QLabel *>(layout->itemAtPosition(i, j)->widget());
            label->setAlignment(Qt::AlignCenter);
            label->setText(key);

            if(!this->parser) {
                label->setStyleSheet(QString("background: %1").arg(dtkThemesEngine::instance()->value(key)));
                label->setToolTip(key + " (" + dtkThemesEngine::instance()->value(key) + ")");
            } else {
                label->setStyleSheet(QString("background: %1").arg(this->parser->values()[key]));
                label->setToolTip(key + " (" + this->parser->values()[key] + ")");
            }
        }
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

dtkThemesWidget::dtkThemesWidget(QWidget *parent) : QWidget(parent)
{
    d = new dtkThemesWidgetPrivate;

    d->layout = new QGridLayout(this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 13; j++)
            d->layout->addWidget(new QLabel(this), i, j);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(dtkThemesEngine::instance(), SIGNAL(changed()), d, SLOT(update()));
}

dtkThemesWidget::dtkThemesWidget(const QString& theme, QWidget *parent) : QWidget(parent)
{
    d = new dtkThemesWidgetPrivate;

    d->layout = new QGridLayout(this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 13; j++)
            d->layout->addWidget(new QLabel(this), i, j);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    d->parser = new dtkThemesParser(theme, this);

    d->update();
}

dtkThemesWidget::~dtkThemesWidget(void)
{
    delete d;
}

QSize dtkThemesWidget::sizeHint(void) const
{
    return QSize(200, 100);
}

// /////////////////////////////////////////////////////////////////////////////

#include "dtkThemesWidget.moc"

//
// dtkThemesWidget.cpp ends here
