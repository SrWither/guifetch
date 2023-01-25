#ifndef GET_IMAGE_HPP
#define GET_IMAGE_HPP

#include <QObject>

class getImage : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString osImage READ osImage NOTIFY osImageChanged)

  public:
    explicit getImage(QObject *parent = nullptr);
    virtual ~getImage();

    QString osImage();

  signals:
    void osImageChanged();

  private:
    QString m_osImage;
};

QString getOsId();

#endif
