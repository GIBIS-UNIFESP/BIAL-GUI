#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class Controller : public QObject {
  Q_OBJECT
public:
  explicit Controller( QObject *parent = 0 );

  int * CurrentImage();

signals:
  void imageChanged();

public slots:

};

#endif /* CONTROLLER_H */
