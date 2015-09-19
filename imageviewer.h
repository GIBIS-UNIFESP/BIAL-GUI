#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "viewerinterface.h"
#include "displayformat.h"
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

  void getNewLayout();
public:
  explicit ImageViewer( QWidget *parent = 0 );
  ~ImageViewer( );

  void setViewBgColor( const QColor &color );
  bool eventFilter(QObject *obj, QEvent *evt);
  QGraphicsScene * getScene(size_t axis);
  void setController(Controller * value);
signals:
  void updateStatus(QString text, int timeout = 0 );
  void mouseClicked(QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved(QPointF pt, size_t axis );

private slots:
  void updateViews();
  void changeImage();
  void updateOverlay(QPointF pt, size_t axis );

public slots:
  void setLayoutType(Layout layout);
  void setGridLayout( );
  void setHorizontalLayout( );
  void setVerticalLayout( );

  void hideViews( );
  void showViews( );

  void setViewMode(Views views);
  void setView0( );
  void setView1( );
  void setView2( );
  void setView3( );
  void setViews012( );
  void setViews123( );
  void setViews0123( );

  void toggleOverlay();

  // QWidget interface
protected:
  void resizeEvent(QResizeEvent *);
};

#endif /* IMAGEVIEWER_H */
