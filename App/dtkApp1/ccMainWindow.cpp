/* sislMainWindow.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Sat Jun  2 01:16:55 2012 (+0200)
 * Version: $Id$
 * Last-Updated: 2012 Thu Nov  8 14:16:39 (+0100)
 *           By: Thibaud Kloczko, Inria.
 *     Update #: 82
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "ccMainWindow.h"

/*
#include <ccComposer/ccComposerFactory.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerEvaluator.h>
#include <dtkComposer/dtkComposerFactoryView.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>
*/
#include <dtkGui/dtkRecentFilesMenu.h>
#include <dtkGui/dtkSpacer.h>
#include <dtkGui/dtkSplitter.h>
#include <dtkGui/dtkViewManager.h>
#include <dtkIo/vtkLidarStream.h>
//#include <dtkCore/dtkGlobal.h>

#include <dtkLog/dtkLog.h>

#include <QtCore>
#include <QtGui>
#include <QFileDialog>

// /////////////////////////////////////////////////////////////////
// ccMainWindowPrivate
// /////////////////////////////////////////////////////////////////


ccMainWindow::ccMainWindow(QWidget *parent) : QMainWindow(parent), d(new ccMainWindowPrivate)
{
 

    vtkLidarStream *pLidarStream = new vtkLidarStream();
    pLidarStream->Start();
    /*
    d->q = this;

    // -- Actions

    d->composition_open_action = new QAction("Open", this);
    d->composition_open_action->setShortcut(QKeySequence::Open);

    d->composition_save_action = new QAction("Save", this);
    d->composition_save_action->setShortcut(QKeySequence::Save);

    d->composition_saveas_action = new QAction("Save As...", this);
    d->composition_saveas_action->setShortcut(QKeySequence::SaveAs);

    d->composition_insert_action = new QAction("Insert", this);
    d->composition_insert_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_I);

    d->composition_quit_action = new QAction("Quit", this);
    d->composition_quit_action->setShortcut(QKeySequence::Quit);

    //d->undo_action = d->composer->stack()->createUndoAction(this);
    //d->undo_action->setShortcut(QKeySequence::Undo);

    //d->redo_action = d->composer->stack()->createRedoAction(this);
    //d->redo_action->setShortcut(QKeySequence::Redo);

    QAction *run_action = new QAction("Run", this);
    run_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_R);

    QAction *step_action = new QAction("Step", this);
    step_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);

    QAction *continue_action = new QAction("Cont", this);
    continue_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_C);

    QAction *next_action = new QAction("Next", this);
    next_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_T);

    QAction *stop_action = new QAction("Stop", this);
    stop_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Period);

    // -- Menus

    QMenuBar *menu_bar = this->menuBar();

    d->recent_compositions_menu = new dtkRecentFilesMenu("Open recent...", this);

    d->composition_menu = menu_bar->addMenu("Composition");
    d->composition_menu->addAction(d->composition_open_action);
    d->composition_menu->addMenu(d->recent_compositions_menu);
    d->composition_menu->addAction(d->composition_save_action);
    d->composition_menu->addAction(d->composition_saveas_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_insert_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_quit_action);

    d->edit_menu = menu_bar->addMenu("Edit");
    d->edit_menu->addAction(d->undo_action);
    d->edit_menu->addAction(d->redo_action);

    QMenu *debug_menu = menu_bar->addMenu("Debug");
    debug_menu->addAction(run_action);
    debug_menu->addAction(step_action);
    debug_menu->addAction(continue_action);
    debug_menu->addAction(next_action);
    debug_menu->addAction(stop_action);


    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    //left->addWidget(d->nodes);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    //right->addWidget(d->scene);
    //right->addWidget(d->editor);
    //right->addWidget(d->stack);
   // right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);

    QWidget *middle = new QWidget(this);

    QVBoxLayout *m_layout = new QVBoxLayout(middle);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
 //   m_layout->addWidget(d->composer);
    m_layout->addWidget(new dtkViewManager(this));

    d->inner = new dtkSplitter(this);
    d->inner->setOrientation(Qt::Horizontal);
    d->inner->addWidget(left);
    d->inner->addWidget(middle);
    d->inner->addWidget(right);
    d->inner->setSizes(QList<int>()
                    << 200
                    << this->size().width()-200-200
                    << 200);

    
    dtkSplitter *central = new dtkSplitter(this);
    central->setOrientation(Qt::Vertical);
    central->addWidget(d->inner);

    this->setCentralWidget(central);
   */ 
}


