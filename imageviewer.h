#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "viewerinterface.h"
#include "displayformat.h"
#include "graphicsscene.h"
#include <QTime>
#include <QWidget>
#include <array>

class ImageWidget;
class QGridLayout;
class QGraphicsScene;
class Controller;

class ImageViewer : public QWidget, public ViewerInterface {
  Q_OBJECT
  std::array< ImageWidget*, 4 > views;
  QGridLayout *layout;
  Controller * controller;
  bool dragging;
  QTime timer;

  void getNewLayout();
public:
  explicit ImageViewer( QWidget *parent = 0 );
  ~ImageViewer( );

  void setViewBgColor( const QColor &color );
  bool eventFilter(QObject *obj, QEvent *evt);
  GraphicsScene * getScene(size_t axis);
  void setController(Controller * value);

signals:
//  void mouseClicked(QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved(QPointF pt, size_t axis );

private slots:
  void updateViews();
  void changeImage();

  void setLayoutType(Layout layout);
  void updateOverlay(QPointF pt, size_t axis );
  void setViewMode(Views views);

  void setGridLayout( );
  void setHorizontalLayout( );
  void setVerticalLayout( );

  void hideViews( );
  void showViews( );

  void setView0( );
  void setView1( );
  void setView2( );
  void setView3( );
  void setViews012( );
  void setViews123( );
  void setViews0123( );

  void sliceChanged(size_t axis, size_t slice);

public slots:

  void toggleOverlay();


  // QWidget interface
protected:
  void resizeEvent(QResizeEvent *);
};

#endif /* IMAGEVIEWER_H */
