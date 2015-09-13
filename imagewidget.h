#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "viewerinterface.h"

#include <QWidget>

namespace Ui {
  class ImageWidget;
}

class GraphicsScene;

class ImageWidget : public QWidget, public ViewerInterface {
  Q_OBJECT

public:
  explicit ImageWidget( QWidget *parent = 0 );
  ~ImageWidget( );
  void hideControls();
  void showControls();
  virtual void setBackgroundColor(const QColor & color);

  GraphicsScene * scene() const;

private:
  Ui::ImageWidget *ui;
  GraphicsScene * m_scene;
};

#endif /* IMAGEWIDGET_H */
