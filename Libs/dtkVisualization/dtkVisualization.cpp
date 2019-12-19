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

#include "dtkVisualization.h"

//#include "dtkVisualizationMetaType.h"

//#include <dtkVisualizationConfig.h>

//#include <dtkCore>

namespace dtk { 
namespace visualization {

  //  DTK_DEFINE_LAYER_MANAGER;

 /*   void activateObjectManager(void) {
        manager().setObjectManager(dtkCoreObjectManager::instance());
    }

 */
    void initialize(void)
    {
        //initialize(QString());
    }

    void initialize(const QString& s)
    {
        /*
        dtk::visualization::registerVTKToMetaType();

        QString path = s;
        QStringList pathslist;

        if (path.isEmpty()) {
            //per default, it will use
            // app->name
            // app->organization
            // user_space
            // ini format since setDefaultFormat is set in dtkApplicationPrivate::initialize
            QSettings settings;
            path = settings.value("dtk-visualization/plugins").toString();
            if (path.isEmpty()) {
                path = QDir::cleanPath(DTKVISUALIZATION_INSTALL_PREFIX + "/plugins/dtkVisualization");
                dtkDebug() << Q_FUNC_INFO << "No plugin path configured, use default:" << path;
            }
        }
        pathslist = path.split(":");

        for (auto p : pathslist) {
            manager().initialize(p);
        }
        */
    }

    void uninitialize(void)
    {
       // manager().uninitialize();
    }

    void setVerboseLoading(bool b)
    {
        //manager().setVerboseLoading(b);
    }
} };

//
// dtkVisualization.cpp ends here
