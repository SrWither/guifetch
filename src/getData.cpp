#include <QDebug>
#include <cpuInfo.hpp>
#include <cstdlib>
#include <getData.hpp>
#include <gf_utils.hpp>
#include <iostream>
#include <sys/sysinfo.h>
#include <sys/utsname.h>


getData::getData(QObject *parent) : QObject(parent) {
  struct utsname name;

  uname(&name);
  // Distro information from sys/utsname.h
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

getData::~getData() { qDebug() << "Bye!"; }

// Linux code
#ifdef __linux__

//Linux free ram from /proc/meminfo
int getData::getFreeRam() {
  // Open /proc/meminfo
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (meminfo == NULL) {
    qDebug() << "Error al obtener informacion sobre la memoria ram";
  }

  char line[256];
  // Read lines
  while (fgets(line, sizeof(line), meminfo)) {
    int freeram;

    if (sscanf(line, "MemAvailable: %d kB", &freeram) == 1) {
      // Close file
      fclose(meminfo);
      // Assign value
      m_osFreeMemory = freeram / 1024;
      return m_osFreeMemory;
    }
  }

  fclose(meminfo);
  return -1;
}

//Linux total ram from /proc/meminfo
int getData::getTotalRam() {
  // Open file
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (meminfo == NULL) {
    qDebug() << "Error al obtener informacion sobre la memoria ram";
  }

  char line[256];
  // Read lines
  while (fgets(line, sizeof(line), meminfo)) {
    int totalram;

    if (sscanf(line, "MemTotal: %d kB", &totalram) == 1) {
      // Close file
      fclose(meminfo);
      // Assign value
      m_osTotalMemory = totalram / 1024;
      return m_osTotalMemory;
    }
  }

  fclose(meminfo);
  return -1;
}

// FreeBSD code
#elif __FreeBSD__

// FreeBSD free ram from sysctl
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

// FreeBSD total ram from sysctl command
int getData::getTotalRam() {
  std::string ram = exec("sysctl -n hw.physmem");
  unsigned long totalram = std::stoul(ram) / 1024 / 1024;
  m_osTotalMemory = totalram;

  return m_osTotalMemory;
}

#endif

// Uptime info from sys/sysinfo.h
int getData::getUptime() {
  struct sysinfo info;
  sysinfo(&info);

  m_osUptime = info.uptime;

  return m_osUptime;
}

QString getData::getDistro() {
  // Open os-release
  FILE *osrelease = fopen("/etc/os-release", "r");
  if (osrelease == NULL) {
    qDebug() << "Error al obtener informacion sobre la distribucion";
  }

  char line[256];
  // Read lines
  while (fgets(line, sizeof(line), osrelease)) {
    char drname;

    if (sscanf(line, "ID=%s", &drname) == 1) {
      // Close file
      fclose(osrelease);
      int pkglength = strlen(&drname);
      // QString from char
      QString distro = QString::fromUtf8(&drname, pkglength);

      // Distro name
      if (distro == "alpine") {
        return "Alpine Linux";
      }

      if (distro == "arch") {
        return "ArchLinux";
      }

      if (distro == "debian") {
        return "Debian";
      }

      if (distro == "fedora") {
        return "Fedora";
      }

      if (distro == "freebsd") {
        return "FreeBSD";
      }

      if (distro == "gentoo") {
        return "Gentoo";
      }

      if (distro == "linuxmint") {
        return "Linux Mint";
      }

      if (distro == "manjaro") {
        return "Manjaro Mint";
      }

      if (distro == "slackware") {
        return "Slackware";
      }

      if (distro == "ubuntu") {
        return "Ubuntu";
      }

      if (distro == "\"void\"") {
        return "Void Linux";
      }

      return "Unknown";
    }
  }

  fclose(osrelease);
  return "Distro not found";
}

// Cpu model and cores from cpuInfo.hpp
QString getData::getCpuInfo() {
  CPUInfo cpuinfo;
  QString model = QString(cpuinfo.model().c_str());
  QString cores = QString("%1").arg(cpuinfo.logicalCpus());

  m_osCpu = QString("%1 (%2)").arg(model).arg(cores);
  return m_osCpu;
}

// Data for qml
QString getData::osName() { return m_osName; }

QString getData::osDistro() { return m_osDistro; }

QString getData::osHost() { return m_osHost; }

QString getData::osKernel() { return m_osKernel; }

QString getData::osArch() { return m_osArch; }

int getData::osFreeMemory() { return m_osFreeMemory; }

int getData::osTotalMemory() { return m_osTotalMemory; }

QString getData::osShell() { return m_osShell; }

int getData::osUptime() { return m_osUptime; }

QString getData::osCpu() { return m_osCpu; }
