/* dtkPluginsEditor.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Nov  9 16:05:37 2011 (+0100)
 * Version: $Id: 02a3c177a9d939d39cf9813ee8e54af5478a1838 $
 * Last-Updated: Wed Nov  9 16:16:24 2011 (+0100)
 *           By: Julien Wintz
 *     Update #: 6
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPluginsEditor.h"

#include <QtWidgets>

class dtkPluginsEditorPrivate
{
public:
    QTreeWidget *tree;
};

dtkPluginsEditor::dtkPluginsEditor(QWidget *parent) : QWidget(parent), d(new dtkPluginsEditorPrivate)
{
    d->tree = new QTreeWidget(this);
    d->tree->setAttribute(Qt::WA_MacShowFocusRect, false);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->tree);
}

dtkPluginsEditor::~dtkPluginsEditor(void)
{
    delete d;

    d = NULL;
}
