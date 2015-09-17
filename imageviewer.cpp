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
  layout->setVerticalSpacing( 0 );
  layout->setHorizontalSpacing( 0 );
  layout->setMargin( 0 );
  setGridLayout( );
/*  setStyleSheet("background-color:black;"); */
  QPalette p( palette( ) );
  p.setColor( QPalette::Background, Qt::black );
  setAutoFillBackground( true );
  setPalette( p );

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
  connect( controller, &Controller::imageUpdated, this, &ImageViewer::updateImage );
  for( ImageWidget *view : views ) {
    connect( view, &ImageWidget::sliceChanged, controller, &Controller::setCurrentSlice );
  }
  for( size_t axis = 0; axis < 4; ++axis ) {
    getScene( axis )->addItem( controller->getPixmapItem( axis ) );
  }
}

void ImageViewer::updateViews( ) {
  for( size_t i = 0; i < 4; ++i ) {
    views[ i ]->fitInView( );
  }
}

void ImageViewer::updateImage( ) {
  COMMENT( "ImageViewer::updateImage", 2 );
  GuiImage *img = controller->currentImage( );
  if( !img ) {
    return;
  }
  for( size_t i = 0; i < 4; ++i ) {
    views[ i ]->setSlice( img->currentSlice( i ) );
  }
  updateViews( );
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
    getScene( axis )->setSceneRect( controller->getPixmapItem( axis )->boundingRect( ) );
  }
  setLayoutType( format.currentLayout );
  setViewMode( format.currentViews );
  /* TODO Continue ... */
}

void ImageViewer::setLayoutType( Layout layout ) {
  switch( layout ) {
      case Layout::GRID:
      setGridLayout( );
      break;
      case Layout::HORIZONTAL:
      setHorizontalLayout( );
      break;
      case Layout::VERTICAL:
      setVerticalLayout( );
      break;
      case Layout::NONE:
      break;
  }
}

void ImageViewer::getNewLayout( ) {
/*
 *  delete layout;
 *  layout = new QGridLayout(this);
 *  layout->setVerticalSpacing( 0 );
 *  layout->setHorizontalSpacing( 0 );
 *  layout->setMargin(0);
 *  setLayout(layout);
 */
  for( ImageWidget *view : views ) {
    layout->removeWidget( view );
  }
}

void ImageViewer::setGridLayout( ) {
  getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 1, 0 );
  layout->addWidget( views[ 3 ], 1, 1 );
/*
 *  layout->setRowStretch(0,1);
 *  layout->setRowStretch(1,0);
 *  layout->setColumnStretch(0, 0);
 *  layout->setColumnStretch(1, 0);
 */

  updateViews( );
}

void ImageViewer::setHorizontalLayout( ) {
  getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 0, 1 );
  layout->addWidget( views[ 2 ], 0, 2 );
  layout->addWidget( views[ 3 ], 0, 3 );
/*
 *  layout->setRowStretch(0,1);
 *  layout->setColumnStretch(0, 1);
 *  layout->setColumnStretch(1, 1);
 *  layout->setColumnStretch(2, 1);
 *  layout->setColumnStretch(3, 1);
 */

  updateViews( );
}

void ImageViewer::setVerticalLayout( ) {
  getNewLayout( );
  layout->addWidget( views[ 0 ], 0, 0 );
  layout->addWidget( views[ 1 ], 1, 0 );
  layout->addWidget( views[ 2 ], 2, 0 );
  layout->addWidget( views[ 3 ], 3, 0 );
/*
 *  layout->setColumnStretch(0, 1);
 *  layout->setRowStretch(0, 1);
 *  layout->setRowStretch(1, 1);
 *  layout->setRowStretch(2, 1);
 *  layout->setRowStretch(3, 1);
 */

  updateViews( );
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

void ImageViewer::setViewMode( Views views ) {
  switch( views ) {
      case Views::NONE:
      hideViews( );
      break;
      case Views::SHOW0:
      setView0( );
      break;
      case Views::SHOW1:
      setView1( );
      break;
      case Views::SHOW2:
      setView2( );
      break;
      case Views::SHOW3:
      setView3( );
      break;
      case Views::SHOW012:
      setViews012( );
      break;
      case Views::SHOW123:
      setViews123( );
      break;
      case Views::SHOW0123:
      setViews0123( );
      break;
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

void ImageViewer::setViews0123( ) {
  showViews( );
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
    GuiImage *img = controller->currentImage( );
    if( img != nullptr ) {
      Bial::Point3D pt = img->getPosition( mouseEvt->scenePos( ), axis );
      int max = img->max( );
      if( img->modality( ) == Modality::NIfTI ) {
        if( img->getImage( ).ValidPixel( pt.x, pt.y, pt.z ) ) {
          int color = img->getImage( ).at( pt.x, pt.y, pt.z );
          emit updateStatus( QString( "Axis %1 : (%2, %3, %4) = %5/%6" ).arg( axis ).arg( ( int ) pt.x ).arg(
                               ( int ) pt.y ).arg( ( int ) pt.z ).arg( color ).arg( max ), 10000 );
        }
      }
      else if( img->modality( ) == Modality::BW ) {
        if( img->getImage( ).ValidPixel( pt.x, pt.y ) ) {
          int color = img->getImage( ).at( pt.x, pt.y );
          emit updateStatus( QString( "(%1, %2) = %3/%4" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( color ).arg(
                               max ), 10000 );
        }
      }
      else if( img->modality( ) == Modality::RGB ) {
        if( img->getImage( ).ValidPixel( pt.x, pt.y ) ) {
          int r = img->getImage( ).at( pt.x, pt.y, 0 );
          int g = img->getImage( ).at( pt.x, pt.y, 1 );
          int b = img->getImage( ).at( pt.x, pt.y, 2 );
          emit updateStatus( QString( "(%1, %2) = (%3, %4, %5)/%6" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg(
                               r ).arg( g ).arg( b ).arg( max ), 10000 );
        }
      }
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
