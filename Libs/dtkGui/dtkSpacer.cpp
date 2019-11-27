// Version: $Id: 5cb6308022342cca95a7e05a059048f0e72648ec $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkSpacer.h"

dtkSpacer::dtkSpacer(QWidget *parent, int width, int height) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setMinimumSize(width, height);

    if (width && !height)
        this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    else if (!width && height)
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    else
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

dtkSpacer::~dtkSpacer(void)
{

}

QSize dtkSpacer::sizeHint(void) const
{
    QSize size = QWidget::sizeHint();

    if (size.width()  < 1) size.setWidth(1);

    if (size.height() < 1) size.setHeight(1);

    return (size);
}

//
// dtkSpacer.cpp ends here
