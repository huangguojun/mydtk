
#include "dtkVisualizationWidgetsActorList.h"

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeCollection.h>
#include <vtkAssembly.h>
#include <vtkProperty.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsActorListPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsActorListPrivate : public QTreeWidget
{
public:
    vtkRenderer *renderer;

public:
    QList<QTreeWidgetItem *> items;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsActorList
// ///////////////////////////////////////////////////////////////////

dtkVisualizationWidgetsActorList::dtkVisualizationWidgetsActorList(QWidget *parent) : QFrame(parent), d(new dtkVisualizationWidgetsActorListPrivate)
{
    d->header()->setVisible(false);
    d->setColumnCount(1);

    d->renderer = nullptr;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d);
}

dtkVisualizationWidgetsActorList::~dtkVisualizationWidgetsActorList(void)
{
    delete d;
}

void dtkVisualizationWidgetsActorList::update(void)
{
    int a_count = 1;
    int v_count = 1;

    if(!d->renderer)
        return;

    d->clear();

    vtkActorCollection *actors = d->renderer->GetActors();
    vtkActor *actor = nullptr;

    for (int i = 0; i < actors->GetNumberOfItems(); i++)
    {
        if ((actor = vtkActor::SafeDownCast(actors->GetItemAsObject(i))))
        {
            d->items << new QTreeWidgetItem(d, QStringList() << QString("Actor %1").arg(a_count++), QTreeWidgetItem::UserType);
        }
    }

    vtkVolumeCollection *volumes = d->renderer->GetVolumes();
    vtkVolume *volume = nullptr;

    for (int i = 0; i < volumes->GetNumberOfItems(); i++)
    {
        if ((volume = vtkVolume::SafeDownCast(volumes->GetItemAsObject(i))))
        {
            d->items << new QTreeWidgetItem(d, QStringList() << QString("Volume %1").arg(v_count++), QTreeWidgetItem::UserType);
        }
    }

    vtkPropCollection *props = d->renderer->GetViewProps();
    props->InitTraversal();
    for(vtkIdType i = 0; i < props->GetNumberOfItems(); ++i) {
        vtkAssembly *assembly = vtkAssembly::SafeDownCast(props->GetNextProp());

        if(!assembly) continue;

        vtkPropCollection *assembly_actors = vtkPropCollection::New();
        assembly->GetActors(assembly_actors);
        assembly_actors->InitTraversal();
        for(vtkIdType j = 0; j < assembly_actors->GetNumberOfItems(); j++) {
            vtkActor *actor = vtkActor::SafeDownCast(assembly_actors->GetNextProp());

            if(!actor) continue;

            d->items << new QTreeWidgetItem(d, QStringList() << QString("Actor %1").arg(a_count++), QTreeWidgetItem::UserType);
        }
    }
}

void dtkVisualizationWidgetsActorList::setRenderer(vtkRenderer *renderer)
{
    d->renderer = renderer;
}

//
// dtkVisualizationWidgetsActorList.cpp ends here
