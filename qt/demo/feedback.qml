import QtQuick 2.7
import Ubuntu.Components 1.3
import QtFeedback 5.0

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
            anchors {
                top: header.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            Button {
                text: "feedback"
                onClicked: haptic.start();
            }
        }

        HapticsEffect {
            id: haptic
            attackIntensity: 0.0
            attackTime: 50
            intensity: 1.0
            duration: 100
            fadeTime: 50
            fadeIntensity: 0.0
        }
    }
}
