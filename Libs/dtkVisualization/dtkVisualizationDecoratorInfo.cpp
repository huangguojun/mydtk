// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkVisualizationDecoratorInfo.h"

#include "dtkVisualizationCanvas.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <QtWidgets>
#include <dtkMeta>
#include <dtkThemes/dtkThemesEngine>

#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkFieldData.h>
#include <vtkPointData.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorInfoPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorInfoPrivate
{
public:
    dtkVisualizationDecoratorInfo *q = nullptr;
    vtkDataSet *dataset = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;
    QFormLayout *layout = nullptr;

public:
    void update(void);
};

void dtkVisualizationDecoratorInfoPrivate::update(void)
{
    if (!dataset)
        return;

    q->setInformation("Nb points:", QString::number(dataset->GetNumberOfPoints()));
    q->setInformation("Nb Cells:", QString::number(dataset->GetNumberOfCells()));

    QString scalar_color = dtkThemesEngine::instance()->value("@cyan");
    QString vector_color = dtkThemesEngine::instance()->value("@green");
    QString fields_tooltip = "<font color=\"" + scalar_color + "\">scalar</font> , <font color=\""
            + vector_color + "\">vector</font>";

    QStringList point_fields;
    for (int i = 0; i < dataset->GetPointData()->GetNumberOfArrays(); ++i) {
        switch (dataset->GetPointData()->GetArray(i)->GetNumberOfComponents()) {
        case 1: {
            QString s = "<font color=\"" + scalar_color + "\">"
                    + QString::fromUtf8(dataset->GetPointData()->GetArrayName(i)) + "</font>";

            point_fields << s;
            break;
        }

        case 3: {
            QString s = "<font color=\"" + vector_color + "\">"
                    + QString::fromUtf8(dataset->GetPointData()->GetArrayName(i)) + "</font>";

            point_fields << s;
            break;
        }
        default:
            point_fields << QString::fromUtf8(dataset->GetPointData()->GetArrayName(i));
        }
    }
    point_fields.sort();

    q->setInformation("PointFields:", point_fields.join("; "), fields_tooltip);

    QStringList cell_fields;
    for (int i = 0; i < dataset->GetCellData()->GetNumberOfArrays(); ++i) {
        switch (dataset->GetCellData()->GetArray(i)->GetNumberOfComponents()) {
        case 1: {
            QString s = "<font color=\"" + scalar_color + "\">"
                    + QString::fromUtf8(dataset->GetCellData()->GetArrayName(i)) + "</font>";

            cell_fields << s;
            break;
        }

        case 3: {
            QString s = "<font color=\"" + vector_color + "\">"
                    + QString::fromUtf8(dataset->GetCellData()->GetArrayName(i)) + "</font>";

            cell_fields << s;
            break;
        }
        default:
            cell_fields << QString::fromUtf8(dataset->GetCellData()->GetArrayName(i));
        }
    }
    cell_fields.sort();
    q->setInformation("CellFields:", cell_fields.join("; "), fields_tooltip);

    double bounds[6];
    dataset->GetBounds(bounds);
    QString bounds_s = "[" + QString::number(bounds[0], 'f', 2) + ", "
            + QString::number(bounds[1], 'f', 2) + ", " + QString::number(bounds[2], 'f', 2) + ", "
            + QString::number(bounds[3], 'f', 2) + ", " + QString::number(bounds[4], 'f', 2) + ", "
            + QString::number(bounds[5], 'f', 2) + "]";
    q->setInformation("Bounds:", bounds_s, "[xmin, xmax, ymin, ymax, zmin, zmax]");
    // dataset->GetCellTypes()
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecorator implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorInfo::dtkVisualizationDecoratorInfo(void)
    : dtkVisualizationDecorator(), d(new dtkVisualizationDecoratorInfoPrivate)
{
    d->q = this;

    this->setObjectName("Info");

    //////////
    // Inspectors creation and setup

    QWidget *infos_widget = new QWidget;
    d->layout = new QFormLayout;

    infos_widget->setLayout(d->layout);

    infos_widget->setObjectName("");

    d->inspectors << infos_widget;
}

dtkVisualizationDecoratorInfo::~dtkVisualizationDecoratorInfo(void)
{
    d->dataset = nullptr;
    d->view = nullptr;
    delete d;
    d = nullptr;
}

bool dtkVisualizationDecoratorInfo::isDecorating(void)
{
    return d->dataset;
}

void dtkVisualizationDecoratorInfo::setVisibility(bool visible)
{
    for (QWidget *w : d->inspectors) {
        w->setVisible(visible);
    }
}

void dtkVisualizationDecoratorInfo::setData(const QVariant &data)
{
    d->dataset = data.value<vtkDataSet *>();

    if (d->dataset) {
        for (int row = 0; row < d->layout->rowCount(); ++row) {
            d->layout->removeRow(row);
        }
        d->update();
    }
}

void dtkVisualizationDecoratorInfo::setCanvas(dtkVisualizationCanvas *canvas)
{
    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO
                   << "View 2D or view 3D expected as canvas. Canvas is reset "
                      "to nullptr.";
        return;
    }
}

void dtkVisualizationDecoratorInfo::setFileInfo(const QFileInfo &f_info)
{
    this->setInformation("Name:", f_info.fileName());
    this->setInformation("Path:", f_info.absolutePath());
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    this->setInformation(
            "Creation Time",
            f_info.fileTime(QFileDevice::FileBirthTime).toString("dd-MMM-yyyy: HH:mm"));
#endif
    double s = f_info.size();
    QString unit = " B";
    if (s > 1000.) {
        s /= 1000.;
        unit = " KB";
    }
    if (s > 1000.) {
        s /= 1000.;
        unit = " MB";
    }
    if (s > 1000.) {
        s /= 1000.;
        unit = " GB";
    }

    this->setInformation("Size:", QString::number(s, 'f', 2) + unit); // in bytes
}

void dtkVisualizationDecoratorInfo::setInformation(const QString &key, const QString &value,
                                                   const QString &tooltip)
{
    // if(!d->view) {
    //     qDebug() << Q_FUNC_INFO << "setCanvas must be called first";
    //     return;
    // }

    QLabel *v = new QLabel(value);
    v->setWordWrap(true);

    if (!tooltip.isEmpty())
        v->setToolTip(tooltip);

    this->setInformation(key, v);
}

void dtkVisualizationDecoratorInfo::setInformation(const QString &key, QWidget *value)
{
    d->layout->addRow(key, value);
}

void dtkVisualizationDecoratorInfo::touch(void) {}

void dtkVisualizationDecoratorInfo::unsetCanvas(void)
{
    d->view = nullptr;
}

QVariant dtkVisualizationDecoratorInfo::data(void) const
{
    /*
    if (d->dataset) {
        return dtk::variantFromValue(d->dataset);

    } else {
        return QVariant();
    }*/
}

dtkVisualizationCanvas *dtkVisualizationDecoratorInfo::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorInfo::inspectors(void) const
{
    return d->inspectors;
}
