#include <getData.hpp>
#include <QDebug>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <cstdlib>
#include <gf_utils.hpp>
#include <cpuInfo.hpp>

getData::getData(QObject *parent) : QObject(parent) {
  struct utsname name;

  uname(&name);

  m_osName = name.sysname;
  m_osDistro = getDistro();
  m_osHost = name.nodename;
  m_osKernel = name.release;
  m_osArch = name.machine;
  getFreeRam();
  getTotalRam();
  m_osShell = getenv("SHELL");
  getCpuInfo();
  /* getPackageManager(); */
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

QString getData::getCpuInfo() {
  CPUInfo cpuinfo;
  QString model = QString(cpuinfo.model().c_str());
  QString cores = QString("%1").arg(cpuinfo.logicalCpus());

  m_osCpu = QString("%1 (%2)").arg(model).arg(cores);
  return m_osCpu;
}

// FreeBSD
#elif __FreeBSD__

int getData::getFreeRam() {
  std::string pagesize = exec("sysctl -n hw.pagesize");
  std::string inactiveCount = exec("sysctl -n vm.stats.vm.v_inactive_count");
  std::string freeCount = exec("sysctl -n vm.stats.vm.v_free_count");
  std::string cacheCount = exec("sysctl -n vm.stats.vm.v_cache_count");

  unsigned long hw_pagesize = std::stoul(pagesize);
  unsigned long mem_inactive = std::stoul(inactiveCount) * hw_pagesize;
  unsigned long mem_unused = std::stoul(freeCount) * hw_pagesize;
  unsigned long mem_cache = std::stoul(cacheCount) * hw_pagesize;

  int mem_free = (mem_inactive + mem_unused + mem_cache) / 1024 / 1024;

  m_osFreeMemory = mem_free;

  return m_osFreeMemory;
}

int getData::getTotalRam() {
  std::string ram = exec("sysctl -n hw.physmem");
  unsigned long totalram = std::stoul(ram) / 1024 / 1024;
  m_osTotalMemory = totalram;

  return m_osTotalMemory;
}

#endif

int getData::getUptime() {
  struct sysinfo info;
  sysinfo(&info);

  m_osUptime = info.uptime;

  return m_osUptime;
}

QString getData::getDistro() {
  FILE *osrelease = fopen("/etc/os-release", "r");
  if(osrelease == NULL){
    qDebug() << "Error al obtener informacion sobre la distribucion";
  }

  char line[256];
  while(fgets(line, sizeof(line), osrelease)){
    char drname;

    if(sscanf(line, "ID=%s", &drname) == 1) {
      fclose(osrelease);
      int pkglength = strlen(&drname);
      QString distro = QString::fromUtf8(&drname, pkglength);

      if(distro == "alpine") {
        return "Alpine Linux";
      }

      if(distro == "arch") {
        return "ArchLinux";
      }

      if(distro == "debian") {
        return "Debian";
      }

      if(distro == "fedora") {
        return "Fedora";
      }

      if(distro == "freebsd") {
        return "FreeBSD";
      }

      if(distro == "gentoo") {
        return "Gentoo";
      }

      if(distro == "linuxmint") {
        return "Linux Mint";
      }

      if(distro == "manjaro") {
        return "Manjaro Mint";
      }

      if(distro == "slackware") {
        return "Slackware";
      }

      if(distro == "ubuntu") {
        return "Ubuntu";
      }

      if(distro == "void") {
        return "Void Linux";
      }

      return "Unknown";
    }
  }

  fclose(osrelease);
  return "Distro not found";
}

QString getData::osName() {
  return m_osName;
}

QString getData::osDistro() {
  return m_osDistro;
}

QString getData::osHost() {
  return m_osHost;
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

QString getData::osCpu() {
  return m_osCpu;
}
