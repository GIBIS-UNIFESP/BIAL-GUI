#include "Common.hpp"
#include "displayformat.h"

BWFormat::BWFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::BW;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW0;

  m_hasViewerControls = false;
  m_enableTools = true;
  m_rotateAll = false;
  m_rotateSingle = true;
  m_showNiftiViews = false;
  m_showNiftiAxis = false;
  m_showOrientation = false;
  m_showPpmViews = false;
  m_showPpmChannels = false;
  m_hasOverlay = false;
  m_hasLayout = false;
  m_has3Views = false;
  m_has4Views = false;
  m_overlay = false;
}

NIfTIFormat::NIfTIFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::NIfTI;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW012;
  m_hasViewerControls = true;
  m_enableTools = true;
  m_rotateAll = true;
  m_rotateSingle = false;
  m_showNiftiViews = true;
  m_showNiftiAxis = false;
  m_showOrientation = true;
  m_showPpmViews = false;
  m_showPpmChannels = false;
  m_hasOverlay = true;
  m_hasLayout = true;
  m_has3Views = true;
  m_has4Views = false;
  m_overlay = false;
}

void NIfTIFormat::setCurrentViews( const Views &currentViews ) {
  m_currentViews = currentViews;
  emit updated( );
}

RGBFormat::RGBFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::RGB;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW0;
  m_hasViewerControls = false;
  m_enableTools = false;
  m_rotateAll = true;
  m_rotateSingle = false;
  m_showNiftiViews = false;
  m_showNiftiAxis = false;
  m_showPpmViews = true;
  m_showOrientation = false;
  m_showPpmChannels = true;
  m_hasOverlay = false;
  m_hasLayout = false;
  m_has3Views = false;
  m_has4Views = true;
  m_overlay = false;
}

DisplayFormat::DisplayFormat( QObject *parent ) : QObject( parent ) {
  m_overlayColor = Qt::green;
}

Modality DisplayFormat::modality( ) const {
  return( m_modality );
}
Layout DisplayFormat::currentLayout( ) const {
  return( m_currentLayout );
}
Views DisplayFormat::currentViews( ) const {
  return( m_currentViews );
}
bool DisplayFormat::hasViewerControls( ) const {
  return( m_hasViewerControls );
}
bool DisplayFormat::enableTools( ) const {
  return( m_enableTools );
}
bool DisplayFormat::rotateAll( ) const {
  return( m_rotateAll );
}
bool DisplayFormat::rotateSingle( ) const {
  return( m_rotateSingle );
}
bool DisplayFormat::hasOverlay( ) const {
  return( m_hasOverlay );
}
bool DisplayFormat::showNiftiViews( ) const {
  return( m_showNiftiViews );
}
bool DisplayFormat::showNiftiAxis( ) const {
  return( m_showNiftiAxis );
}
bool DisplayFormat::showOrientation( ) const {
  return( m_showOrientation );
}
bool DisplayFormat::showPpmViews( ) const {
  return( m_showPpmViews );
}
bool DisplayFormat::showPpmChannels( ) const {
  return( m_showPpmChannels );
}
bool DisplayFormat::hasLayout( ) const {
  return( m_hasLayout );
}
bool DisplayFormat::has3Views( ) const {
  return( m_has3Views );
}
bool DisplayFormat::has4Views( ) const {
  return( m_has4Views );
}

std::array< bool, 4 > DisplayFormat::getViews( ) {
  std::array< bool, 4 > views;

  views[ 0 ] = ( int ) currentViews( ) & ( int ) Views::SHOW0;
  views[ 1 ] = ( int ) currentViews( ) & ( int ) Views::SHOW1;
  views[ 2 ] = ( int ) currentViews( ) & ( int ) Views::SHOW2;
  views[ 3 ] = ( int ) currentViews( ) & ( int ) Views::SHOW3;

  return( views );
}
int DisplayFormat::getNumberOfViews( ) const {
  return( m_numberOfViews );
}

QColor DisplayFormat::overlayColor( ) const {
  return( m_overlayColor );
}

void DisplayFormat::setOverlayColor( const QColor &overlayColor ) {
  m_overlayColor = overlayColor;
  emit updated( );
}

bool DisplayFormat::overlay( ) const {
  return( m_overlay );
}

void DisplayFormat::setCurrentLayout( const Layout &currentLayout ) {
  COMMENT( "Layout set to " << ( int ) currentLayout, 0 );
  m_currentLayout = currentLayout;
  emit updated( );
}

void DisplayFormat::setCurrentViews( const Views &currentViews ) {
  COMMENT( "View set to " << ( int ) currentViews, 0 );
  switch( currentViews ) {
      case Views::SHOW0:
      case Views::SHOW1:
      case Views::SHOW2:
      case Views::SHOW3:
      setNumberOfViews( 1 );
      break;
      case Views::SHOW012:
      case Views::SHOW123:
      setNumberOfViews( 3 );
      break;
      case Views::SHOW0123:
      setNumberOfViews( 4 );
      break;
  }
  m_currentViews = currentViews;
  emit updated( );
}
void DisplayFormat::setOverlay( bool overlay ) {
  COMMENT( "Overlay set to " << overlay, 0 );
  m_overlay = overlay;
  emit updated( );
}

void DisplayFormat::toggleOverlay( ) {
  setOverlay( !overlay( ) );
}

void BWFormat::setNumberOfViews( int numberOfViews ) {
  if( numberOfViews != 1 ) {
    throw std::invalid_argument( "The number of views cannot be changed!" );
  }
}

void NIfTIFormat::setNumberOfViews( int numberOfViews ) {
  if( ( numberOfViews != 1 ) && ( numberOfViews != 3 ) ) {
    throw std::invalid_argument( "Invalid number of views!" );
  }
  m_currentLayout = Layout::GRID;
  if( m_numberOfViews != numberOfViews ) {
    if( numberOfViews == 1 ) {
      m_currentViews = Views::SHOW0;
      m_showNiftiAxis = true;
      m_showOrientation = false;
    }
    else {
      m_currentViews = Views::SHOW012;
      m_showNiftiAxis = false;
      m_showOrientation = true;
    }
    m_numberOfViews = numberOfViews;
    emit updated( );
  }
}

void RGBFormat::setNumberOfViews( int numberOfViews ) {
  if( ( numberOfViews != 1 ) && ( numberOfViews != 4 ) ) {
    throw std::invalid_argument( "Invalid number of views!" );
  }
  m_currentLayout = Layout::GRID;
  if( m_numberOfViews != numberOfViews ) {
    if( numberOfViews == 1 ) {
      m_currentViews = Views::SHOW0;
      m_showPpmChannels = true;
    }
    else {
      m_currentViews = Views::SHOW0123;
      m_showPpmChannels = false;
    }
    m_numberOfViews = numberOfViews;
    emit updated( );
  }
}
