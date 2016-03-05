#include "ColorBasics.hpp"
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


GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ), m_fileName( fname ) {

  std::string ext = Bial::File::Extension( fname.toStdString( ) );
  transform.resize( 4 );
  m_equalizeHistogram = false;
  if( ext == ".ppm" ) {
    COMMENT( "PPM image detected.", 2 );
    cimage = new Bial::Image< Bial::Color >( GDCM::OpenCImage( fname.toStdString( ) ) );
    m_type = image_type::type_color;
    m_dims = cimage->Dim( );
  }
  else {
    dimage = new Bial::Image< int >( GDCM::OpenIImage( fname.toStdString( ) ) );
    m_type = image_type::type_int;
    m_dims = dimage->Dim( );
  }
  bounding.insert( 0, 4, Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( m_dims( 0 ), m_dims( 1 ), 1 ) ) );
  m_currentSlice.insert( 0, 4, 0 );
  if( m_type == image_type::type_color ) {
    m_modality = Modality::RGB2D;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( m_dims( 0 ), m_dims( 1 ), 1 ) );
    bounding[ 0 ] = box;
    cachedPixmaps.resize( 4 );
    needUpdate.insert( 0, 4, true );
    /* TODO: Verificar quando é color e fazer máximo entre os máximos dos canal. */
    Bial::Color c_max = Bial::Maximum( *cimage );
    int d_max = std::max( std::max( c_max( 1 ), c_max( 2 ) ), c_max( 3 ) );
    m_max = d_max;
  }
  else {
    m_max = Bial::Maximum( *dimage );
    if( m_dims.size( ) == 3 ) {
      COMMENT( "NIfTI image detected.", 2 );
      m_modality = Modality::BW3D;
      if( Bial::NiftiHeader::IsNiftiFile( fname.toStdString( ) ) ) {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 0 ].Scale( 1, -1, -1 );
        updateBoundings( 0 );

        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 180.0, Bial::FastTransform::Z ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 1 ].Scale( -1, 1, 1 );
        updateBoundings( 1 );

        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
      else {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 0 );

        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 1 );

        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
      cachedPixmaps.resize( 3 );
      needUpdate.insert( 0, 3, true );
      for( int view = 0; view < m_currentSlice.size( ); ++view ) {
        setCurrentSlice( view, depth( view ) / 2 );
      }
      m_currentToolPos = -1;
    }
    else if( ( m_dims.size( ) == 2 ) ) {
      COMMENT( "Gray image detected.", 2 );
      m_modality = Modality::BW2D;
      Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( m_dims( 0 ), m_dims( 1 ), 1 ) );
      bounding[ 0 ] = box;
      cachedPixmaps.resize( 1 );
      needUpdate.push_back( true );
    }
  }
  COMMENT( "Computing equalizaztion transform.", 2 );
  /* TODO: Fazer histograma de imagem colorida. Sugestão: Fazer histograma de imagem convertida para Gray. */
  if( m_type == image_type::type_int ) {
    histogram = Bial::Signal::ZeroStartHistogram( *dimage );
  }
  else if( m_type == image_type::type_color ) {
    histogram = Bial::Signal::ZeroStartHistogram( Bial::ColorBasics::ARGBtoGraybyBrightness< int >( *cimage ) );
  }
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
  if( m_type == image_type::type_color ) {
    delete cimage;
  }
  else if( m_type == image_type::type_int ) {
    delete dimage;
  }
  else if( m_type == image_type::type_float ) {
    delete fimage;
  }
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
    if( modality( ) == Modality::BW2D ) {
      COMMENT( "Generating BW 2D view.", 2 );
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor)
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) {
          int pixel = 0;
          int xx, yy, zz;
          transf( x, y, slice, &xx, &yy, &zz );
          if( m_type == image_type::type_int ) {
            pixel = (*dimage)( xx, yy);
          }
          if( m_type == image_type::type_float ) {
            pixel = (*fimage)( xx, yy);
          }
          if( m_equalizeHistogram ) {
            pixel = equalization[ pixel ];
          }
          pixel *= factor;
          scanLine[ x ] = qRgb( pixel, pixel, pixel );
        }
      }
    }
    else if( modality( ) == Modality::BW3D ) {
      COMMENT( "Generating BW 3D view.", 2 );
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor)
      for( size_t y = 0; y < ysize; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) {
          int pixel = 0;
          int xx, yy, zz;
          transf( x, y, slice, &xx, &yy, &zz );
          if( m_type == image_type::type_int ) {
            pixel = (*dimage)( xx, yy, zz );
          }
          if( m_type == image_type::type_float ) {
            pixel = (*fimage)( xx, yy, zz );
          }
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
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor)
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            int xx, yy, zz;
            transf( x, y, slice, &xx, &yy, &zz );
            size_t pos = cimage->Position( xx, yy );
            int r = ( *cimage )[ pos ]( 1 );
            int g = ( *cimage )[ pos ]( 2 );
            int b = ( *cimage )[ pos ]( 3 );
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

image_type GuiImage::getImageType( ) const {
  return( m_type );
}

const Bial::Image< int > &GuiImage::getIImage( ) const {
  return( *dimage );
}

const Bial::Image< Bial::Color > &GuiImage::getCImage( ) const {
  return( *cimage );
}

const Bial::Image< float > &GuiImage::getFImage( ) const {
  return( *fimage );
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
    if( dimage->ValidCoordinate( ( size_t ) x, ( size_t ) y ) ) {
      color = dimage->at( x, y );
    }
  }
  else if( modality( ) == Modality::BW3D ) {
    if( dimage->ValidCoordinate( ( size_t ) x, ( size_t ) y, ( size_t ) z ) ) {
      color = dimage->at( x, y, z );
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
  if( currentToolPos < static_cast< size_t >( tools.size( ) ) ) {
    m_currentToolPos = currentToolPos;
  }
}

void GuiImage::updateBoundings( size_t axis ) {
  Bial::Point3D start;
  Bial::Point3D end( m_dims( 0 ) - 1, m_dims( 1 ) - 1, 1 );
  if( m_dims.size( ) > 2 ) {
    end = Bial::Point3D( m_dims( 0 ) - 1, m_dims( 1 ) - 1, m_dims( 2 ) - 1 );
  }
  transform[ axis ]( start, &start );
  transform[ axis ]( end, &end );
  bounding[ axis ] = Bial::BBox( start, end );
  transform[ axis ] = transform[ axis ].Inverse( );
  transform[ axis ].Translate( bounding[ axis ].pMin.x, bounding[ axis ].pMin.y, bounding[ axis ].pMin.z );
  bounding[ axis ] = bounding[ axis ].Normalized( );
}
