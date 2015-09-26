/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file Dicomdir.h
 * @author Lucas Santana Lellis
 * @date 2015/Jul/21
 * @version 2.0.00
 * @brief
 * Content: Dicomdir related classes
 * <br> Description: Dicomdir related classes and GDCM library integration.
 * <br> Future add-on's: unknown.
 */

#include "Common.hpp"
#ifndef DICOMDIR_H
#define DICOMDIR_H
class ImageInfo {
  std::string uid;
  std::string referencedFileID;
  std::string dir;
public:
  ImageInfo( );
  std::string getUid( ) const;
  void setUid( const std::string &value );
  std::string getReferencedFileID( ) const;
  void setReferencedFileID( const std::string &value );
  std::string getDir( ) const;
  void setDir( const std::string &value );
  std::string getFileName( );
};
class Series {
  std::vector< ImageInfo > images;
  std::string uid;
  std::string modality;
  std::string descr;
public:
  void addImage( const ImageInfo &imageInfo );
  std::string getUid( ) const;
  void setUid( const std::string &value );
  std::string getModality( ) const;
  void setModality( const std::string &value );
  std::string getDescr( ) const;
  void setDescr( const std::string &value );
  std::vector< std::string > getImages( );
};
class Study {
  std::vector< Series > series;
  std::string uid;
  std::string date;
  std::string descr;
public:
  void addSeries( const Series &s );
  std::string getUid( ) const;
  void setUid( const std::string &value );
  std::string getDate( ) const;
  void setDate( const std::string &value );
  std::string getDescr( ) const;
  void setDescr( const std::string &value );
  std::vector< std::string > getImages( );
};
class Patient {
  std::vector< Study > study;
  std::string name;
  std::string id;
public:
  void addStudy( const Study &s );
  std::string getName( ) const;
  void setName( const std::string &value );
  std::string getId( ) const;
  void setId( const std::string &value );
  std::vector< std::string > getImages( );
};
class DicomDir {
  std::vector< Patient > patient;
  std::string filename;
  std::string title;
  std::string dir;
public:
  DicomDir( );
  bool open( const std::string &filename );
  std::string getTitle( ) const;
  void setTitle( const std::string &value );
  std::string getFilename( ) const;
  std::vector< std::string > getImages( );
};

#endif /* DICOMDIR_H */
