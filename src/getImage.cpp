#include <getImage.hpp>
#include <QDebug>

getImage::getImage(QObject *parent) : QObject(parent) {
  QString idOs = getOsId();
  m_osImage = QString("assets/%1.png").arg(idOs);
  /* m_osImage = QString("assets/alpine.png"); */
}

getImage::~getImage() {
  qDebug() << "Close image";
}

QString getOsId() {
  FILE *osrelease = fopen("/etc/os-release", "r");
  if(osrelease == NULL){
    qDebug() << "Error al obtener informacion sobre el id del os";
  }

  char line[256];
  while(fgets(line, sizeof(line), osrelease)){
    char id;

    if(sscanf(line, "ID=%s", &id) == 1) {
      fclose(osrelease);
      int pkglength = strlen(&id);
      QString osId = QString::fromUtf8(&id, pkglength);

      return osId;
    }
  }

  fclose(osrelease);
  return "generic";
}

QString getImage::osImage() {
  return m_osImage;
}
