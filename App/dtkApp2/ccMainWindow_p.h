/* sislMainWindow_p.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:20:26 2012 (+0200)
 * Version: $Id$
 * Last-Updated: Sat Jun  2 01:26:57 2012 (+0200)
 *           By: Julien Wintz
 *     Update #: 13
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef CCMAINWINDOW_P_H
#define CCMAINWINDOW_P_H

#include <QtCore>
#include <QtGui>
#include <QMenu>
/*
class dtkComposer;
class dtkComposerFactoryView;
class dtkComposerGraphView;
class dtkComposerSceneModel;
class dtkComposerSceneNodeEditor;
class dtkComposerSceneView;
class dtkComposerStackView;
*/
class dtkRecentFilesMenu;

class dtkSplitter;

class ccMainWindow;

class ccMainWindowPrivate : public QObject
{
    Q_OBJECT

public:
    bool maySave(void);

public:
    void setCurrentFile(const QString& file);


public:
    dtkSplitter *inner;

public:
    QMenu *composition_menu;
    QAction *composition_open_action;
    QAction *composition_save_action;
    QAction *composition_saveas_action;
    QAction *composition_insert_action;
    QAction *composition_quit_action;

    QMenu *edit_menu;
    QAction *undo_action;
    QAction *redo_action;

    dtkRecentFilesMenu *recent_compositions_menu;

public:
    QString current_composition;

public:
    bool closing;

public:
    ccMainWindow *q;
};

#endif
