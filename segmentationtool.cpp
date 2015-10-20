#include "Draw.hpp"
#include "File.hpp"
#include "Geometrics.hpp"
#include "Segmentation.hpp"
#include "guiimage.h"
#include "segmentationtool.h"
#include <Gradient.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>


double SegmentationTool::getAlpha( ) const {
  return( alpha );
}

void SegmentationTool::setAlpha( double value ) {
  alpha = value;
}

double SegmentationTool::getBeta( ) const {
  return( beta );
}

void SegmentationTool::setBeta( double value ) {
  beta = value;
}

int SegmentationTool::getDrawType( ) const {
  return( drawType );
}

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage,
                                                                                      viewer ), seeds(
    guiImage->getImage( ).Dim( ) ) {
  drawType = 1;
  drawing = false;
  setObjectName( "SegmentationTool" );
  setHasLabel( true );

}

int SegmentationTool::type( ) {
  return( SegmentationTool::Type );
}

void SegmentationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  drawing = true;
  if( ( drawType == 1 ) || ( drawType == 2 ) ) {
    switch( buttons ) {
        case Qt::LeftButton:
        drawType = 1;
        break;
        case Qt::RightButton:
        drawType = 2;
        break;

    }
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  lastPoint = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
}


void SegmentationTool::mouseMoved( QPointF pt, size_t axis ) {
  if( drawing ) {
    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    Bial::Point3D actual = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    if( lastPoint == actual ) {
      return;
    }
    drawSeed( lastPoint, actual );
    lastPoint = actual;
    emit guiImage->imageUpdated( );
  }
}

void SegmentationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  drawing = false;
  if( drawing ) {
    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    Bial::Point3D actual = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    drawSeed( lastPoint, actual );
    emit guiImage->imageUpdated( );
  }
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
}

void SegmentationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
  /* nothing happens */
}

void SegmentationTool::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice );
  Q_UNUSED( axis );
  /* nothing happens */
}

void SegmentationTool::drawSeed( Bial::Point3D last, Bial::Point3D actual ) {
  Bial::Vector< float > vLast;
  Bial::Vector< float > vActual;
  if( guiImage->modality( ) == Modality::BW3D ) {
    vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ], ( float ) last[ 2 ] } };
    vActual = { { ( float ) actual[ 0 ], ( float ) actual[ 1 ], ( float ) actual[ 2 ] } };
  }
  else {
    vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ] } };
    vActual = { { ( float ) actual[ 0 ], ( float ) actual[ 1 ] } };
  }
  Bial::Line imgLine( vLast, vActual );
  imgLine.Draw( seeds, drawType );
}


void SegmentationTool::setDrawType( int type ) {
  drawType = type;
}

void SegmentationTool::clearSeeds( ) {
  for( size_t i = 0; i < seeds.Size( ); ++i ) {
    seeds[ i ] = 0;
  }
}

Bial::Image< char > SegmentationTool::segmentationOGS( double alpha, double beta ) {
  Bial::Vector< size_t > img;
  Bial::Vector< size_t > bkg;
  for( size_t i = 0; i < seeds.size( ); ++i ) {
    if( seeds[ i ] == 1 ) {
      img.push_back( i );
    }
    else if( seeds[ i ] == 2 ) {
      bkg.push_back( i );
    }
  }
  if( ( !img.empty( ) ) || ( !bkg.empty( ) ) ) {
    Bial::Image< char > res = Bial::Segmentation::OrientedGeodesicStar( guiImage->getImage( ), img, bkg, alpha, beta );
    mask = Bial::Gradient::Morphological( res );
    emit guiImage->imageUpdated( );
    return( res );
  }
  else {
    throw std::runtime_error( "Seeds Missing" );
  }
}


QPixmap SegmentationTool::getLabel( size_t axis ) {
  size_t xsz = guiImage->width( axis );
  size_t ysz = guiImage->heigth( axis );

  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  if( guiImage->modality( ) == Modality::BW3D ) {
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        QRgb color = qRgba( 0, 0, 0, 0 );
        if( ( mask.size( ) == seeds.size( ) ) && mask( pos.x, pos.y, pos.z ) ) {
          color = qRgb( 255, 0, 0 );
        }
        else {
          char pixel = seeds( pos.x, pos.y, pos.z );
          if( pixel == 1 ) {
            color = qRgb( 255, 0, 0 );
          }
          else if( pixel == 2 ) {
            color = qRgb( 0, 0, 255 );
          }
        }
        scanLine[ x ] = color;
      }
    }
  }
  else {
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        QRgb color = qRgba( 0, 0, 0, 0 );
        if( ( mask.size( ) == seeds.size( ) ) && mask( pos.x, pos.y ) ) {
          color = qRgb( 255, 0, 0 );
        }
        else {
          char pixel = seeds( pos.x, pos.y );
          if( pixel == 1 ) {
            color = qRgb( 0, 255, 0 );
          }
          else if( pixel == 2 ) {
            color = qRgb( 0, 0, 255 );
          }
        }
        scanLine[ x ] = color;
      }
    }
  }
  return( QPixmap::fromImage( res ) );
}
