#include "imageviewer.h"
#include "imagewidget.h"

#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGridLayout>

#include <iostream>

ImageViewer::ImageViewer( QWidget *parent ) : QWidget( parent ) {
  for( size_t i = 0; i < views.size( ); ++i ) {
    views[ i ] = new ImageWidget( this );
    views[ i ]->hideControls( );
    views[ i ]->scene( )->installEventFilter( this );
  }
  layout = new QGridLayout( this );
  setGridLayout( );
}

ImageViewer::~ImageViewer( ) {

}


void ImageViewer::setBackgroundColor( const QColor &color ) {
  for( ImageWidget *view : views ) {
    view->setBackgroundColor( color );
  }
}


void ImageViewer::setGridLayout( ) {
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 1, 0 );
  layout->addWidget( views[ 3 ], 1, 1 );
}

void ImageViewer::setHorizontalLayout( ) {
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 0, 2 );
  layout->addWidget( views[ 3 ], 0, 3 );
}

void ImageViewer::setVerticalLayout( ) {
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 1, 0 );
  layout->addWidget( views[ 2 ], 2, 0 );
  layout->addWidget( views[ 3 ], 3, 0 );
}

void ImageViewer::hideViews( ) {
  for( ImageWidget *view : views ) {
    view->hide( );
  }
}

void ImageViewer::showViews( ) {
  for( ImageWidget *view : views ) {
    view->show( );
  }
}

void ImageViewer::setView0( ) {
  hideViews( );
  views[ 0 ]->show( );
}

void ImageViewer::setView1( ) {
  hideViews( );
  views[ 1 ]->show( );
}

void ImageViewer::setView2( ) {
  hideViews( );
  views[ 2 ]->show( );
}

void ImageViewer::setView3( ) {
  hideViews( );
  views[ 3 ]->show( );
}

void ImageViewer::set3Views( ) {
  showViews( );
  views[ 3 ]->hide( );
}

void ImageViewer::set4Views( ) {
  showViews( );
}

bool ImageViewer::eventFilter( QObject *obj, QEvent *evt ) {
  QGraphicsSceneMouseEvent *mouseEvt = dynamic_cast< QGraphicsSceneMouseEvent* >( evt );
  size_t scene = 0;
  for( size_t scn = 1; scn < views.size( ); ++scn ) {
    if( obj == views[ scn]->scene( ) ) {
      scene = scn;
    }
  }
  if( mouseEvt ) {
    std::cout <<
    QString( "Scene %3 = (%1, %2)" ).arg( mouseEvt->scenePos( ).x( ) ).arg( mouseEvt->scenePos( ).y( ) ).arg(scene).toStdString( )
              << std::endl;
  }
  return( QWidget::eventFilter( obj, evt ) );
}
