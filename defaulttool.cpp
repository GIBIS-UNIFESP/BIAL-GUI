#include "defaulttool.h"

DefaultTool::DefaultTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage, viewer ) {

}

int DefaultTool::type( ) {
  return( DEFAULT );
}

Modality DefaultTool::supportedFormats( ) {
  return( ( Modality ) ( ( int ) Modality::BW2D | ( int ) Modality::BW3D | ( int ) Modality::RGB2D ) );
}

void DefaultTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse released.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
  }
}

void DefaultTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse clicked.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
  }
}

void DefaultTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse dragged.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
  }
}

void DefaultTool::mouseMoved( QPointF pt, size_t axis ) {
  COMMENT( "Mouse moved.", 2 );
  Q_UNUSED(pt)
  Q_UNUSED(axis)
}


void DefaultTool::changeOtherSlices( QPointF posF, size_t view ) {
  COMMENT( "Changing slice position of other frames based on image position.", 2 );
  if( ( guiImage->modality( ) == Modality::BW3D ) ) {
    Bial::FastTransform transform = guiImage->getTransform( view );
    Bial::Point3D pt = transform( ( double ) posF.x( ), ( double ) posF.y( ),
                                  ( double ) guiImage->currentSlice( view ) );
    for( size_t other = 0; other < 3; ++other ) {
      if( other != view ) {
        Bial::FastTransform otherTransf = guiImage->getTransform( other ).Inverse( );
        Bial::Point3D otherPt = otherTransf( pt );
        size_t pos = static_cast< size_t >( round( otherPt.z ) );
        if( pos < guiImage->depth( other ) ) {
          guiImage->setCurrentSlice( other, pos );
        }
      }
    }
  }
}


void DefaultTool::imageUpdated()
{
}
