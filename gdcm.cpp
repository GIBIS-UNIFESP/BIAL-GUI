#include "gdcm.h"
#include <Common.hpp>
#include "FileBasics.hpp"

#ifdef LIBGDCM
#include <gdcmImageReader.h>

Bial::Image< int > Convert2D( const gdcm::Image &gimage ) {
  if( gimage.IsEmpty( ) ) {
    BIAL_WARNING( "GDCM GImage is empty!" )
    throw std::runtime_error( BIAL_ERROR( "GDCM GImage is empty!" ) );
  }
  std::vector< char > vbuffer( gimage.GetBufferLength( ) );
  char *buffer = &vbuffer[ 0 ];
  gimage.GetBuffer( buffer );
  const std::vector< size_t > spc_dim = { gimage.GetDimension( 0 ), gimage.GetDimension( 1 ) };
  const std::vector< float > pixel_size = { static_cast< float >( gimage.GetSpacing( )[ 0 ] ),
                                            static_cast< float >( gimage.GetSpacing( )[ 1 ] ) };
  if( gimage.GetPhotometricInterpretation( ) == gdcm::PhotometricInterpretation::RGB ) {
    if( gimage.GetPixelFormat( ) != gdcm::PixelFormat::UINT8 ) {
      throw std::runtime_error( BIAL_ERROR( "Unhandled pixel format!" ) );
    }
    Bial::Image< int > res( spc_dim, pixel_size, 3 );
    const size_t channelSz = res.size(0) * res.size(1);
    for( size_t pxl = 0; pxl < channelSz; ++pxl ) {
      res[ pxl ] = static_cast< int >( *buffer++ );
      res[ pxl + channelSz ] = static_cast< int >( *buffer++ );
      res[ pxl + channelSz * 2 ] = static_cast< int >( *buffer++ );
    }
    return( res );
  }
  else if( gimage.GetPhotometricInterpretation( ) == gdcm::PhotometricInterpretation::MONOCHROME2 ) {
    Bial::Image< int > res( spc_dim, pixel_size );
    COMMENT( "MONO2 FORMAT", 1 );
    if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT8 ) {
      COMMENT( "UINT8", 1 )
      unsigned char *ubuffer = ( unsigned char* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *ubuffer++ );
      }
    }
    else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::INT16 ) {
      COMMENT( "INT16", 1 );
      short *buffer16 = ( short* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *buffer16++ );
      }
    }
    else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT16 ) {
      COMMENT( "UINT16", 1 );
      unsigned short *buffer16 = ( unsigned short* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *buffer16++ );
      }
    }
    else {
      BIAL_WARNING( "Unhandled Pixel Format!" << ". Format is: " << gimage.GetPixelFormat( ) << ". Pixel Format is: "
                                              << gimage.GetPixelFormat( ) << std::endl );
      throw std::runtime_error( BIAL_ERROR( "Unhandled Pixel Format!" ) );
    }
    return( res );
  }
  else {
    BIAL_WARNING( "Unhandled PhotometricInterpretation" << gimage.GetPhotometricInterpretation( ) )
    throw std::runtime_error( BIAL_ERROR( "Unhandled PhotometricInterpretation" ) );
  }
}

Bial::Image< int > GDCM::OpenGImage( const std::string &filename ) {
  gdcm::ImageReader ir;
  ir.SetFileName( filename.c_str( ) );
  try{
    return Bial::File::Read<int>(filename);
  }
  catch( std::bad_alloc e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::runtime_error e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::out_of_range e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::logic_error e ) {
    BIAL_WARNING( e.what( ) );
  }
  if( !ir.Read( ) ) {
    BIAL_WARNING( "Could not read " << filename << " with gdcm.");
    throw std::runtime_error(BIAL_ERROR("GDCM CAN'T READ THIS IMAGE"));
  }
  COMMENT( "Getting image from GDCM ImageReader.", 1 );
  gdcm::Image gimage = ir.GetImage( );
  if( gimage.IsEmpty( ) ) {
    BIAL_WARNING( "GDCM GImage is empty!" );
    throw std::runtime_error(BIAL_ERROR("GDCM CAN'T READ THIS IMAGE"));
  }
  if( gimage.GetNumberOfDimensions( ) == 2 ) {
    return( Convert2D( gimage ) );
  }else{
    BIAL_WARNING( "3D image conversion not implemented yet." );
    throw std::runtime_error(BIAL_ERROR("3D image conversion not implemented yet."));
  }
  return Bial::Image<int>();
}

#else

Bial::Image< int > GDCM::OpenIImage( const std::string &filename ) {
  return( Bial::Image< int >( Bial::Read< int >( filename ) ) );
}

Bial::Image< float > GDCM::OpenFImage( const std::string &filename ) {
  return( Bial::Image< float >( Bial::Read< float >( filename ) ) );
}

Bial::Image< Bial::Color > GDCM::OpenCImage( const std::string &filename ) {
  return( Bial::Image< Bial::Color >( Bial::Read( filename ) ) );
}

#endif
