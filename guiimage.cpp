#include "gdcm.h"
#include "guiimage.h"
#include <QPixmap>
#include <QRgb>

GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ), image( GDCM::OpenGImage(
                                                                                   fname.toStdString( ) ) ), m_fileName(
    fname ) {
  transform.resize( 4 );
  m_equalizeHistogram = false;
  bounding.insert( 0, 4, Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) ) );
  m_currentSlice.insert( 0, 4, 0 );
  m_max = image.Maximum( );
  if( image.Dims( ) == 3 ) {
    COMMENT( "NIfTI image detected.", 2 );
    m_modality = Modality::BW3D;
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
    cachedPixmaps.resize( 3 );
    needUpdate.insert( 0, 3, true );
    for( int axis = 0; axis < m_currentSlice.size( ); ++axis ) {
      setCurrentSlice( axis, depth( axis ) / 2 );
    }
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
  equalization = histogram;
  equalization[ 0 ] = 0;
  equalization.Equalize( );
  for( size_t val = 0; val < equalization.size( ); ++val ) {
    equalization[ val ] = std::round( equalization[ val ] );
  }
  COMMENT( "Computing equalized histogram.", 2 );
  equalized = histogram;
  for( size_t val = 0; val < equalized.size( ); ++val ) {
    equalized[ equalization[ val ] ] = histogram[ val ];
  }
  COMMENT( "Image " << fileName( ).toStdString( ) << " size = (" << width( 0 ) << ", " << heigth( 0 ) << ", " <<
           depth( 0 ) << ")", 0 );
}

Modality GuiImage::modality( ) {
  return( m_modality );
}

QString GuiImage::fileName( ) {
  return( m_fileName );
}

QPixmap GuiImage::getSlice( size_t axis ) {
  size_t slice = currentSlice( axis );
  COMMENT( "GET SLICE: image = " << m_fileName.toStdString( ) << ", axis = " << axis << ", slice = " << slice, 0 );
  if( needUpdate[ axis ] ) {
    if( slice >= depth( axis ) ) {
      throw( std::out_of_range( BIAL_ERROR( QString( "Slice is out of range. Expected < %1" ).arg( depth( axis ) ).
                                            toStdString( ) ) ) );
    }
    const size_t xsize = width( axis );
    const size_t ysize = heigth( axis );
    QImage res( xsize, ysize, QImage::Format_ARGB32 );
    double factor = 255.0 / ( double ) m_max;
    const Bial::FastTransform &transf = transform[ axis ];
    if( modality( ) == Modality::BW3D ) {
      COMMENT("Generating Nifti view.", 2);
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) { /*  */
          Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
          int pixel = 0;
          if( image.ValidPixel( pos.x, pos.y, pos.z ) ) {
            pixel = image( pos.x, pos.y, pos.z );
          }
          if( m_equalizeHistogram ) {
            pixel = static_cast< int >( equalization[ pixel ] );
          }
          pixel *= factor;
          scanLine[ x ] = qRgb( pixel, pixel, pixel );
        }
      }
    }
    else if( modality( ) == Modality::BW2D ) {
      COMMENT("Generating Grayscale view.", 2);
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) { /*  */
          Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
          int pixel = 0;
          if( image.ValidPixel( pos.x, pos.y ) ) {
            pixel = image( pos.x, pos.y );
          }
          if( m_equalizeHistogram ) {
            pixel = static_cast< int >( equalization[ equalization.Bin( pixel ) ] );
          }
          pixel *= factor;
          scanLine[ x ] = qRgb( pixel, pixel, pixel );
        }
      }
    }
    else if( modality( ) == Modality::RGB2D ) {
      if( needUpdate[ 0 ] ) {
        COMMENT("Generating RGB view.", 2);
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) { /*  */
            Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
            int r( 0 ), g( 0 ), b( 0 );
            if( image.ValidPixel( pos.x, pos.y ) ) {
              r = image( pos.x, pos.y, 0 );
              g = image( pos.x, pos.y, 1 );
              b = image( pos.x, pos.y, 2 );
            }
            if( m_equalizeHistogram ) {
              r = static_cast< int >( equalization[ equalization.Bin( r ) ] );
              g = static_cast< int >( equalization[ equalization.Bin( g ) ] );
              b = static_cast< int >( equalization[ equalization.Bin( b ) ] );
            }
            r *= factor;
            g *= factor;
            b *= factor;
            scanLine[ x ] = qRgb( r, g, b );
          }
        }
        cachedPixmaps[ 0 ] = QPixmap::fromImage( res );
        needUpdate[ 0 ] = false;
      }
      if( axis > 0 ) {
        res = cachedPixmaps[ 0 ].toImage( );
        int r( axis == 1 ), g( axis == 2 ), b( axis == 3 );
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            QRgb clr = scanLine[ x ];
            scanLine[ x ] = qRgb( qRed(clr) * r, qGreen(clr) * g , qBlue(clr) * b);
          }
        }
      }
    }
    if( needUpdate[ axis ] ) {
      cachedPixmaps[ axis ] = QPixmap::fromImage( res );
      needUpdate[ axis ] = false;
    }
  }
  return( cachedPixmaps[ axis ] );
}

size_t GuiImage::width( size_t axis = 0 ) {
  return( abs( round( bounding.at( axis ).pMax.x ) ) );
}

size_t GuiImage::heigth( size_t axis = 0 ) {
  return( abs( round( bounding.at( axis ).pMax.y ) ) );
}

size_t GuiImage::depth( size_t axis = 0 ) {
  return( abs( round( bounding.at( axis ).pMax.z ) ) );
}

bool GuiImage::hasLabels( ) {
  return( false );
}

void GuiImage::setCurrentSlice( size_t axis, size_t slice ) {
  size_t sz = m_currentSlice.size( );
  if( axis < sz ) {
    if( ( m_currentSlice[ axis ] != slice ) && ( slice < depth( axis ) ) ) {
      m_currentSlice[ axis ] = slice;
      needUpdate[ axis ] = true;
      emit imageUpdated( );
    }
  }
  else {
    throw std::out_of_range( BIAL_ERROR( "Axis out of range." ) );
  }
}

Bial::Point3D GuiImage::getPosition( QPointF pos, size_t axis ) {
  Bial::Point3D point( pos.x( ), pos.y( ), ( double ) m_currentSlice[ axis ] );
  transform[ axis ]( point, &point );
  return( point );
}

Bial::FastTransform GuiImage::getTransform( size_t axis ) {
  return( transform.at( axis ) );
}

const Bial::Image< int > &GuiImage::getImage( ) const {
  return( image );
}

void GuiImage::rotate90( size_t axis ) {
  Bial::FastTransform transf;
  transf.Rotate( -90.0, Bial::FastTransform::Z );
  transform[ axis ] = transf * transform[ axis ].Inverse( );
  updateBoundings( axis );
  needUpdate[ axis ] = true;
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

size_t GuiImage::currentSlice( size_t axis ) {
  return( m_currentSlice[ axis ] );
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

void GuiImage::updateBoundings( size_t axis ) {
  Bial::Point3D start;
  Bial::Point3D end( image.size( 0 ), image.size( 1 ), 1 );
  if( image.Dims( ) > 2 ) {
    end = Bial::Point3D( image.size( 0 ), image.size( 1 ), image.size( 2 ) );
  }
  transform[ axis ]( start, &start );
  transform[ axis ]( end, &end );
  bounding[ axis ] = Bial::BBox( start, end );
  transform[ axis ] = transform[ axis ].Inverse( );
  transform[ axis ].Translate( bounding[ axis ].pMin.x, bounding[ axis ].pMin.y, bounding[ axis ].pMin.z );
  bounding[ axis ] = bounding[ axis ].Normalized( );
}
