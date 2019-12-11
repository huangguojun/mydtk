/* dtkPlotRenderer.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Mon Jul  4 21:11:19 2011 (+0200)
 * Version: $Id: e8575b0501f4284417b28bc63b2746652eccd205 $
 * Last-Updated: Mon Apr 16 12:31:29 2012 (+0200)
 *           By: tkloczko
 *     Update #: 17
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLOTRENDERER_H
#define DTKPLOTRENDERER_H

#include <dtkPlotSupportExport.h>


class dtkPlotRendererPrivate;
class dtkPlotView;

class DTKPLOTSUPPORT_EXPORT dtkPlotRenderer
{
public:
    dtkPlotRenderer(void);
    ~dtkPlotRenderer(void);

public:
    void render(void);

public:
    void setView(dtkPlotView *view);
    void setSize(const QSize& size);
    void setPath(const QString& path);

private:
    dtkPlotRendererPrivate *d;
};

#endif
