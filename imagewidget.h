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
  void hideControls( );
  void showControls( );
  virtual void setViewBgColor( const QColor &color );

  QGraphicsScene* scene( ) const;

  size_t viewNumber( ) const;
  void setViewNumber( const size_t &viewNumber );

  void setRange(int start, int end);

  void setSlice(int slice);

  void fitInView(QRectF rect);

  void show();

signals:
  void sliceChanged( size_t viewNbr, size_t slice );

private slots:

  void on_spinBox_valueChanged(int position);

  void on_rotateButton_clicked();

  void on_horizontalSlider_valueChanged(int position);

private:
  Ui::ImageWidget *ui;
  QGraphicsScene *m_scene;
  size_t m_viewNumber;

};

#endif /* IMAGEWIDGET_H */
