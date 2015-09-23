#include "guiimage.h"
#include <QPixmap>

GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ), m_fileName( fname ) {
  image = Bial::Image< int >::Read( fname.toStdString( ) );
  transform.resize( 4 );
  bounding.insert( 0, 4, Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) ) );
  m_currentSlice.insert( 0, 4, 0 );

  m_max = image.Maximum( );
  if( image.Dims( ) == 3 ) {
    COMMENT( "NIfTI image detected.", 2 );
    m_modality = Modality::NIfTI;
    {
      COMMENT( "Generating Axial affine transform.", 2 );
      transform[ 0 ].Rotate( 90.0, Bial::Transform3D::X ).Rotate( 90.0, Bial::Transform3D::Y );
      Bial::Point3D start, end( image.size( 0 ), image.size( 1 ), image.size( 2 ) );
      transform[ 0 ]( start, &start );
      transform[ 0 ]( end, &end );
      bounding[ 0 ] = Bial::BBox( start, end );
      transform[ 0 ] = transform[ 0 ].Inverse( );
      transform[ 0 ].Translate( bounding[ 0 ].pMin.x, bounding[ 0 ].pMin.y, bounding[ 0 ].pMin.z );
      bounding[ 0 ] = bounding[ 0 ].Normalized( );
    }
    {
      COMMENT( "Generating Coronal affine transform.", 2 );
      transform[ 1 ].Rotate( 180.0, Bial::Transform3D::Z ).Rotate( 90.0, Bial::Transform3D::Y );
      Bial::Point3D start, end( image.size( 0 ), image.size( 1 ), image.size( 2 ) );
      transform[ 1 ]( start, &start );
      transform[ 1 ]( end, &end );
      bounding[ 1 ] = Bial::BBox( start, end );
      transform[ 1 ] = transform[ 1 ].Inverse( );
      transform[ 1 ].Translate( bounding[ 1 ].pMin.x, bounding[ 1 ].pMin.y, bounding[ 1 ].pMin.z );
      bounding[ 1 ] = bounding[ 1 ].Normalized( );
    }
    {
      COMMENT( "Generating Sagittal affine transform.", 2 );
      transform[ 2 ].Rotate( 180.0, Bial::Transform3D::Z );
      Bial::Point3D start, end( image.size( 0 ), image.size( 1 ), image.size( 2 ) );
      transform[ 2 ]( start, &start );
      transform[ 2 ]( end, &end );
      bounding[ 2 ] = Bial::BBox( start, end );
      transform[ 2 ] = transform[ 2 ].Inverse( );
      transform[ 2 ].Translate( bounding[ 2 ].pMin.x, bounding[ 2 ].pMin.y, bounding[ 2 ].pMin.z );
      bounding[ 2 ] = bounding[ 2 ].Normalized( );
    }
    for( int axis = 0; axis < m_currentSlice.size( ); ++axis ) {
      setCurrentSlice( axis, depth( axis ) / 2 );
    }
  }
  else if( ( image.Dims( ) == 2 ) && ( image.Channels( ) == 3 ) ) {
    COMMENT( "PPM image detected.", 2 );
    m_modality = Modality::RGB;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) );
    bounding[ 0 ] = box;
  }
  else if( ( image.Dims( ) == 2 ) && ( image.Channels( ) == 1 ) ) {
    COMMENT( "Gray image detected.", 2 );
    m_modality = Modality::BW;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( image.size( 0 ), image.size( 1 ), 1 ) );
    bounding[ 0 ] = box;
  }
}

Modality GuiImage::modality( ) {
  return( m_modality );
}

QString GuiImage::fileName( ) {
  return( m_fileName );
}

QPixmap GuiImage::getSlice( size_t axis, size_t slice ) {
  COMMENT( "GET SLICE: image = " << m_fileName.toStdString() << ", axis = " << axis << ", slice = " << slice, 0 );
  if( slice >= depth( axis ) ) {
    throw( std::out_of_range( BIAL_ERROR( QString( "Slice is out of range. Expected < %1" ).arg( depth( axis ) ).
                                          toStdString( ) ) ) );
  }
  try {
    const size_t xsize = width( axis );
    const size_t ysize = heigth( axis );
    QImage res( xsize, ysize, QImage::Format_ARGB32 );
    const Bial::Transform3D &transf = transform[ axis ];

    double factor = 255.0 / ( double ) m_max;

//    COMMENT( "Xsize = " << xsize << ", Ysize = " << ysize, 0 );
    if( ( modality( ) == Modality::NIfTI ) || ( modality( ) == Modality::BW ) ) {
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) { /*  */
          Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
          int pixel = 0;
          if( image.ValidPixel( pos.x, pos.y, pos.z ) ) {
            pixel = static_cast< int >( image( pos.x, pos.y, pos.z ) * factor );
          }
          scanLine[ x ] = qRgb( pixel, pixel, pixel );
        }
      }
    }
    else if( modality( ) == Modality::RGB ) {
      if( axis == 0 ) {
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) { /*  */
            Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
            int r( 0 ), g( 0 ), b( 0 );
            if( image.ValidPixel( pos.x, pos.y ) ) {
              r = static_cast< int >( image( pos.x, pos.y, 0 ) * factor );
              g = static_cast< int >( image( pos.x, pos.y, 1 ) * factor );
              b = static_cast< int >( image( pos.x, pos.y, 2 ) * factor );
            }
            scanLine[ x ] = qRgb( r, g, b );
          }
        }
      }
      else {
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) { /*  */
            Bial::Point3D pos = transf( Bial::Point3D( x, y, slice ) );
            int pixel = 0;
            if( image.ValidPixel( pos.x, pos.y ) ) {
              pixel = static_cast< int >( image( pos.x, pos.y, axis - 1 ) * factor );
            }
            scanLine[ x ] = qRgb( pixel, pixel, pixel );
          }
        }
      }
    }
    return( QPixmap::fromImage( res ) );
  }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }
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
    if( m_currentSlice[ axis ] != slice && slice < depth(axis)) {
      m_currentSlice[ axis ] = slice;
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

Bial::Transform3D GuiImage::getTransform( size_t axis ) {
  return( transform.at( axis ) );
}

const Bial::Image< int > &GuiImage::getImage( ) const {
  return( image );
}

int GuiImage::max( ) {
  return( m_max );
}


size_t GuiImage::currentSlice( size_t axis ) {
  return( m_currentSlice[ axis ] );
}
