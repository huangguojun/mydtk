// Version: $Id:
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsParameterMenuBarGenerator.h"

#include "dtkWidgetsMenu.h"
#include "dtkWidgetsMenuBar.h"
#include "dtkWidgetsMenuBar_p.h"
#include "dtkWidgetsParameter.h"
#include "dtkWidgetsParameterFactory.h"

#include <dtkCore/dtkCoreParameter>

#include <dtkFonts/dtkFontAwesome>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterMenuBarGeneratorPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterMenuBarGeneratorPrivate
{
public:
    template<class T>
    void parseGroup(T *, const QJsonObject &);
    void parseParameter(dtkWidgetsMenu *, const QJsonObject &);

public:
    QJsonArray menu;

public:
    dtkCoreParameters parameters;
};

// ///////////////////////////////////////////////////////////////////

void dtkWidgetsParameterMenuBarGeneratorPrivate::parseParameter(dtkWidgetsMenu *menu,
                                                                const QJsonObject &parameter_object)
{
    QString widget_string = parameter_object["widget"].toString();
    QString parameter_uid = parameter_object["uid"].toString();
    dtkCoreParameter *parameter = this->parameters[parameter_uid];
    parameter->setAdvanced(parameter_object["advanced"].toBool());

    menu->addParameter(parameter->label(), parameter, widget_string);
}

template<class T>
void dtkWidgetsParameterMenuBarGeneratorPrivate::parseGroup(T *menu,
                                                            const QJsonObject &group_object)
{
    QString group_icon = group_object["icon"].toString();
    QString group_uid = group_object["uid"].toString();
    bool advanced = group_object["advanced"].toBool();

    auto group_menu =
            menu->addMenu(dtkFontAwesome::instance()->namedCodePoints()[group_icon], group_uid);

    if (!group_menu)
        return;

    QJsonArray group_contents = group_object["contents"].toArray();
    for (auto it = group_contents.begin(); it != group_contents.end(); ++it) {
        if (it->isObject()) {
            QJsonObject content_object = it->toObject();
            if (content_object.contains("group")) {
                QJsonObject subgroup_object = content_object["group"].toObject();
                parseGroup(group_menu, subgroup_object);
            } else if (content_object.contains("parameter")) {
                QJsonObject parameter_object = content_object["parameter"].toObject();
                parseParameter(group_menu, parameter_object);
            } else {
                qWarning() << Q_FUNC_INFO
                           << "'contents' array contains an object with "
                              "unhandlded key. The entry is ignored.";
            }
        } else {
            qWarning() << Q_FUNC_INFO
                       << "'contents' sections are expected to contain objects "
                          "only. Non object entry is ignored.";
        }
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterMenuBarGenerator implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterMenuBarGenerator::dtkWidgetsParameterMenuBarGenerator(
        const QString &menu_file_path, const QString &definition_file_path)
    : d(new dtkWidgetsParameterMenuBarGeneratorPrivate)
{
    QFile menu_file(menu_file_path);

    d->parameters = dtk::core::readParameters(definition_file_path);

    if (d->parameters.count() == 0) {
        qWarning() << Q_FUNC_INFO << "The file" << definition_file_path
                   << "could not be read: no parameter found";
        return;
    }

    if (menu_file.open(QFile::ReadOnly)) {
        QJsonParseError menu_error;
        QJsonDocument menu_document = QJsonDocument::fromJson(menu_file.readAll(), &menu_error);
        if (menu_error.error != QJsonParseError::NoError) {
            qWarning() << Q_FUNC_INFO << "Error :" << menu_error.errorString() << "parsing"
                       << menu_file_path << "offset :" << menu_error.offset << ".";
            return;
        }

        QJsonObject menu_object = menu_document.object();
        if (!menu_object.contains("contents")) {
            qWarning() << Q_FUNC_INFO
                       << "The menu parameters file should contain a first "
                          "json object with key 'contents'. The file cannot be "
                          "processed any further.";
            return;
        }

        QJsonValue menu_contents = menu_object["contents"];
        if (menu_contents.isArray()) {
            d->menu = menu_contents.toArray();
        } else {
            qWarning() << Q_FUNC_INFO << "The first object with key 'contents' must be an array.";
        }
    } else {
        qWarning() << Q_FUNC_INFO << "The file" << menu_file_path << "could not be opened.";
    }
}

dtkWidgetsParameterMenuBarGenerator::~dtkWidgetsParameterMenuBarGenerator(void)
{
    delete d;
}

void dtkWidgetsParameterMenuBarGenerator::populate(dtkWidgetsMenu *menu_to_fill)
{
    for (auto it = d->menu.begin(); it != d->menu.end(); ++it) {
        if (it->isObject()) {
            QJsonObject content_object = it->toObject();
            if (content_object.contains("group")) {
                QJsonObject group_object = content_object["group"].toObject();
                d->parseGroup(menu_to_fill, group_object);
            } else {
                qWarning() << Q_FUNC_INFO
                           << "'contents' sections are expected to contain "
                              "objects only. Non object entry is ignored.";
            }
        } else {
            qWarning() << Q_FUNC_INFO
                       << "'contents' sections are expected to contain objects "
                          "only. Non object entry is ignored.";
        }
    }
}

void dtkWidgetsParameterMenuBarGenerator::populate(dtkWidgetsMenuBar *menubar)
{
    for (auto it = d->menu.begin(); it != d->menu.end(); ++it) {
        if (it->isObject()) {
            QJsonObject content_object = it->toObject();
            if (content_object.contains("group")) {
                QJsonObject group_object = content_object["group"].toObject();
                d->parseGroup(menubar, group_object);
            } else {
                qWarning() << Q_FUNC_INFO
                           << "'contents' sections are expected to contain "
                              "objects only. Non object entry is ignored.";
            }
        } else {
            qWarning() << Q_FUNC_INFO
                       << "'contents' sections are expected to contain objects "
                          "only. Non object entry is ignored.";
        }
    }
}

void dtkWidgetsParameterMenuBarGenerator::populate(dtkWidgetsMenuBarContainer *container)
{
    dtkWidgetsMenuBar *dummy = new dtkWidgetsMenuBar;

    for (auto it = d->menu.begin(); it != d->menu.end(); ++it) {
        if (it->isObject()) {
            QJsonObject content_object = it->toObject();
            if (content_object.contains("group")) {
                QJsonObject group_object = content_object["group"].toObject();
                d->parseGroup(dummy, group_object);
            } else {
                qWarning() << Q_FUNC_INFO
                           << "'contents' sections are expected to contain "
                              "objects only. Non object entry is ignored.";
            }
        } else {
            qWarning() << Q_FUNC_INFO
                       << "'contents' sections are expected to contain objects "
                          "only. Non object entry is ignored.";
        }
    }

    container->build(dummy->menus());
}

dtkCoreParameters dtkWidgetsParameterMenuBarGenerator::parameters(void) const
{
    return d->parameters;
}

//
// dtkWidgetsParameterMenuBarGenerator.cpp ends here
