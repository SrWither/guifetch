#include <QDebug>
#include <getImage.hpp>

getImage::getImage(QObject *parent) : QObject(parent) {
  // Assign image from os-release id
  QString idOs = getOsId();
  m_osImage = QString("assets/%1.png").arg(idOs);
  /* m_osImage = QString("assets/alpine.png"); */
}

getImage::~getImage() { qDebug() << "Close image"; }

QString getOsId() {
  // Open /etc/os-release
  FILE *osrelease = fopen("/etc/os-release", "r");
  if (osrelease == NULL) {
    qDebug() << "Error al obtener informacion sobre el id del os";
  }

  char line[256];
  // Read lines
  while (fgets(line, sizeof(line), osrelease)) {
    char id;

    if (sscanf(line, "ID=%s", &id) == 1) {
      // Close file
      fclose(osrelease);
      int pkglength = strlen(&id);
      // QString from char
      QString osId = QString::fromUtf8(&id, pkglength);

      // Id with double quotes
      if (osId == "\"void\"") {
        return "void";
      }

      if (osId == "\"ghostbsd\"") {
        return "ghostbsd";
      }

      return osId;
    }
  }
  fclose(osrelease);
  // Generic image for Unknown distros
  return "generic";
}

// Data for qml
QString getImage::osImage() { return m_osImage; }
