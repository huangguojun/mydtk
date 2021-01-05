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

#include "dtkVisualizationDecoratorWithClut.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorScalarGlyphs
    : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorScalarGlyphs(void);
    ~dtkVisualizationDecoratorScalarGlyphs(void);

public:
    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

protected:
    bool setCurrentFieldName(const QString &) override;
    void setColorMap(const QMap<double, QColor> &) override;

public:
    void setVisibility(bool) override;
    void restoreSettings(void) override;

public:
    void setCurrentGlyphsSize(double);
    void setCurrentGlyphsSource(const QString &);

signals:
    void currentGlyphsSizeChanged(double);
    void currentGlyphsSourceChanged(const QString &);

public:
    void *glyphs(void);
    void *actor(void);

public:
    void touch(void) override;

protected:
    class dtkVisualizationDecoratorScalarGlyphsPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorScalarGlyphs.h ends here
