#include <getData.hpp>
#include <QDebug>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <cstdlib>

getData::getData(QObject *parent) : QObject(parent) {
  struct utsname name;

  uname(&name);

  m_osName = name.sysname;
  m_osDistro = name.nodename;
  m_osKernel = name.release;
  m_osArch = name.machine;
  getFreeRam();
  getTotalRam();
  m_osShell = getenv("SHELL");
}

getData::~getData() {
  qDebug() << "Bye!";
}

//Linux
#ifdef __linux__

int getData::getFreeRam() {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if(meminfo == NULL){
    qDebug() << "Error al obtener informacion sobre la memoria ram";
  }

  char line[256];
  while(fgets(line, sizeof(line), meminfo)){
    int freeram;

    if(sscanf(line, "MemAvailable: %d kB", &freeram) == 1) {
      fclose(meminfo);
      m_osFreeMemory = freeram / 1024;
      return m_osFreeMemory;
    }
  }

  fclose(meminfo);
  return -1;
}

int getData::getTotalRam() {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if(meminfo == NULL){
    qDebug() << "Error al obtener informacion sobre la memoria ram";
  }

  char line[256];
  while(fgets(line, sizeof(line), meminfo)){
    int totalram;

    if(sscanf(line, "MemTotal: %d kB", &totalram) == 1) {
      fclose(meminfo);
      m_osTotalMemory = totalram / 1024;
      return m_osTotalMemory;
    }
  }

  fclose(meminfo);
  return -1;
}

// FreeBSD
#elif __FreeBSD__

int getData::getFreeRam() {
  struct sysinfo info;
  sysinfo(&info);

  m_osFreeMemory = info.freeram + info.bufferram;

  return m_osFreeMemory / 1024;
}

int getData::getTotalRam() {
  struct sysinfo info;
  sysinfo(&info);

  m_osTotalMemory = info.totalram;

  return m_osTotalMemory / 1024;
}

#endif

int getData::getUptime() {
  struct sysinfo info;
  sysinfo(&info);

  m_osUptime = info.uptime;

  return m_osUptime;
}

QString getData::osName() {
  return m_osName;
}

QString getData::osDistro() {
  return m_osDistro;
}

QString getData::osKernel() {
  return m_osKernel;
}

QString getData::osArch() {
  return m_osArch;
}

int getData::osFreeMemory() {
  return m_osFreeMemory;
}

int getData::osTotalMemory() {
  return m_osTotalMemory;
}

QString getData::osShell() {
  return m_osShell;
}

int getData::osUptime() {
  return m_osUptime;
}
