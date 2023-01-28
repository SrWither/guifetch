#ifndef GET_DATA_HPP
#define GET_DATA_HPP

#include <QObject>

class getData : public QObject {
  Q_OBJECT
  // Qml Properties
  Q_PROPERTY(QString osName READ osName NOTIFY osNameChanged)
  Q_PROPERTY(QString osDistro READ osDistro NOTIFY osDistroChanged)
  Q_PROPERTY(QString osHost READ osHost NOTIFY osHostChanged)
  Q_PROPERTY(QString osKernel READ osKernel NOTIFY osKernelChanged)
  Q_PROPERTY(QString osArch READ osArch NOTIFY osArchChanged)
  Q_PROPERTY(int osFreeMemory READ osFreeMemory NOTIFY osFreeMemoryChanged)
  Q_PROPERTY(int osTotalMemory READ osTotalMemory NOTIFY osTotalMemoryChanged)
  Q_PROPERTY(QString osShell READ osShell NOTIFY osShellChanged)
  Q_PROPERTY(int osUptime READ osUptime NOTIFY osUptimeChanged)
  Q_PROPERTY(QString osCpu READ osCpu NOTIFY osCpuChanged)
  /* Q_PROPERTY(QString osPackagesInstalled READ osPackagesInstalled NOTIFY
   * osPackagesInstalledChanged) */

public:
  explicit getData(QObject *parent = nullptr);
  virtual ~getData();

  // Functions for Qml
  Q_INVOKABLE int getFreeRam();
  Q_INVOKABLE int getTotalRam();
  Q_INVOKABLE int getUptime();
  Q_INVOKABLE QString getDistro();
  Q_INVOKABLE QString getCpuInfo();
  /* Q_INVOKABLE QString getPackageManager(); */

  // Data for qml
  QString osName();
  QString osDistro();
  QString osHost();
  QString osKernel();
  QString osArch();
  int osFreeMemory();
  int osTotalMemory();
  QString osShell();
  int osUptime();
  QString osCpu();
  /* QString osPackagesInstalled(); */

signals:
  void osNameChanged();
  void osDistroChanged();
  void osHostChanged();
  void osKernelChanged();
  void osArchChanged();
  void osFreeMemoryChanged();
  void osTotalMemoryChanged();
  void osShellChanged();
  void osUptimeChanged();
  void osCpuChanged();
  /* void osPackagesInstalledChanged */

private:
  QString m_osName;
  QString m_osDistro;
  QString m_osHost;
  QString m_osKernel;
  QString m_osArch;
  unsigned long m_osFreeMemory;
  unsigned long m_osTotalMemory;
  QString m_osShell;
  int m_osUptime;
  QString m_osCpu;
  /* QString m_osPackagesInstalled; */
};

#endif
