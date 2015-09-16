#include "controller.h"
#include "imageviewer.h"
#include "imagewidget.h"

#include <Common.hpp>
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
    views[ i ]->setViewNumber( i );
  }
  layout = new QGridLayout( this );
  layout->setVerticalSpacing( 10 );
  layout->setHorizontalSpacing( 10 );
  setGridLayout( );

}

ImageViewer::~ImageViewer( ) {

}

void ImageViewer::setBackgroundColor( const QColor &color ) {
  for( ImageWidget *view : views ) {
    view->setBackgroundColor( color );
  }
}

void ImageViewer::setController( Controller *value ) {
  controller = value;
  connect( controller, &Controller::imageChanged, this, &ImageViewer::changeImage );
  connect( controller, &Controller::imageUpdated, this, &ImageViewer::updateImage );

  for( ImageWidget *view : views ) {
    connect(view, &ImageWidget::sliceChanged, controller, &Controller::setCurrentSlice);
  }
}

void ImageViewer::updateImage( ) {
  COMMENT("ImageViewer::updateImage", 2);
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  for( size_t i = 0; i < 4; ++i ) {
    views[ i ]->setSlice(img->currentSlice(i));
  }
  /* TODO continue ... */
}

void ImageViewer::changeImage() {
  COMMENT("ImageViewer::changeImage", 0);
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  for( size_t i = 0; i < 4; ++i ) {
    views[ i ]->setRange( 0, img->depth( i ) - 1 );
    views[ i ]->showControls( );
    views[ i ]->setSlice(img->currentSlice(i));
  }
  /* TODO continue ... */

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

void ImageViewer::setViews012( ) {
  showViews( );
  views[ 3 ]->hide( );
}

void ImageViewer::setViews123( ) {
  showViews( );
  views[ 0 ]->hide( );
}

void ImageViewer::set0123Views( ) {
  showViews( );
}

bool ImageViewer::eventFilter( QObject *obj, QEvent *evt ) {
  QGraphicsSceneMouseEvent *mouseEvt = dynamic_cast< QGraphicsSceneMouseEvent* >( evt );
  size_t scene = 0;
  for( size_t scn = 1; scn < views.size( ); ++scn ) {
    if( obj == views[ scn ]->scene( ) ) {
      scene = scn;
    }
  }
  if( mouseEvt ) {
    emit updateStatus( QString( "Scene %3 position: (%1, %2)" ).arg( mouseEvt->scenePos( ).x( ) ).arg(
                         mouseEvt->scenePos( ).y( ) ).arg( scene ), 100 );
  }
  return( QWidget::eventFilter( obj, evt ) );
}

QGraphicsScene* ImageViewer::getScene( size_t axis ) {
  if( axis > views.size( ) ) {
    throw std::out_of_range( BIAL_ERROR( QString( "Invalid axis, expected < %1." ).arg( views.size( ) ).toStdString( ) ) );
  }
  return( views[ axis ]->scene( ) );
}
