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

#include <QWidget>

class QString;
class QSize;

class DTKTHEMES_EXPORT dtkThemesWidget : public QWidget
{
    Q_OBJECT

public:
     dtkThemesWidget(QWidget *parent = nullptr);
     dtkThemesWidget(const QString& theme, QWidget *parent = nullptr);
    ~dtkThemesWidget(void);

public:
    QSize sizeHint(void) const;

private:
    class dtkThemesWidgetPrivate *d;
};

//
// dtkThemesWidget.h ends here
