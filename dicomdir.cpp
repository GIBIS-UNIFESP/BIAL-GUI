#include "dicomdir.h"
#ifdef LIBGDCM
#include <gdcmAttribute.h>
#include <gdcmReader.h>
#include <gdcmStreamImageReader.h>
#include <gdcmStringFilter.h>
#endif
#include "File.hpp"

/* ImageInfo */
std::string ImageInfo::getDir( ) const {
  return( dir );
}

void ImageInfo::setDir( const std::string &value ) {
  dir = value;
}

std::string ImageInfo::getFileName( ) {
  std::string imgpath = referencedFileID;
  for( size_t pos = 0; pos < imgpath.size( ); ++pos ) {
    if( ( ( imgpath[ pos ] == '\\' ) || ( imgpath[ pos ] == '/' ) ) && ( imgpath[ pos ] != DIR_SEPARATOR ) ) {
      imgpath[ pos ] = DIR_SEPARATOR;
    }
  }
  return( dir + imgpath );
}

ImageInfo::ImageInfo( ) {
}

std::string ImageInfo::getReferencedFileID( ) const {
  return( referencedFileID );
}

void ImageInfo::setReferencedFileID( const std::string &value ) {
  referencedFileID = value;
}

std::string ImageInfo::getUid( ) const {
  return( uid );
}

void ImageInfo::setUid( const std::string &value ) {
  uid = value;
}

std::string Series::getUid( ) const {
  return( uid );
}

void Series::setUid( const std::string &value ) {
  uid = value;
}

std::string Series::getDescr( ) const {
  return( descr );
}

void Series::setDescr( const std::string &value ) {
  descr = value;
}

std::vector< std::string > Series::getImages( ) {
  std::vector< std::string > files;
  for( auto itr = images.begin( ); itr != images.end( ); ++itr ) {
    files.push_back( itr->getFileName( ) );
  }
  return( files );
}

void Series::addImage( const ImageInfo &imageInfo ) {
  images.push_back( imageInfo );
}

std::string Series::getModality( ) const {
  return( modality );
}

void Series::setModality( const std::string &value ) {
  modality = value;
}
/* Study */
std::string Study::getUid( ) const {
  return( uid );
}

void Study::setUid( const std::string &value ) {
  uid = value;
}

std::string Study::getDescr( ) const {
  return( descr );
}

void Study::setDescr( const std::string &value ) {
  descr = value;
}

void Study::addSeries( const Series &s ) {
  series.push_back( s );
}

std::string Study::getDate( ) const {
  return( date );
}

void Study::setDate( const std::string &value ) {
  date = value;
}

std::vector< std::string > Study::getImages( ) {
  std::vector< std::string > files;
  for( auto itr = series.begin( ); itr != series.end( ); ++itr ) {
    std::vector< std::string > sFiles = itr->getImages( );
    files.insert( files.end( ), sFiles.begin( ), sFiles.end( ) );
  }
  return( files );
}
/* Patient */
std::string Patient::getName( ) const {
  return( name );
}

void Patient::setName( const std::string &value ) {
  name = value;
}

std::string Patient::getId( ) const {
  return( id );
}

void Patient::setId( const std::string &value ) {
  id = value;
}

void Patient::addStudy( const Study &s ) {
  study.push_back( s );
}

std::vector< std::string > Patient::getImages( ) {
  std::vector< std::string > files;
  for( auto itr = study.begin( ); itr != study.end( ); ++itr ) {
    std::vector< std::string > sFiles = itr->getImages( );
    files.insert( files.end( ), sFiles.begin( ), sFiles.end( ) );
  }
  return( files );
}
/* Dicomdir */
#ifdef LIBGDCM
std::string str( const gdcm::Value &val ) {
  std::stringstream strm;
  strm << val;
  return( strm.str( ) );
}

std::string str( gdcm::SmartPointer< gdcm::Value > val ) {
  std::stringstream strm;
  strm << *val;
  return( strm.str( ) );
}
#endif
DicomDir::DicomDir( ) {
}

bool DicomDir::open( const std::string &filename ) {
  this->filename = filename;
  title = Bial::File::Basename( filename );
  /* TODO Verify if file exists ( The function Bial::File::Exists do not exists yes ) */
  std::string dir = Bial::File::Directory( filename );
#ifndef LIBGDCM
  BIAL_WARNING( "GDCM library not found and/or reprocessor variable LIBGDCM not set!" );
  return( false );
#else
  gdcm::Reader reader;
  COMMENT( "Opening dicomdir file: " << filename, 1 );
  reader.SetFileName( filename.c_str( ) );
  if( !reader.Read( ) ) {
    BIAL_WARNING( "Cannot read file!" );
    return( false );
  }
  const gdcm::File &file = reader.GetFile( );
  const gdcm::FileMetaInformation header = file.GetHeader( );
  const gdcm::DataSet &ds = file.GetDataSet( );
  gdcm::MediaStorage ms;
  ms.SetFromFile( file );
  if( gdcm::MediaStorage::GetMSType( ms.GetString( ) ) != gdcm::MediaStorage::MediaStorageDirectoryStorage ) {
    BIAL_WARNING( "This file is not a DICOMDIR ( Media storage type: " << ms << " )" )
    return( false );
  }
  if( header.FindDataElement( gdcm::Tag( 0x0002, 0x0002 ) ) ) {
    if( str( header.GetDataElement( gdcm::Tag( 0x0002, 0x0002 ) ).GetValue( ) ) != "1.2.840.10008.1.3.10" ) {
      BIAL_WARNING( "This file is not a DICOMDIR" );
    }
  }
  else {
    BIAL_WARNING( "Media Storage SOP Class not present" );
    return( false );
  }
  for( auto it = ds.GetDES( ).begin( ); it != ds.GetDES( ).end( ); ++it ) {
    const gdcm::DataElement &de = *it;
    if( de.GetTag( ) == gdcm::Tag( 0x004, 0x1220 ) ) {   /* verifies if it is a valid ... */
      gdcm::SmartPointer< gdcm::SequenceOfItems > sq = de.GetValueAsSQ( );
      COMMENT( "Iterating through DicomDir itens", 1 );
      for( size_t itemNr = 1; itemNr < sq->GetNumberOfItems( ); ++itemNr ) {
        gdcm::Item &item = sq->GetItem( itemNr );
        if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
          gdcm::SmartPointer< gdcm::Value > val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
          while( ( str( val ) == "PATIENT " ) || ( str( val ) == "PATIENT" ) ) {
            COMMENT( "Creating Patient", 2 );
            Patient p;
            if( item.FindDataElement( gdcm::Tag( 0x0010, 0x0010 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0010, 0x0010 ) ).GetValue( );
              p.setName( str( val ) );
            }
            if( item.FindDataElement( gdcm::Tag( 0x0010, 0x0020 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0010, 0x0020 ) ).GetValue( );
              p.setId( str( val ) );
            }
            /* Next */
            item = sq->GetItem( ++itemNr );
            if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
            }
            while( ( str( val ) == "STUDY " ) || ( str( val ) == "STUDY" ) ) {
              COMMENT( "Creating Study", 2 )
              Study study;
/*              std::cout << str( val ) << std::endl; */
              if( item.FindDataElement( gdcm::Tag( 0x0020, 0x000d ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0020, 0x000d ) ).GetValue( );
/*                std::cout << "  UID = " << *val << std::endl; */
                study.setUid( str( val ) );
              }
              if( item.FindDataElement( gdcm::Tag( 0x0008, 0x0020 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0008, 0x0020 ) ).GetValue( );
/*                std::cout << "  Date = " << *val << std::endl; */
                study.setDate( str( val ) );
              }
              if( item.FindDataElement( gdcm::Tag( 0x0008, 0x1030 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0008, 0x1030 ) ).GetValue( );
/*                std::cout << "  Description = " << *val << std::endl; */
                study.setDescr( str( val ) );
              }
              item = sq->GetItem( ++itemNr );
              if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
              }
              while( str( val ) == "SERIES " || str( val ) == "SERIES" ) {
                COMMENT( "Creating Series", 2 );
                Series series;
/*                std::cout << "  " << str( val ) << std::endl; */
                if( item.FindDataElement( gdcm::Tag( 0x0020, 0x000e ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0020, 0x000e ) ).GetValue( );
/*                  std::cout << "    UID = " << *val << std::endl; */
                  series.setUid( str( val ) );
                }
                if( item.FindDataElement( gdcm::Tag( 0x0008, 0x0060 ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0008, 0x0060 ) ).GetValue( );
/*                  std::cout << "    Modality = " << *val << std::endl; */
                  series.setModality( str( val ) );
                }
                if( item.FindDataElement( gdcm::Tag( 0x0008, 0x103e ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0008, 0x103e ) ).GetValue( );
/*                  std::cout << "    Description = " << *val << std::endl; */
                  series.setDescr( str( val ) );
                }
                item = sq->GetItem( ++itemNr );
                if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
                }
                while( str( val ) == "IMAGE " || str( val ) == "IMAGE" ) {
                  COMMENT( "Creating ImageInfo", 2 );
                  ImageInfo imageInfo;
                  imageInfo.setDir( dir );
/*
 *                  std::cout << "    " << item << std::endl;
 *                  std::cout << "    " << *val << std::endl;
 */
                  if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1511 ) ) ) {
                    val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1511 ) ).GetValue( );
/*                    std::cout << "      UID = " << *val << std::endl; */
                    imageInfo.setUid( str( val ) );
                  }
                  if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1500 ) ) ) {
                    val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1500 ) ).GetValue( );
/*                    std::cout << "      ReferencedFileID = " << *val << std::endl; */
                    imageInfo.setReferencedFileID( str( val ) );
                  }
                  if( itemNr < sq->GetNumberOfItems( ) ) {
                    item = sq->GetItem( ++itemNr );
                    if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                      val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
                    }
                  }
                  else {
                    break;
                  }
                  series.addImage( imageInfo );
                }
                study.addSeries( series );
              }
              p.addStudy( study );
            }
            patient.push_back( p );
          }
        }
      }
    }
  }
#endif
  return( true );
}

std::vector< std::string > DicomDir::getImages( ) {
  std::vector< std::string > files;
  for( auto itr = patient.begin( ); itr != patient.end( ); ++itr ) {
    std::vector< std::string > pFiles = itr->getImages( );
    files.insert( files.end( ), pFiles.begin( ), pFiles.end( ) );
  }
  return( files );
}
