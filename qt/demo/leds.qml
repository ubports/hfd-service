import QtQuick 2.7
import Ubuntu.Components 1.3
import Hfd 0.1

MainView {
    id: root
    objectName: 'mainView'
    applicationName: "app"
    automaticOrientation: true

    width: units.gu(45)
    height: units.gu(75)

    Page {
        anchors.fill: parent

        header: PageHeader {
            id: header
            title: i18n.tr("led test")
        }

        Column {
            anchors.fill: parent

            Button {
                text: "on"
                onClicked: Leds.state = Leds.state == 0 ? 1 : 0
            }

            Label {
                text: "state: " + Leds.state
            }
        }


    }
}
