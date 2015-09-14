#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "viewerinterface.h"

#include <QWidget>
#include <array>

class ImageWidget;
class QGridLayout;
class QGraphicsScene;

class ImageViewer : public QWidget, public ViewerInterface {
  Q_OBJECT
  std::array< ImageWidget*, 4 > views;
  QGridLayout *layout;

public:
  explicit ImageViewer( QWidget *parent = 0 );
  ~ImageViewer( );

  void setBackgroundColor( const QColor &color );
  bool eventFilter(QObject *obj, QEvent *evt);

signals:
  void updateStatus(QString text, int timeout = 0 );

public slots:
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
  void set0123Views( );

};

#endif /* IMAGEVIEWER_H */