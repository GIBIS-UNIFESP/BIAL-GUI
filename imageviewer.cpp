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
    getScene( i )->installEventFilter( this );
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
    connect( view, &ImageWidget::sliceChanged, this, &ImageViewer::sliceChanged );
    connect( view, &ImageWidget::rotate, controller, &Controller::rotate90 );
    connect( view, &ImageWidget::fliph, controller, &Controller::flipH );
    connect( view, &ImageWidget::flipv, controller, &Controller::flipV );
  }
  for( size_t axis = 0; axis < 4; ++axis ) {
    getScene( axis )->addItem( controller->getPixmapItem( axis ) );
  }
}

void ImageViewer::updateViews( ) {
  COMMENT( "ImageViewer::updateViews", 2 );
  if( !controller ) {
    return;
  }
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  for( size_t axis = 0; axis < 4; ++axis ) {
    views[ axis ]->setSlice( img->currentSlice( axis ) );
    getScene( axis )->setOverlay( controller->currentFormat( )->overlay( ) );
  }
}

void ImageViewer::changeImage( ) {
  COMMENT( "ImageViewer::changeImage", 2 );
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  DisplayFormat *format = controller->currentFormat( );
  for( size_t axis = 0; axis < 4; ++axis ) {

    getScene( axis )->setOverlay( false );
    getScene( axis )->setOverlayPen( format->overlayColor( ) );
    if( format->hasViewerControls( ) ) {
      views[ axis ]->setRange( 0, img->depth( axis ) - 1 );
      views[ axis ]->setSlice( img->currentSlice( axis ) );
      views[ axis ]->showControls( );
    }
    else {
      views[ axis ]->hideControls( );
    }
  }
  if( format->modality( ) == Modality::RGB2D ) {
    getScene(1)->setOverlayPen( QPen( Qt::cyan ) );
    getScene(2)->setOverlayPen( QPen( Qt::magenta ) );
    getScene(3)->setOverlayPen( QPen( Qt::yellow ) );
  }
  setLayoutType( format->currentLayout( ) );
  setViewMode( format->currentViews( ) );
  for( size_t axis = 0; axis < 4; ++axis ) {
    if( controller ) {
      QRectF r = controller->getPixmapItem( axis )->boundingRect( );
      getScene( axis )->setSceneRect( r );
      QGraphicsView *view = views[ axis ]->graphicsView( );
      view->fitInView( controller->getPixmapItem( axis ), Qt::KeepAspectRatio );
    }
  }
  updateViews( );
}

void ImageViewer::updateOverlay( QPointF pt, size_t axis ) {
  COMMENT( "ImageViewer::updateOverlay", 2 );
  GuiImage *img = controller->currentImage( );
  pt.setX( qMin( qMax( pt.x( ), 0.0 ), ( double ) img->width( axis ) ) );
  pt.setY( qMin( qMax( pt.y( ), 0.0 ), ( double ) img->heigth( axis ) ) );
  getScene( axis )->setOverlayPos( pt );
  Bial::FastTransform transform = img->getTransform( axis );
  Bial::Point3D pt3d = transform( ( double ) pt.x( ),
                                  ( double ) pt.y( ),
                                  ( double ) img->currentSlice( axis ) );
  size_t size = controller->currentFormat( )->getMaximumNumberOfViews( );
  for( size_t other = 0; other < size; ++other ) {
    if( controller->currentFormat( )->overlay( ) ) {
      views[ other ]->scene( )->setOverlay( true );
      if( other != axis ) {
        Bial::FastTransform otherTransf = img->getTransform( other ).Inverse( );
        Bial::Point3D otherPt = otherTransf( pt3d );
        views[ other ]->scene( )->setOverlayPos( QPointF( otherPt.x, otherPt.y ) );
      }
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

void ImageViewer::setViewMode( Views view ) {
  hideViews( );
  views[ 0 ]->setVisible( ( int ) view & ( int ) Views::SHOW0 );
  views[ 1 ]->setVisible( ( int ) view & ( int ) Views::SHOW1 );
  views[ 2 ]->setVisible( ( int ) view & ( int ) Views::SHOW2 );
  views[ 3 ]->setVisible( ( int ) view & ( int ) Views::SHOW3 );
  updateViews( );
}

void ImageViewer::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice )
  updateOverlay( getScene( axis )->overlayPos( ), axis );
}

void ImageViewer::toggleOverlay( ) {
  if( controller->currentImage( ) ) {
    controller->currentFormat( )->toggleOverlay( );
  }
}

void ImageViewer::resizeEvent( QResizeEvent* ) {
  changeImage( );
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
        emit mouseDragged( scnPos, mouseEvt->buttons( ), axis );
      }
      emit mouseMoved( scnPos, axis );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMousePress ) {
      if( mouseEvt->button( ) == Qt::LeftButton ) {
        dragging = true;
        timer.restart( );
        controller->changeOthersSlices( scnPos, axis );
        updateOverlay( scnPos, axis );
      }
      emit mouseClicked( scnPos, mouseEvt->buttons( ), axis );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMouseRelease ) {
      if( mouseEvt->button( ) == Qt::LeftButton ) {
        dragging = false;
        controller->changeOthersSlices( scnPos, axis );
        updateOverlay( scnPos, axis );
      }
      emit mouseReleased( scnPos, mouseEvt->buttons( ), axis );
    }
  }
  return( QWidget::eventFilter( obj, evt ) );
}

GraphicsScene* ImageViewer::getScene( size_t axis ) {
  if( axis > views.size( ) ) {
    throw std::out_of_range( BIAL_ERROR( QString( "Invalid axis, expected < %1." ).arg( views.size( ) ).toStdString( ) ) );
  }
  return( views[ axis ]->scene( ) );
}
