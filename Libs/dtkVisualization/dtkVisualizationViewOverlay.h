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

#include <dtkVisualizationExport>

#include <dtkFonts/dtkFontAwesome>

#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>

#include <QtGui/QPixmap>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewOverlay
// ///////////////////////////////////////////////////////////////////

class DTKVISUALIZATION_EXPORT dtkVisualizationViewOverlayIcon : public QLabel
{
    Q_OBJECT

public:
    dtkVisualizationViewOverlayIcon(QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const override;

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *) override;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKVISUALIZATION_EXPORT dtkVisualizationViewOverlayText : public QLabel
{
    Q_OBJECT

public:
    dtkVisualizationViewOverlayText(QString = "", QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const override;

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *) override;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class DTKVISUALIZATION_EXPORT dtkVisualizationViewOverlay : public QFrame
{
    Q_OBJECT

public:
    dtkVisualizationViewOverlay(QChar, QString = "", QWidget *parent = nullptr);
    dtkVisualizationViewOverlay(fa::icon, QString = "", QWidget *parent = nullptr);
    dtkVisualizationViewOverlay(const QString &path_on, const QString &path_off, QString = "",
                                QWidget *parent = nullptr);
    ~dtkVisualizationViewOverlay(void);

    void activate(bool);
    void toggle(bool);

    void changePixmaps(const QPixmap &, const QPixmap &);
    void changeColor(const QColor &);
    void changePaths(const QString &, const QString &);
    void changeIcon(fa::icon);

    QString text(void) const;

    bool isActivated(void) const;
    bool isToggled(void) const;

    void setVisible(bool) override;

signals:
    void iconClicked(void);
    void textClicked(void);

public:
    bool toggled = false;
    bool activated = true;

public:
    dtkVisualizationViewOverlayIcon *label_icon = nullptr;
    dtkVisualizationViewOverlayText *label_text = nullptr;

public:
    QPixmap pix_on;
    QPixmap pix_off;
    fa::icon icon;
    bool pixmap = false;
};

//
// dtkVisualizationViewOverlay.h ends here
