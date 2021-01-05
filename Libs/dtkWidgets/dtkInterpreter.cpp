/* dtkInterpreter.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 15:31:39 2009 (+0200)
 * Version: $Id: 4c3aef7db35336bc364f972207a8ec68ae793d45 $
 * Last-Updated: Thu Jan 10 14:47:04 2013 (+0100)
 *           By: Julien Wintz
 *     Update #: 521
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include <iostream>

#include <QtCore>
#include <QtDebug>
#include <QtGui>
#include <QtWidgets>

#include <dtkConfig.h>

#include <dtkScript/dtkScriptInterpreter.h>
#if defined(DTK_BUILD_WRAPPERS) && defined(DTK_HAVE_PYTHON)
#    include <dtkScript/dtkScriptInterpreterPython.h>
#endif
#if defined(DTK_BUILD_WRAPPERS) && defined(DTK_HAVE_TCL)
#    include <dtkScript/dtkScriptInterpreterTcl.h>
#endif

#include "dtkInterpreter.h"

// /////////////////////////////////////////////////////////////////
// dtkInterpreterPrivate
// /////////////////////////////////////////////////////////////////

class dtkInterpreterPrivate
{
public:
    dtkScriptInterpreter *interpreter;

public:
    QTextCursor cursor;

public:
    QStringList history;
    unsigned int history_index;
    bool history_dirty;

public:
    QString prompt;
};

// /////////////////////////////////////////////////////////////////
// dtkInterpreter
// /////////////////////////////////////////////////////////////////

dtkInterpreter::dtkInterpreter(QWidget *parent)
    : QPlainTextEdit(parent), d(new dtkInterpreterPrivate)
{
    d->interpreter = NULL;
    d->history_index = 0;
    d->history_dirty = false;
    d->prompt = ">>> ";

    this->setReadOnly(true);
}

dtkInterpreter::~dtkInterpreter(void)
{
    delete d;
}

#pragma mark -
#pragma mark - Interpretation interface

dtkScriptInterpreter *dtkInterpreter::interpreter(void)
{
    return d->interpreter;
}

void dtkInterpreter::registerInterpreter(dtkScriptInterpreter *interpreter)
{
    d->interpreter = interpreter;

    this->appendPlainText(d->prompt);
    this->setReadOnly(false);
}

#pragma mark -
#pragma mark - Graphical interface

void dtkInterpreter::keyPressEvent(QKeyEvent *event)
{
    QTextCursor cursor = textCursor();

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {

        this->onKeyEnterPressed();

    } else if (event->key() == Qt::Key_Backspace) {

        if (cursor.columnNumber() > d->prompt.size())
            QPlainTextEdit::keyPressEvent(event);

    } else if (event->key() == Qt::Key_Up) {

        this->onKeyUpPressed();

    } else if (event->key() == Qt::Key_Down) {

        this->onKeyDownPressed();

    } else if (event->key() == Qt::Key_Left) {

        if (cursor.columnNumber() > d->prompt.size())
            QPlainTextEdit::keyPressEvent(event);

        this->onKeyLeftPressed();

    } else if (event->key() == Qt::Key_Right) {

        if (cursor.columnNumber() < currentLine().size())
            QPlainTextEdit::keyPressEvent(event);

        this->onKeyRightPressed();

    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}

void dtkInterpreter::mousePressEvent(QMouseEvent *event)
{
    d->cursor = this->textCursor();

    QPlainTextEdit::mousePressEvent(event);
}

void dtkInterpreter::mouseReleaseEvent(QMouseEvent *event)
{
    QPlainTextEdit::mouseReleaseEvent(event);

    if (d->cursor.blockNumber() + 1 != currentLineNumber()
        && d->cursor.columnNumber() <= d->prompt.size())
        this->setTextCursor(d->cursor);
}

void dtkInterpreter::onKeyUpPressed(void)
{
    if (d->history.size() == 0)
        return;

    if (d->history_index && d->history_index == (unsigned int)d->history.size() - 1)
        return;

    if (d->history_index == 0 && !d->history_dirty) {
        QString line = currentLine();

        if (d->interpreter)
            line.remove(d->prompt);

        d->history.push_front(line);
        d->history_dirty = true;
    }

    d->history_index++;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, d->prompt.size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(d->history.at(d->history_index));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);
}

void dtkInterpreter::onKeyDownPressed(void)
{
    if (d->history_index == 0)
        return;

    d->history_index--;

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, d->prompt.size());
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(d->history.at(d->history_index));
    cursor.movePosition(QTextCursor::EndOfLine);
    this->setTextCursor(cursor);

    if (d->history_index == 0 && d->history_dirty) {
        d->history.pop_front();
        d->history_dirty = false;
    }
}

void dtkInterpreter::onKeyLeftPressed(void)
{
    // check cursor
}

void dtkInterpreter::onKeyRightPressed(void)
{
    // check cursor
}

void dtkInterpreter::onKeyEnterPressed(void)
{
    int stat;

    QString line = currentBlock();

    if (d->interpreter)
        line.remove(d->prompt);

    if (!line.isEmpty()) {
        if (d->history_index > 0 && d->history_dirty)
            d->history.removeFirst();

        d->history.push_front(line);
        d->history_index = 0;
        d->history_dirty = false;
    }

    this->output(d->interpreter->interpret(line, &stat));
}

void dtkInterpreter::output(const QString &result)
{
    QString text(result);

    if (!text.simplified().isEmpty())
        this->appendPlainText(text);

    this->appendPlainText(d->prompt);

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
}

int dtkInterpreter::currentLineNumber(void) const
{
    return this->textCursor().blockNumber() + 1;
}

QString dtkInterpreter::currentLine(void) const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::LineUnderCursor);

    return tc.selectedText();
}

QString dtkInterpreter::currentBlock(void) const
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);

    while (!tc.selectedText().contains(d->prompt))
        tc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

    QString block;
    QString text = tc.selectedText();
    QStringList lines = text.split(QChar(8233));

    if (lines.count() > 1)
        block = lines.join("\n");
    else
        block = text;

    return block;
}
