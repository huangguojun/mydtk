/* dtkObjectEditor.cpp ---
 *
 * Author: Thibaud Kloczko
 * Created: mar. oct. 15 17:02:00 2013 (+0200)
 * Version:
 * Last-Updated: mer. oct. 23 14:30:43 2013 (+0200)
 *           By: Thibaud Kloczko
 *     Update #: 137
 */

/* Change Log:
 *
 */

#include "dtkObjectEditor.h"
#include "dtkPropertyEditorFactory.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <QMetaProperty>
#include <QObject>
#include <QVariant>

class dtkPropertyEditor;

class dtkObjectEditorPrivate
{
public:
};

dtkObjectEditor::dtkObjectEditor(QObject *object, QWidget *parent)
    : QFrame(parent), d(new dtkObjectEditorPrivate)
{
    // QFrame *frame = new QFrame(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    // this->setWidget(frame);
    // this->setWidgetResizable(true);
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    // this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // this->setContentsMargins(0, 0, 0, 0);

    QList<const QMetaObject *> class_list;
    const QMetaObject *meta_object = object->metaObject();

    while (meta_object) {
        class_list.prepend(meta_object);
        meta_object = meta_object->superClass();
    }

    int prev_count = 0;
    QWidget *editor = NULL;
    QString name;
    QList<const QMetaObject *>::const_iterator it = class_list.constBegin();

    while (it != class_list.constEnd()) {
        meta_object = *it;
        int count = meta_object->propertyCount();

        for (int i = prev_count; i < count; ++i) {
            name = QString(meta_object->property(i).name());
            editor = reinterpret_cast<QWidget *>(
                    dtkPropertyEditorFactory::instance()->create(name, object, this));
            layout->addWidget(new QLabel(name.append(":"), this));
            layout->addWidget(editor);
        }

        prev_count = count;
        ++it;
    }
}

dtkObjectEditor::~dtkObjectEditor(void) {}
