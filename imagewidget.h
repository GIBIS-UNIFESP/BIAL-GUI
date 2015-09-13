#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "viewerinterface.h"

#include <QWidget>

namespace Ui {
  class ImageWidget;
}

class QGraphicsScene;

class ImageWidget : public QWidget, public ViewerInterface {
  Q_OBJECT

public:
  explicit ImageWidget( QWidget *parent = 0 );
  ~ImageWidget( );
  void hideControls();
  void showControls();
  virtual void setBackgroundColor(const QColor & color);

  QGraphicsScene * scene() const;

private:
  Ui::ImageWidget *ui;
  QGraphicsScene * m_scene;
};

#endif /* IMAGEWIDGET_H */
