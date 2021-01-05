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

#include "dtkWidgets.h"

#include <dtkWidgetsConfig>

#include "dtkWidgetsParameterBoolCheckBox.h"
#include "dtkWidgetsParameterBoolPushButton.h"
#include "dtkWidgetsParameterDirBrowse.h"
#include "dtkWidgetsParameterDoubleSlider.h"
#include "dtkWidgetsParameterDoubleSpinBox.h"
#include "dtkWidgetsParameterFileBrowse.h"
#include "dtkWidgetsParameterFileSave.h"
#include "dtkWidgetsParameterIntSlider.h"
#include "dtkWidgetsParameterIntSpinBox.h"
#include "dtkWidgetsParameterLongLongSpinBox.h"
#include "dtkWidgetsParameterScientificSpinBox.h"
#include "dtkWidgetsParameterStringLineEdit.h"
#include "dtkWidgetsParameterStringListComboBox.h"

#include "dtkWidgetsParameterFactory.h"

#include "dtkWidgetsWorkspace.h"

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// Layer methods implementations
// /////////////////////////////////////////////////////////////////

namespace dtk {

namespace widgets {

DTK_DEFINE_LAYER_MANAGER;

void initialize(void)
{
    initialize(QString());
}

void initialize(const QString &path)
{
    QString realpath = path;
    QStringList pathslist;
    QSettings::setDefaultFormat(QSettings::IniFormat);

    if (path.isEmpty()) {
        QSettings settings;
        settings.beginGroup("widgets");
        realpath = settings.value("plugins").toString();
        settings.endGroup();
        if (realpath.isEmpty()) {
            realpath = QDir(DTK_WIDGETS_INSTALL_PREFIX).filePath("plugins/");
            dtkDebug() << "no plugin path configured for dtkWidgets, use default:" << realpath;
        }
        pathslist = realpath.split(":");
    } else {
        pathslist = realpath.split(":");
    }
    for (const QString &v_path : pathslist) {
        manager().initialize(v_path);
    }

    // ///////////////////////////////////////////////////////////////////
    // Initializes default plugins
    // ///////////////////////////////////////////////////////////////////
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterBoolCheckBox",
                                                     dtkWidgetsParameterBoolCheckBoxCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterBoolPushButton",
                                                     dtkWidgetsParameterBoolPushButtonCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterDirBrowse",
                                                     dtkWidgetsParameterDirBrowseCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterDoubleSlider",
                                                     dtkWidgetsParameterDoubleSliderCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterDoubleSpinBox",
                                                     dtkWidgetsParameterDoubleSpinBoxCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterIntSlider",
                                                     dtkWidgetsParameterIntSliderCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterIntSpinBox",
                                                     dtkWidgetsParameterIntSpinBoxCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterFileBrowse",
                                                     dtkWidgetsParameterFileBrowseCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterFileSave",
                                                     dtkWidgetsParameterFileSaveCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterLongLongSpinBox",
                                                     dtkWidgetsParameterLongLongSpinBoxCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterScientificSpinBox",
                                                     dtkWidgetsParameterScientificSpinBoxCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterStringLineEdit",
                                                     dtkWidgetsParameterStringLineEditCreator);
    dtk::widgets::parameters::pluginFactory().record("dtkWidgetsParameterStringListComboBox",
                                                     dtkWidgetsParameterStringListComboBoxCreator);
}
void setVerboseLoading(bool b)
{
    manager().setVerboseLoading(b);
}

void setAutoLoading(bool auto_load)
{
    manager().setAutoLoading(auto_load);
}

void uninitialize(void)
{
    manager().uninitialize();
}

} // namespace widgets
} // namespace dtk

//
// dtkWidgets.cpp ends here
