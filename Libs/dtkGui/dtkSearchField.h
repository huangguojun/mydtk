/* dtkSearchField.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Mar 21 23:43:07 2012 (+0100)
 * Version: $Id: 3b9bffadea949f43d762f4c2cc172bda5fad3461 $
 * Last-Updated: Wed Mar 21 23:59:52 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 19
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKSEARCHFIELD_H
#define DTKSEARCHFIELD_H

#include <QtWidgets/QWidget>

class dtkSearchFieldPrivate;

class dtkSearchField : public QWidget
{
    Q_OBJECT

public:
    dtkSearchField(QWidget *parent);
    ~dtkSearchField(void);

public:
    QString text(void) const;

public slots:
    void setText(const QString& text);
    void setPlaceholderText(const QString& text);

public slots:
    void clear(void);

signals:
    void textChanged(const QString& text);
    void editingFinished();

protected:
    void resizeEvent(QResizeEvent *);

private:
    dtkSearchFieldPrivate *d;

private:
    friend class dtkSearchFieldPrivate;
};

#endif // QSEARCHFIELD_H
