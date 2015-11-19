#include "gdcm.h"
#include "guiimage.h"
#include "tool.h"
#include <NiftiHeader.hpp>
#include <QDebug>
#include <QDebug>
#include <QPixmap>
#include <QRgb>
#include <QTime>
#include <QTime>


GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ), image( GDCM::OpenGImage(
                                                                                   fname.toStdString( ) ) ), m_fileName(
    fname ) {
  transform.resize( 4 );
  m_equalizeHistogram = false;
  bounding.insert( 0, 4, Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) ) );
  m_currentSlice.insert( 0, 4, 0 );
  m_max = Maximum( image );
  if( image.Dims( ) == 3 ) {
    COMMENT( "NIfTI image detected.", 2 );
    m_modality = Modality::BW3D;
    if( Bial::NiftiHeader::IsNiftiFile( fname.toStdString( ) ) ) {
      {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 0 ].Scale( 1, -1, -1 );
        updateBoundings( 0 );
      }
      {
        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 180.0, Bial::FastTransform::Z ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 1 ].Scale( -1, 1, 1 );
        updateBoundings( 1 );
      }
      {
        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
    }
    else {
      {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 0 );
      }
      {
        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 1 );
      }
      {
        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
    }
    cachedPixmaps.resize( 3 );
    needUpdate.insert( 0, 3, true );
    for( int view = 0; view < m_currentSlice.size( ); ++view ) {
      setCurrentSlice( view, depth( view ) / 2 );
    }
    m_currentToolPos = -1;
  }
  else if( ( image.Dims( ) == 2 ) && ( image.Channels( ) == 3 ) ) {
    COMMENT( "PPM image detected.", 2 );
    m_modality = Modality::RGB2D;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) );
    bounding[ 0 ] = box;
    cachedPixmaps.resize( 4 );
    needUpdate.insert( 0, 4, true );
  }
  else if( ( image.Dims( ) == 2 ) && ( image.Channels( ) == 1 ) ) {
    COMMENT( "Gray image detected.", 2 );
    m_modality = Modality::BW2D;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) );
    bounding[ 0 ] = box;
    cachedPixmaps.resize( 1 );
    needUpdate.push_back( true );
  }
  COMMENT( "Computing equalizaztion transform.", 2 );
  histogram = Bial::Signal::ZeroStartHistogram( image );
  Bial::Signal levi = histogram;
  levi[ 0 ] = 0;
  levi.Equalize( );
  equalization.resize( levi.size( ) );
  for( size_t val = 0; val < levi.size( ); ++val ) {
    equalization[ val ] = std::round( levi[ val ] );
  }
  COMMENT( "Computing equalized histogram.", 2 );
  equalized = Bial::Signal( histogram.size( ), 0.0, 1.0 );
  for( size_t val = 0; val < equalized.size( ); ++val ) {
    equalized[ equalization[ val ] ] = histogram[ val ];
  }
  COMMENT( "Image " << fileName( ).toStdString( ) << " size = (" << width( 0 ) << ", " << heigth( 0 ) << ", " <<
           depth( 0 ) << ")", 0 );
}

GuiImage::~GuiImage( ) {
  qDeleteAll( tools );
}

Tool* GuiImage::currentTool( ) {
  if( tools.isEmpty( ) ) {
    return( nullptr );
  }
  if( m_currentToolPos >= ( size_t ) tools.size( ) ) {
    m_currentToolPos = 0;
  }
  return( tools.at( m_currentToolPos ) );
}

Modality GuiImage::modality( ) {
  return( m_modality );
}

QString GuiImage::fileName( ) {
  return( m_fileName );
}

QPixmap GuiImage::getSlice( size_t view ) {
  size_t slice = currentSlice( view );
  COMMENT( "GET SLICE: image = " << m_fileName.toStdString( ) << ", axis = " << view << ", slice = " << slice, 2 );
  if( needUpdate[ view ] ) {
/*
 *    QTime timer;
 *    timer.start( );
 */
    if( slice >= depth( view ) ) {
      throw( std::out_of_range( BIAL_ERROR( QString( "Slice is out of range. Expected < %1" ).arg( depth( view ) ).
                                            toStdString( ) ) ) );
    }
    const size_t xsize = width( view );
    const size_t ysize = heigth( view );
    QImage res( xsize, ysize, QImage::Format_ARGB32 );
    double factor = 255.0 / ( double ) m_max;
    const Bial::FastTransform &transf = transform[ view ];
    if( ( modality( ) == Modality::BW3D ) || ( modality( ) == Modality::BW2D ) ) {
      COMMENT( "Generating BW view.", 2 );
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor)
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) {
          int pixel = 0;
          int xx, yy, zz;
          transf( x, y, slice, &xx, &yy, &zz );
          pixel = image( xx, yy, zz );
          if( m_equalizeHistogram ) {
            pixel = equalization[ pixel ];
          }
          pixel *= factor;
          scanLine[ x ] = qRgb( pixel, pixel, pixel );
        }
      }
    }
    else if( modality( ) == Modality::RGB2D ) {
      if( needUpdate[ 0 ] ) {
        COMMENT( "Generating RGB view.", 2 );
        size_t disp1 = image.size( 0 ) * image.size( 1 );
        size_t disp2 = disp1 * 2;
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor, disp1, disp2)
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            int xx, yy, zz;
            transf( x, y, slice, &xx, &yy, &zz );
            size_t pos = image.Position( xx, yy );
            int r = image[ pos ];
            int g = image[ pos + disp1 ];
            int b = image[ pos + disp2 ];
            if( m_equalizeHistogram ) {
              r = equalization[ r ];
              g = equalization[ g ];
              b = equalization[ b ];
            }
            scanLine[ x ] = qRgb( r * factor, g * factor, b * factor );
          }
        }
        cachedPixmaps[ 0 ] = QPixmap::fromImage( res );
        needUpdate[ 0 ] = false;
      }
      if( view > 0 ) {
        res = cachedPixmaps[ 0 ].toImage( );
        int r( view == 1 ), g( view == 2 ), b( view == 3 );
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            QRgb clr = scanLine[ x ];
            scanLine[ x ] = qRgb( qRed( clr ) * r, qGreen( clr ) * g, qBlue( clr ) * b );
          }
        }
      }
    }
    if( needUpdate[ view ] ) {
      cachedPixmaps[ view ] = QPixmap::fromImage( res );
      needUpdate[ view ] = false;
    }
/*    qDebug( ) << "Elapsed: " << timer.elapsed( ); */

  }
  return( cachedPixmaps[ view ] );
}

size_t GuiImage::width( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.x ) ) );
}

size_t GuiImage::heigth( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.y ) ) );
}

size_t GuiImage::depth( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.z ) ) );
}

void GuiImage::setCurrentSlice( size_t view, size_t slice ) {
  size_t sz = m_currentSlice.size( );
  if( view < sz ) {
    if( ( m_currentSlice[ view ] != slice ) && ( slice < depth( view ) ) ) {
      m_currentSlice[ view ] = slice;
      needUpdate[ view ] = true;
      emit imageUpdated( );
    }
  }
  else {
    throw std::out_of_range( BIAL_ERROR( "Axis out of range." ) );
  }
}

Bial::Point3D GuiImage::getPosition( QPointF pos, size_t view ) {
  Bial::Point3D point( pos.x( ), pos.y( ), ( double ) m_currentSlice[ view ] );
  transform[ view ]( point, &point );
  return( point );
}

Bial::FastTransform GuiImage::getTransform( size_t axis ) {
  return( transform.at( axis ) );
}

const Bial::Image< int > &GuiImage::getImage( ) const {
  return( image );
}

void GuiImage::rotate90( size_t view ) {
  Bial::FastTransform transf;
  transf.Rotate( -90.0, Bial::FastTransform::Z );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::flipH( size_t view ) {
  Bial::FastTransform transf;
  transf.Scale( -1, 1, 1 );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::flipV( size_t view ) {
  Bial::FastTransform transf;
  transf.Scale( 1, -1, 1 );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::rotateAll90( ) {
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    rotate90( axis );
  }
}

int GuiImage::max( ) {
  return( m_max );
}

size_t GuiImage::currentSlice( size_t view ) {
  return( m_currentSlice[ view ] );
}

bool GuiImage::getEqualizeHistogram( ) const {
  return( m_equalizeHistogram );
}

void GuiImage::setEqualizeHistogram( bool equalizeHistogram ) {
  m_equalizeHistogram = equalizeHistogram;
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit imageUpdated( );
}


const Bial::Signal &GuiImage::getHistogram( ) const {
  if( m_equalizeHistogram ) {
    return( equalized );
  }
  return( histogram );
}

int GuiImage::getPixel( int x, int y, int z ) {
  int color = 0;
  if( modality( ) == Modality::BW2D ) {
    if( image.ValidCoordinate( (size_t)x, (size_t)y ) ) {
      color = image.at( x, y );
    }
  }
  else {
    if( image.ValidCoordinate( (size_t)x, (size_t)y, (size_t)z ) ) {
      color = image.at( x, y, z );
    }
  }
  if( m_equalizeHistogram ) {
    return( equalization[ color ] );
  }
  return( color );
}

size_t GuiImage::currentToolPos( ) const {
  return( m_currentToolPos );
}

void GuiImage::setCurrentToolPos( const size_t &currentToolPos ) {
  if( currentToolPos < tools.size( ) ) {
    m_currentToolPos = currentToolPos;
  }
}

void GuiImage::updateBoundings( size_t axis ) {
  Bial::Point3D start;
  Bial::Point3D end( image.size( 0 ) - 1, image.size( 1 ) - 1, 1 );
  if( image.Dims( ) > 2 ) {
    end = Bial::Point3D( image.size( 0 ) - 1, image.size( 1 ) - 1, image.size( 2 ) - 1 );
  }
  transform[ axis ]( start, &start );
  transform[ axis ]( end, &end );
  bounding[ axis ] = Bial::BBox( start, end );
  transform[ axis ] = transform[ axis ].Inverse( );
  transform[ axis ].Translate( bounding[ axis ].pMin.x, bounding[ axis ].pMin.y, bounding[ axis ].pMin.z );
  bounding[ axis ] = bounding[ axis ].Normalized( );
}
