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

#include "dtkVisualizationTypeTraits.h"

#include <vtkDataSet.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>

#include <QtCore>

//#include <dtkCore/dtkCoreMetaType>

Q_DECLARE_METATYPE(vtkDataSet *);
Q_DECLARE_METATYPE(vtkImageData *);
Q_DECLARE_METATYPE(vtkPoints *);
Q_DECLARE_METATYPE(vtkPointSet *);
Q_DECLARE_METATYPE(vtkPolyData *);
Q_DECLARE_METATYPE(vtkRectilinearGrid *);
Q_DECLARE_METATYPE(vtkStructuredGrid *);
Q_DECLARE_METATYPE(vtkUnstructuredGrid *);

namespace dtk {
/*
    namespace detail
    {
        // Custom fallback to create QVariant from vtkDataSet data
        template <typename T>
        struct variant_handler<T, std::enable_if_t<dtk::is_vtkdataset<T>::value>>
        {
            static QVariant fromValue(const T& t) {
                QString class_name(t->GetClassName());
                int class_type = QMetaType::type(qPrintable(class_name + "*"));

                if (class_type == QMetaType::UnknownType) {
                    return QVariant::fromValue(t);
                }
                return QVariant(class_type, &t, 1);
            }
        };
    }
    */
}

//
// dtkVisualizationMetaType.tpp ends here
