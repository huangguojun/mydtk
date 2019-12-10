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

#include "dtkVisualizationDecorator.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorInfo : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorInfo(void);
    ~dtkVisualizationDecoratorInfo(void);

public:
    void setFileInfo(const QFileInfo& f_info);
    void setInformation(const QString& key, const QString& value, const QString& tooltip = QString());
    void setInformation(const QString& key, QWidget *value);

public:
    void setVisibility(bool) override;

public:
    bool isDecorating(void) override;

    void setData(const QVariant&)            override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void)                   override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

    void touch(void) override;

protected:
    class dtkVisualizationDecoratorInfoPrivate *d;
};

//
// dtkVisualizationDecoratorInfo.h ends here
