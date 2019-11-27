import QtQuick 2.0

Item {
    id: dtkSwitch;
    objectName: "dtkSwitch";

    width: background.width;
    height: background.height;

    property bool on: false;

    signal toggled(bool on);

    function init() {
        dtkSwitch.state = "on";
    }

    function toggle() {
        if (dtkSwitch.state == "on") {
            dtkSwitch.state = "off";
        } else {
            dtkSwitch.state = "on";
        }

        toggled(dtkSwitch.state == "on");
    }

    function releaseSwitch() {
        if (knob.x == 0) {
            if (dtkSwitch.state == "off") return;
        }
        if (knob.x == (background.width - knob.width)) {
            if (dtkSwitch.state == "on") return;
        }
        toggle();
    }

    Image {
        id: background;
        source: wrapper.background();

        MouseArea {
            anchors.fill: parent;
            onClicked: toggle();
        }

        Text {
            id: l_label;

            color: "white";
            text: wrapper.leftLabel();
            font.pointSize: 24;
            font.bold: true;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.topMargin: 10;
            anchors.leftMargin: 15;

            width: background.width/2;
        }

        Text {
            id: r_label;

            color: "white";
            text: wrapper.rightLabel();
            font.pointSize: 24;
            font.bold: true;

            anchors.top: parent.top;
            anchors.left: l_label.right;
            anchors.topMargin: 10;
            anchors.rightMargin: 15;

            width: background.width/2;
        }
    }

    Image {
        id: knob;
        x: (background.width - knob.width - 2);
        y: (background.height - knob.height) / 2;
        source: wrapper.foreground();

        MouseArea {
            anchors.fill: parent;
            drag.target: knob;
            drag.axis: Drag.XAxis;
            drag.minimumX: 0;
            drag.maximumX: background.width - knob.width;
            onClicked: toggle();
            onReleased: releaseSwitch();
        }
    }

    states: [
        State {
            name: "on"
            PropertyChanges { target: knob; x: background.width - knob.width; }
            PropertyChanges { target: dtkSwitch; on: true; }
        },
        State {
            name: "off"
            PropertyChanges { target: knob; x: 0; }
            PropertyChanges { target: dtkSwitch; on: false; }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200; }
    }
}
