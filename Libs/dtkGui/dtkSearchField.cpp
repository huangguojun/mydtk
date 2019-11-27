/* dtkSearchField.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Mar 21 23:45:02 2012 (+0100)
 * Version: $Id: 7fda346026acdb2c805f84ea852bbabc62993745 $
 * Last-Updated: Thu Mar 22 00:00:54 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 8
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkSearchField.h"

#include <QtCore>
#include <QtWidgets>

class dtkSearchFieldPrivate : public QObject
{
public:
    dtkSearchFieldPrivate(dtkSearchField *searchField, QLineEdit *lineEdit, QToolButton *clearButton)
        : QObject(searchField), lineEdit(lineEdit), clearButton(clearButton) {}

    int lineEditFrameWidth() const {
        return lineEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    }
    int clearButtonPaddedWidth() const {
        return clearButton->width() + lineEditFrameWidth() * 2;
    }
    int clearButtonPaddedHeight() const {
        return clearButton->height() + lineEditFrameWidth() * 2;
    }

    QPointer<QLineEdit> lineEdit;
    QPointer<QToolButton> clearButton;
};

dtkSearchField::dtkSearchField(QWidget *parent) : QWidget(parent)
{
    QLineEdit *lineEdit = new QLineEdit(this);
    connect(lineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(textChanged(QString)));
    connect(lineEdit, SIGNAL(editingFinished()),
            this, SIGNAL(editingFinished()));
    connect(lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setText(QString)));

    QToolButton *clearButton = new QToolButton(this);
    QPixmap clearIcon(QString(":/Qocoa/qsearchfield_nonmac.png"));
    clearButton->setIcon(QIcon(clearIcon));
    clearButton->setIconSize(clearIcon.size());
    clearButton->setFixedSize(clearIcon.size());
    clearButton->setStyleSheet("border: none;");
    clearButton->hide();
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    d = new dtkSearchFieldPrivate(this, lineEdit, clearButton);

    lineEdit->setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(d->clearButtonPaddedWidth()));
    const int width = qMax(lineEdit->minimumSizeHint().width(), d->clearButtonPaddedWidth());
    const int height = qMax(lineEdit->minimumSizeHint().height(), d->clearButtonPaddedHeight());
    lineEdit->setMinimumSize(width, height);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(lineEdit);
}

dtkSearchField::~dtkSearchField(void)
{
    delete d;

    d = NULL;
}

void dtkSearchField::setText(const QString& text)
{
    Q_ASSERT(d && d->clearButton && d->lineEdit);

    if (!(d && d->clearButton && d->lineEdit))
        return;

    d->clearButton->setVisible(!text.isEmpty());

    if (text != this->text())
        d->lineEdit->setText(text);
}

void dtkSearchField::setPlaceholderText(const QString& text)
{
    Q_ASSERT(d && d->lineEdit);

    if (!(d && d->lineEdit))
        return;

#if QT_VERSION >= 0x040700
    d->lineEdit->setPlaceholderText(text);
#endif
}

void dtkSearchField::clear()
{
    Q_ASSERT(d && d->lineEdit);

    if (!(d && d->lineEdit))
        return;

    d->lineEdit->clear();
}

QString dtkSearchField::text() const
{
    Q_ASSERT(d && d->lineEdit);

    if (!(d && d->lineEdit))
        return QString();

    return d->lineEdit->text();
}

void dtkSearchField::resizeEvent(QResizeEvent *resizeEvent)
{
    Q_ASSERT(d && d->clearButton && d->lineEdit);

    if (!(d && d->clearButton && d->lineEdit))
        return;

    QWidget::resizeEvent(resizeEvent);
    const int x = width() - d->clearButtonPaddedWidth();
    const int y = (height() - d->clearButton->height()) / 2;
    d->clearButton->move(x, y);
}
