/* dtkInterpreter.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 15:29:01 2009 (+0200)
 * Version: $Id: e96f2e1deda305657d7b12ed7f26195c66efde93 $
 * Last-Updated: Thu Jan 10 14:45:13 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 92
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QtWidgets>

class dtkScriptInterpreter;
class dtkInterpreterPrivate;

class DTKWIDGETS_EXPORT dtkInterpreter : public QPlainTextEdit
{
    Q_OBJECT

public:
             dtkInterpreter(QWidget *parent = 0);
    virtual ~dtkInterpreter(void);

#pragma mark -
#pragma mark - Interpretation interface

    dtkScriptInterpreter *interpreter(void);

    void registerInterpreter(dtkScriptInterpreter *interpreter);

#pragma mark -
#pragma mark - Graphical interface

    void output(const QString& result);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

protected:
    void onKeyUpPressed(void);
    void onKeyDownPressed(void);
    void onKeyLeftPressed(void);
    void onKeyRightPressed(void);
    void onKeyEnterPressed(void);

protected:
    int currentLineNumber(void) const;
    QString currentLine(void) const;
    QString currentBlock(void) const;

private:
    dtkInterpreterPrivate *d;
};
