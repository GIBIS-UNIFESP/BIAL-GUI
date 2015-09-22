#include <QDebug>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <iostream>

#include "Common.hpp"
#include "controller.h"
#include "graphicsscene.h"
#include "imageviewer.h"
#include "imagewidget.h"

ImageViewer::ImageViewer( QWidget *parent ) : QWidget( parent ) {
  controller = nullptr;
  for( size_t i = 0; i < views.size( ); ++i ) {
    views[ i ] = new ImageWidget( this );
    views[ i ]->hideControls( );
    views[ i ]->scene( )->installEventFilter( this );
    views[ i ]->setViewNumber( i );
  }
  layout = new QGridLayout( this );
  layout->setVerticalSpacing( 0 );
  layout->setHorizontalSpacing( 0 );
  layout->setMargin( 0 );
  setGridLayout( );
  QPalette p( palette( ) );
  p.setColor( QPalette::Background, Qt::black );
  setAutoFillBackground( true );
  setPalette( p );
  dragging = false;
}

ImageViewer::~ImageViewer( ) {

}

void ImageViewer::setViewBgColor( const QColor &color ) {
  for( ImageWidget *view : views ) {
    view->setViewBgColor( color );
  }
}

void ImageViewer::setController( Controller *value ) {
  controller = value;
  connect( controller, &Controller::currentImageChanged, this, &ImageViewer::changeImage );
  connect( controller, &Controller::imageUpdated, this, &ImageViewer::updateViews );
  /*  connect( this, &ImageViewer::mouseClicked, controller, &Controller::changeOthersSlices ); */
  for( ImageWidget *view : views ) {
    connect( view, &ImageWidget::sliceChanged, controller, &Controller::setCurrentSlice );
  }
  for( size_t axis = 0; axis < 4; ++axis ) {
    getScene( axis )->addItem( controller->getPixmapItem( axis ) );
  }
}

void ImageViewer::updateViews( ) {
  COMMENT( "ImageViewer::updateViews", 0 );
  if( !controller ) {
    return;
  }
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  for( size_t i = 0; i < 4; ++i ) {
    views[ i ]->setSlice( img->currentSlice( i ) );
    if( controller ) {
      QGraphicsView *view = views[ i ]->graphicsView( );
      view->fitInView( controller->getPixmapItem( i ), Qt::KeepAspectRatio );
      QRectF r = controller->getPixmapItem( i )->boundingRect( );
      views[ i ]->scene( )->setSceneRect( r );
    }
  }
}

void ImageViewer::changeImage( ) {
  COMMENT( "ImageViewer::changeImage", 0 );
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  DisplayFormat format = controller->currentFormat( );
  for( size_t axis = 0; axis < 4; ++axis ) {
    if( format.viewerControls ) {
      views[ axis ]->setRange( 0, img->depth( axis ) - 1 );
      views[ axis ]->setSlice( img->currentSlice( axis ) );
      views[ axis ]->showControls( );
    }
    else {
      views[ axis ]->hideControls( );
    }
  }
  setLayoutType( format.currentLayout );
  setViewMode( format.currentViews );
  updateViews( );
}

void ImageViewer::updateOverlay( QPointF pt, size_t axis ) {
  /* FIXME Use "currentSlice" to determine overlay positions. */
  COMMENT( "ImageViewer::updateOverlay", 0 );
  views[ axis ]->scene( )->updateOverlay( pt );
  GuiImage *img = controller->currentImage( );
  Bial::Transform3D transform = img->getTransform( axis );
  Bial::Point3D pt3d = transform( ( double ) pt.x( ), ( double ) pt.y( ),
                                  ( double ) img->currentSlice( axis ) );
  for( size_t other = 0; other < 3; ++other ) {
    if( other != axis ) {
      Bial::Transform3D otherTransf = img->getTransform( other ).Inverse( );
      Bial::Point3D otherPt = otherTransf( pt3d );
      views[ other ]->scene( )->updateOverlay( QPointF( otherPt.x, otherPt.y ) );
    }
  }
}

void ImageViewer::setLayoutType( Layout layout ) {
  switch( layout ) {
  case Layout::GRID: {
      setGridLayout( );
      break;
    }
  case Layout::HORIZONTAL: {
      setHorizontalLayout( );
      break;
    }
  case Layout::VERTICAL: {
      setVerticalLayout( );
      break;
    }
  }
}

void ImageViewer::getNewLayout( ) {
  for( ImageWidget *view : views ) {
    layout->removeWidget( view );
  }
  updateViews( );
}

void ImageViewer::setGridLayout( ) {
  COMMENT( "Set grid layout.", 0 )
      getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 1, 0 );
  layout->addWidget( views[ 3 ], 1, 1 );

  updateViews( );
}

void ImageViewer::setHorizontalLayout( ) {
  COMMENT( "Set horizontal layout.", 0 )
      getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 0, 2 );
  layout->addWidget( views[ 3 ], 0, 3 );

  updateViews( );
}


void ImageViewer::setVerticalLayout( ) {
  COMMENT( "Set vertical layout.", 0 )
      getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 1, 0 );
  layout->addWidget( views[ 2 ], 2, 0 );
  layout->addWidget( views[ 3 ], 3, 0 );

  updateViews( );
}

void ImageViewer::hideViews( ) {
  for( ImageWidget *view : views ) {
    view->hide( );
  }
  updateViews( );
}

void ImageViewer::showViews( ) {
  for( ImageWidget *view : views ) {
    view->show( );
  }
  updateViews( );
}

void ImageViewer::setViewMode( Views views ) {
  switch( views ) {
  case Views::SHOW0: {
      setView0( );
      break;
    }
  case Views::SHOW1: {
      setView1( );
      break;
    }
  case Views::SHOW2: {
      setView2( );
      break;
    }
  case Views::SHOW3: {
      setView3( );
      break;
    }
  case Views::SHOW012: {
      setViews012( );
      break;
    }
  case Views::SHOW123: {
      setViews123( );
      break;
    }
  case Views::SHOW0123: {
      setViews0123( );
      break;
    }
  }
}

void ImageViewer::setView0( ) {
  hideViews( );
  views[ 0 ]->show( );
  updateViews( );
}

void ImageViewer::setView1( ) {
  hideViews( );
  views[ 1 ]->show( );
  updateViews( );
}

void ImageViewer::setView2( ) {
  hideViews( );
  views[ 2 ]->show( );
  updateViews( );
}

void ImageViewer::setView3( ) {
  hideViews( );
  views[ 3 ]->show( );
  updateViews( );
}

void ImageViewer::setViews012( ) {
  showViews( );
  views[ 3 ]->hide( );
  updateViews( );
}

void ImageViewer::setViews123( ) {
  showViews( );
  views[ 0 ]->hide( );
  updateViews( );
}

void ImageViewer::setViews0123( ) {
  showViews( );
  updateViews( );
}

void ImageViewer::toggleOverlay( ) {
  for( size_t axis = 0; axis < 4; ++axis ) {
    if( controller->currentImage( )->modality( ) == Modality::NIfTI ) {
      views[ axis ]->scene( )->setOverlay( !views[ axis ]->scene( )->overlay( ) );
    }
  }
}

void ImageViewer::resizeEvent( QResizeEvent* ) {
  updateViews( );
}

bool ImageViewer::eventFilter( QObject *obj, QEvent *evt ) {
  QGraphicsSceneMouseEvent *mouseEvt = dynamic_cast< QGraphicsSceneMouseEvent* >( evt );
  size_t axis = 0;
  for( size_t scn = 1; scn < views.size( ); ++scn ) {
    if( obj == views[ scn ]->scene( ) ) {
      axis = scn;
    }
  }
  if( mouseEvt ) {
    QPointF scnPos = mouseEvt->scenePos( );
    if( mouseEvt->type( ) == QEvent::GraphicsSceneMouseMove ) {
      if( dragging && ( timer.elapsed( ) > 100 ) ) {
        timer.restart( );
        controller->changeOthersSlices( scnPos, axis );
        updateOverlay( scnPos, axis );
      }
      emit mouseMoved( scnPos, axis );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMousePress ) {
      dragging = true;
      timer.restart( );
      /*emit mouseClicked( scnPos, mouseEvt->buttons( ), axis ); */
      controller->changeOthersSlices( scnPos, axis );
      updateOverlay( scnPos, axis );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMouseRelease ) {
      dragging = false;
    }
  }
  return( QWidget::eventFilter( obj, evt ) );
}

QGraphicsScene* ImageViewer::getScene( size_t axis ) {
  if( axis > views.size( ) ) {
    throw std::out_of_range( BIAL_ERROR( QString( "Invalid axis, expected < %1." ).arg( views.size( ) ).toStdString( ) ) );
  }
  return( views[ axis ]->scene( ) );
}
