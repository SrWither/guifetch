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
