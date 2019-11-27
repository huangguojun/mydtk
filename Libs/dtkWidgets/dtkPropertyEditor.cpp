/* dtkPropertyEditor.cpp ---
 *
 * Author: Thibaud Kloczko
 * Created: mar. oct. 15 13:33:45 2013 (+0200)
 * Version:
 * Last-Updated: lun. févr.  3 14:33:55 2014 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 284
 */

/* Change Log:
 *
 */

#include "dtkPropertyEditor.h"

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditorPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkPropertyEditorPrivate
{
public:
    QObject *object;
    QMetaProperty meta_property;
    QWidget *editor;
    QLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditor implementation
// ///////////////////////////////////////////////////////////////////

dtkPropertyEditor::dtkPropertyEditor(const QString& property_name, QObject *object, QWidget *parent) : QFrame(parent), d(new dtkPropertyEditorPrivate)
{
    if (!object) {
        qDebug() << Q_FUNC_INFO << "dtkPropertyEditor must be created using a valid QObject.";
        return;
    }

    d->object = object;
    this->setObjectName(property_name);

    d->meta_property = object->metaObject()->property(object->metaObject()->indexOfProperty(qPrintable(property_name)));

    d->editor = NULL;
    d->layout = NULL;
}

dtkPropertyEditor::~dtkPropertyEditor(void)
{
    if (d->editor)
        delete d->editor;

    delete d;

    d = NULL;
}

void dtkPropertyEditor::setValue(const QVariant& value)
{
    d->object->setProperty(d->meta_property.name(), value);
}

QVariant dtkPropertyEditor::value(void) const
{
    return d->object->property(d->meta_property.name());
}

QObject *dtkPropertyEditor::propertyObject(void)
{
    return d->object;
}

// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditorDouble implementation
// ///////////////////////////////////////////////////////////////////

dtkPropertyEditorDouble::dtkPropertyEditorDouble(const QString& property_name, QObject *object, QWidget *parent) : dtkPropertyEditor(property_name, object, parent)
{
    QDoubleSpinBox *spin_d = new QDoubleSpinBox(this);
    spin_d->setObjectName(property_name);
    spin_d->setMinimum(-999999999);
    spin_d->setMaximum(+999999999);
    spin_d->setDecimals(14);
    spin_d->setSingleStep(1);
    spin_d->setEnabled(true);
    spin_d->setValue(object->property(d->meta_property.name()).toDouble());

    QObject::connect(spin_d, SIGNAL(editingFinished()), this, SLOT(onValueChanged()));

    d->editor = spin_d;

    d->layout = new QHBoxLayout;
    d->layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(d->layout);
    d->layout->addWidget(d->editor);
}

dtkPropertyEditorDouble::~dtkPropertyEditorDouble(void)
{

}

void dtkPropertyEditorDouble::setEditorData(const QVariant& data)
{
    d->editor->blockSignals(true);
    static_cast<QDoubleSpinBox *>(d->editor)->setValue(data.toDouble());
    d->editor->blockSignals(false);
}

QVariant dtkPropertyEditorDouble::editorData(void)
{
    return QVariant::fromValue(static_cast<QDoubleSpinBox *>(d->editor)->value());
}

void dtkPropertyEditorDouble::onValueChanged(void)
{
    d->object->setProperty(d->meta_property.name(), this->editorData());
}

dtkPropertyEditor *createDtkPropertyEditorDouble(const QString& property_name, QObject *object, QWidget *parent)
{
    return new dtkPropertyEditorDouble(property_name, object, parent);
}


// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditorInteger implementation
// ///////////////////////////////////////////////////////////////////

dtkPropertyEditorInteger::dtkPropertyEditorInteger(const QString& property_name, QObject *object, QWidget *parent) : dtkPropertyEditor(property_name, object, parent)
{
    QSpinBox *spin_i = new QSpinBox(this);
    spin_i->setObjectName(property_name);
    spin_i->setMinimum(-999999999);
    spin_i->setMaximum(+999999999);
    spin_i->setSingleStep(1);
    spin_i->setEnabled(true);
    spin_i->setValue(object->property(d->meta_property.name()).toLongLong());

    QObject::connect(spin_i, SIGNAL(editingFinished()), this, SLOT(onValueChanged()));

    d->editor = spin_i;

    d->layout = new QHBoxLayout;
    d->layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(d->layout);
    d->layout->addWidget(d->editor);
}

dtkPropertyEditorInteger::~dtkPropertyEditorInteger(void)
{

}

void dtkPropertyEditorInteger::setEditorData(const QVariant& data)
{
    d->editor->blockSignals(true);
    static_cast<QSpinBox *>(d->editor)->setValue(data.toLongLong());
    d->editor->blockSignals(false);
}

QVariant dtkPropertyEditorInteger::editorData(void)
{
    return QVariant::fromValue(static_cast<QSpinBox *>(d->editor)->value());
}

void dtkPropertyEditorInteger::onValueChanged(void)
{
    d->object->setProperty(d->meta_property.name(), this->editorData());
}

dtkPropertyEditor *createDtkPropertyEditorInteger(const QString& property_name, QObject *object, QWidget *parent)
{
    return new dtkPropertyEditorInteger(property_name, object, parent);
}

// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditorString implementation
// ///////////////////////////////////////////////////////////////////

dtkPropertyEditorString::dtkPropertyEditorString(const QString& property_name, QObject *object, QWidget *parent) : dtkPropertyEditor(property_name, object, parent)
{
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName(property_name);
    edit->setEnabled(true);
    edit->setText(object->property(d->meta_property.name()).toString());

    QObject::connect(edit, SIGNAL(returnPressed()), this, SLOT(onTextChanged()));

    d->editor = edit;

    d->layout = new QHBoxLayout;
    d->layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(d->layout);
    d->layout->addWidget(d->editor);
}

dtkPropertyEditorString::~dtkPropertyEditorString(void)
{

}

void dtkPropertyEditorString::setEditorData(const QVariant& data)
{
    d->editor->blockSignals(true);
    static_cast<QLineEdit *>(d->editor)->setText(data.toString());
    d->editor->blockSignals(false);
}

QVariant dtkPropertyEditorString::editorData(void)
{
    return QVariant::fromValue(static_cast<QLineEdit *>(d->editor)->text());
}

void dtkPropertyEditorString::onTextChanged(void)
{
    d->object->setProperty(d->meta_property.name(), this->editorData());
}

dtkPropertyEditor *createDtkPropertyEditorString(const QString& property_name, QObject *object, QWidget *parent)
{
    return new dtkPropertyEditorString(property_name, object, parent);
}

// ///////////////////////////////////////////////////////////////////
// dtkPropertyEditorEnum implementation
// ///////////////////////////////////////////////////////////////////

dtkPropertyEditorEnum::dtkPropertyEditorEnum(const QString& property_name, QObject *object, QWidget *parent) : dtkPropertyEditor(property_name, object, parent)
{
    QComboBox *list = new QComboBox(parent);
    list->setObjectName(property_name);
    list->setEnabled(true);

    int current_value = object->property(d->meta_property.name()).toInt();

    QMetaEnum meta_enum = d->meta_property.enumerator();
    int count = meta_enum.keyCount();
    int current_index = 0;

    for (int i = 0; i < count; ++i) {
        list->addItem(meta_enum.key(i));

        if (meta_enum.value(i) == current_value)
            current_index = i;
    }

    list->setCurrentIndex(current_index);

    QObject::connect(list, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));

    d->editor = list;

    d->layout = new QHBoxLayout;
    d->layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(d->layout);
    d->layout->addWidget(d->editor);
}

dtkPropertyEditorEnum::~dtkPropertyEditorEnum(void)
{

}

void dtkPropertyEditorEnum::setEditorData(const QVariant& data)
{
    int current_index = -1;

    switch (data.type()) {

    case QMetaType::Int:
        current_index = data.toInt();
        break;

    case QMetaType::QString: {
        QMetaEnum meta_enum = d->meta_property.enumerator();
        int count = meta_enum.keyCount();
        QString name = data.toString();

        for (int i = 0; i < count; ++i) {
            if (QString(meta_enum.key(i)) == name) {
                current_index = i;
                break;
            }
        }

        break;
    }

    default:
        return;
        break;
    }

    d->editor->blockSignals(true);
    static_cast<QComboBox *>(d->editor)->setCurrentIndex(current_index);
    d->editor->blockSignals(false);
}

QVariant dtkPropertyEditorEnum::editorData(void)
{
    return QVariant::fromValue(static_cast<QComboBox *>(d->editor)->currentIndex());
}

void dtkPropertyEditorEnum::onIndexChanged(int index)
{
    int value = d->object->metaObject()->property(d->meta_property.propertyIndex()).enumerator().value(index);
    d->object->setProperty(d->meta_property.name(), QVariant::fromValue(value));
}

dtkPropertyEditor *createDtkPropertyEditorEnum(const QString& property_name, QObject *object, QWidget *parent)
{
    return new dtkPropertyEditorEnum(property_name, object, parent);
}

