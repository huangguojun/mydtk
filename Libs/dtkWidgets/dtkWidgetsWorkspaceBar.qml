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

import QtQuick 2.9
import QtQuick.Controls 2.0

Rectangle {

    id: root;

    color: theme.value("@bg");

    signal clicked(string name);

    // /////////////////////////////////////////////////////////////////////////////
    // Components
    // /////////////////////////////////////////////////////////////////////////////

    Component {
        id: delegate;

        Rectangle {

            id: delegate_r;

            color: theme.value("@bgalt");

            border.width: 1;
            border.color: theme.value("@bd");

            height: 80;
            width: parent.width;

            Column {

                anchors.fill: parent;

                Text {
                    text: title;

                    padding: 5;

                    width: parent.width;

                    height: 30;

                    color: theme.value("@fg");

                    Image {
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.right: parent.right;
                        anchors.rightMargin: 10;

                        visible: view.currentIndex == 0;

                        source: "ast/next.png";
                        height: 15;
                        width: 9;
                    }
                }

                TextArea {
                    width: parent.width;

                    height: 50;

                    color: theme.value("@fgalt");

                    text: description;

                    font.pointSize: 9;

                    wrapMode: TextEdit.WordWrap;
                }
            }

            MouseArea {
                anchors.fill: parent;

                onClicked: {

                    if(view.currentIndex == 1) {
                        root.clicked(title);
                    }

                    if(view.currentIndex == 0) {

                        level2.visible = true;

                        model2.clear();

                        for (var prop in groups[title]) {
                            model2.append({
                                "title": groups[title][prop].toString(),
                                "description": works_descs[groups[title][prop]].toString()
                            });
                        }

                        view.currentIndex = view.currentIndex + 1;

                        for(var child in level1.contentItem.children) {
                            level1.contentItem.children[child].color = theme.color("@bgalt");
                        }

                        delegate_r.color = theme.value("@bg");
                    }
                }
            }

        }
    }

    // /////////////////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////////////////

    Column {

        anchors.fill: parent;

        Rectangle {

            id: header;

            property alias text: label.text;

            width: parent.width;
            height: 25;

            color: theme.value("@bg");

            Text {

                id: label;

                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;

                color: theme.value("@fg");

                text: "Choose package";
            }

            Image {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left;
                anchors.leftMargin: 10;

                visible: view.currentIndex == 1;

                source: "ast/prev.png";
                height: 15;
                width: 9;

                MouseArea {

                    anchors.fill: parent;

                    onClicked: view.currentIndex = view.currentIndex - 1;
                }
            }
        }

        SwipeView {
            id: view

            width: parent.width;
            height: parent.height - header.height - footer.height;

            clip: true;

            ListView {

                id: level1;

                visible: true;

                delegate: delegate;

                model: ListModel { id: model1; }

                ScrollBar.vertical: ScrollBar {
                    id: vbar
                    active: true;
                    visible: true;
                    hoverEnabled: true
                    // active: hovered || pressed
                    orientation: Qt.Vertical
                    size: view.height / level1.height
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

                Component.onCompleted: {

                    for (var prop in groups) {
                        model1.append({
                                  "title": prop.toString(),
                            "description": group_descs[prop].toString()
                        });
                    }
                }
            }

            ListView {

                id: level2;

                visible: false;

                delegate: delegate;

                model: ListModel { id: model2; }

                ScrollBar.vertical: ScrollBar {
                    id: vbar2
                    visible: true;
                    hoverEnabled: true
                    active: true; // hovered || pressed
                    orientation: Qt.Vertical
                    size: view.height / level2.height
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

            }
        }

        Rectangle {

            id: footer;

            width: parent.width;
            height: 25;

            color: theme.value("@bg");

            PageIndicator {
                id: indicator

                count: view.currentIndex + 1;
                currentIndex: view.currentIndex;

                anchors.centerIn: parent;

                delegate: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8

                    radius: width / 2
                    color: theme.color("@fg");

                    opacity: index === indicator.currentIndex ? 1.0 : 0.35

                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 100
                        }
                    }
                }

                onCurrentIndexChanged: {
                    if(currentIndex == 0)
                        header.text = "Choose Package";
                    else
                        header.text = "Choose Workspace";
                }
            }
        }
    }
}

//
// dtkWidgetsWorkspaceBar.qml ends here
