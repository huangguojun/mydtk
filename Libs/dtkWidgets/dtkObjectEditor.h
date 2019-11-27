/* dtkObjectEditor.h ---
 *
 * Author: Thibaud Kloczko
 * Created: mar. oct. 15 16:54:54 2013 (+0200)
 * Version:
 * Last-Updated: mer. oct. 23 14:30:58 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 18
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkWidgetsExport.h>

#include <QScrollArea>

class dtkObjectEditorPrivate;

class DTKWIDGETS_EXPORT dtkObjectEditor : public QFrame
{
    Q_OBJECT

public:
    dtkObjectEditor(QObject *object, QWidget *parent = 0);
    ~dtkObjectEditor(void);

private:
    dtkObjectEditorPrivate *d;
};
