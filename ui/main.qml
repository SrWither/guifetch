import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
  id: mainWindow
  visible: true

  width: WINDOW_WIDTH
  height: WINDOW_HEIGHT

  background: Rectangle {
    color: "#303134"
  }

  Grid {
    id: mainGrid
    columns: 2
    spacing: 10
    anchors.fill: parent

    Rectangle {
      id: logoCol
      color: "#303134"
      width: parent.width/2
      height: parent.height
      Image {
        id: logo
        source: "assets/ArchLinux.png"
        width: parent.width - 20
        height: parent.height - 20
        y: 10
      }
    }

    Rectangle {
      id: dataCol
      color: "#303134"
      width: parent.width/2
      height: parent.height
      Grid {
        id: dataGrid
        columns: 1
        spacing: 2
        Text {
          id: dataOs
          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>OS:</b> %1 %2").arg(getData.osName).arg(getData.osArch)
        }
        Text {
          id: dataDistro
          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>Distro:</b> %1").arg(getData.osDistro)
        }
        Text {
          id: dataKernel
          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>Kernel:</b> %1").arg(getData.osKernel)
        }
        Text {
          id: dataUptime

          property var seconds: getData.getUptime()
          property var minutes: seconds / 60;
          property var hours: minutes / 60;

          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>Uptime:</b> %1 hours %2 minutes and %3 seconds").arg((hours).toFixed(0)).arg((minutes%60).toFixed(0)).arg((seconds%60).toFixed(0))
        }
        Text {
          id: dataShell
          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>Shell:</b> %1").arg(getData.osShell)
        }
        Text {
          id: dataMemory

          property var freeRam: getData.getFreeRam()
          property var totalRam: getData.getTotalRam()

          color: "#ffffff"
          font.pixelSize: 20
          text: qsTr("<b>Memory:</b> %1/%2").arg(totalRam - freeRam).arg(totalRam)
        }
        Timer {
          interval: 1000; running: true; repeat: true
          onTriggered: {
            dataMemory.freeRam = getData.getFreeRam()
            dataMemory.totalRam = getData.getTotalRam()
            dataUptime.seconds = getData.getUptime()
          }
        }
      }
    }
  }
}
