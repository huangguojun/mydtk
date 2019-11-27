/* dtkPluginsEditor.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Nov  9 16:04:20 2011 (+0100)
 * Version: $Id: e518b0b583e62aba93ffbefa924e856c16394b2d $
 * Last-Updated: Wed Nov  9 16:15:41 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 7
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKPLUGINSEDITOR_H
#define DTKPLUGINSEDITOR_H

#include <dtkGuiSupportExport.h>

#include <QtWidgets/QWidget>

class dtkPluginsEditorPrivate;

class DTKGUISUPPORT_EXPORT dtkPluginsEditor : public QWidget
{
    Q_OBJECT

public:
    dtkPluginsEditor(QWidget *parent = 0);
    ~dtkPluginsEditor(void);

private:
    dtkPluginsEditorPrivate *d;
};

#endif
