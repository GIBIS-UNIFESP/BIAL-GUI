#include "controller.h"
#include "thumbswidget.h"

#include <QDebug>
#include <QFile>
#include <qsettings.h>

Controller::Controller( int views, QObject *parent ) : QObject( parent ), bwFormat(
    new BWFormat( this ) ), rgbFormat( new RGBFormat( this ) ), niftiFormat( new NIfTIFormat( this ) ) {
  for( int item = 0; item < views; ++item ) {
    m_pixmapItems.append( new QGraphicsPixmapItem( ) );
  }
  m_currentImagePos = -1;
  connect( rgbFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( niftiFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( bwFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( rgbFormat, &DisplayFormat::updated, this, &Controller::update );
  connect( niftiFormat, &DisplayFormat::updated, this, &Controller::update );
  connect( bwFormat, &DisplayFormat::updated, this, &Controller::update );
}

GuiImage* Controller::currentImage( ) {
  return( imageAt( currentImagePos( ) ) );
}

GuiImage* Controller::imageAt( int pos ) {
  if( ( pos >= 0 ) && ( pos < m_images.size( ) ) ) {
    return( m_images.at( pos ) );
  }
  return( nullptr );
}

int Controller::currentImagePos( ) const {
  return( m_currentImagePos );
}

bool Controller::addImage( QString fname ) {
  COMMENT( "Loading file " << fname.toStdString( ), 0 );
  GuiImage *img = nullptr;
  try {
    img = new GuiImage( fname, this );
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
  catch( ... ) {

  }
  if( img == nullptr ) {
    emit containerUpdated( );
    return( false );
  }
  m_images.append( img );


  m_thumbsWidget->addThumbnail( img );
  if( currentImagePos( ) == -1 ) {
    setCurrentImagePos( 0 );
  }
  setRecentFile( fname );

  emit containerUpdated( );

  return( true );
}

bool Controller::addLabel( QString label ) {
  /* TODO Controller::addLabel( QString label ) */
  return( false );
}

bool Controller::removeCurrentLabel( ) {
  /* TODO Controller::removeCurrentLabel( ) */
  return( false );
}

void Controller::removeCurrentImage( ) {
  m_images.removeAt( currentImagePos( ) );
  m_thumbsWidget->removeAt( currentImagePos( ) );
  if( currentImagePos( ) == 0 ) {
    setCurrentImagePos( 0 );
  }
  else {
    setCurrentImagePos( currentImagePos( ) - 1 );
  }
  emit containerUpdated( );
}

bool Controller::isEmpty( ) {
  return( m_images.isEmpty( ) );
}

void Controller::clear( ) {
  COMMENT( "Reseting images.", 1 );
  qDeleteAll( m_images );
  m_images.clear( );
  setCurrentImagePos( -1 );
  m_thumbsWidget->clear( );
  emit containerUpdated( );
}

int Controller::size( ) {
  return( m_images.size( ) );
}

void Controller::update( ) {
  COMMENT( "UPDATING IMAGE!", 0 );

  GuiImage *img = currentImage( );
  if( img ) {
    std::array< bool, 4 > showItens = currentFormat( )->getViews( );
    for( int axis = 0; axis < 4; ++axis ) {
      if( showItens[ axis ] ) {
        const QPixmap &pix = img->getSlice( axis );
        m_pixmapItems.at( axis )->setPixmap( pix );
      }
    }
  }
  else {
    for( int axis = 0; axis < m_pixmapItems.size( ); ++axis ) {
      m_pixmapItems[ axis ]->setPixmap( QPixmap( ) );
    }
  }
  emit imageUpdated( );
}

void Controller::setCurrentImagePos( int position ) {
  m_currentImagePos = position;
  if( currentImage( ) != nullptr ) {
    disconnect( currentImage( ), &GuiImage::imageUpdated, this, &Controller::update );
  }
  if( currentImage( ) != nullptr ) {
    emit currentImageChanged( );
    update( );
    connect( currentImage( ), &GuiImage::imageUpdated, this, &Controller::update );
  }
}

void Controller::loadNextImage( ) {
  if( currentImagePos( ) == ( m_images.count( ) - 1 ) ) {
    setCurrentImagePos( 0 );
  }
  else {
    setCurrentImagePos( currentImagePos( ) + 1 );
  }
}

void Controller::changeOthersSlices( QPointF posF, size_t axis ) {
  COMMENT( "Changing slice position of other frames based on image position.", 2 );
  if( currentImage( ) ) {
    if( ( currentImage( )->modality( ) == Modality::NIfTI ) ) {
      Bial::FastTransform transform = currentImage( )->getTransform( axis );
      Bial::Point3D pt = transform( ( double ) posF.x( ), ( double ) posF.y( ),
                                    ( double ) currentImage( )->currentSlice( axis ) );
      for( size_t other = 0; other < 3; ++other ) {
        if( other != axis ) {
          Bial::FastTransform otherTransf = currentImage( )->getTransform( other ).Inverse( );
          Bial::Point3D otherPt = otherTransf( pt );
          size_t pos = static_cast< size_t >( round( otherPt.z ) );
          if( pos < currentImage( )->depth( other ) ) {
            currentImage( )->setCurrentSlice( other, pos );
          }
        }
      }
    }
  }
  else {
    BIAL_WARNING( "CURRENT IMAGE NOT FOUND!" );
  }
}

void Controller::setCurrentSlice( size_t axis, size_t slice ) {
  currentImage( )->setCurrentSlice( axis, slice );
}

void Controller::setZoom( int value ) {
  /* FIXME Zoom doesn't work yet. */
  scale = 1.0 + value / 100.0;
  update( );
}

void Controller::setRecentFile( QString fname ) {
  COMMENT( "Setting recent file to : \"" << fname.toStdString( ) << "\"", 1 );
  if( !QFile( fname ).exists( ) ) {
    return;
  }
  QSettings settings;
  QStringList files = settings.value( "recentFileList" ).toStringList( );

  files.removeAll( fname );

  files.prepend( fname );
  while( files.size( ) > MaxRecentFiles ) {
    files.removeLast( );
  }
  settings.setValue( "recentFileList", files );

  emit recentFilesUpdated( );
}

void Controller::setThumbsWidget( ThumbsWidget *thumbsWidget ) {
  m_thumbsWidget = thumbsWidget;
  m_thumbsWidget->setController( this );
}

DisplayFormat* Controller::currentFormat( ) {
  Modality mod = currentImage( )->modality( );
  if( mod == Modality::RGB ) {
    return( rgbFormat );
  }
  else if( mod == Modality::NIfTI ) {
    return( niftiFormat );
  }
  else {
    return( bwFormat );
  }
}

QGraphicsPixmapItem* Controller::getPixmapItem( size_t axis ) {
  return( m_pixmapItems.at( axis ) );
}
