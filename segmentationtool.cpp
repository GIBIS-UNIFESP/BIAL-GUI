#include "Draw.hpp"
#include "Geometrics.hpp"
#include "guiimage.h"
#include "segmentationtool.h"
#include <QDebug>
#include <QPointF>

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage,
                                                                                      viewer ), seeds(
    guiImage->getImage( ).Dim( ) ) {

  drawing = false;
  setObjectName( "SegmentationTool" );
  setHasLabel(true);
}

int SegmentationTool::type( ) {
  return( SegmentationTool::Type );
}

void SegmentationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {

}

void SegmentationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  drawing = true;
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  lastPoint = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
  qDebug( ) << "Mouse clicked at " << pt;
}

void SegmentationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  /* nothing happens */
}

void SegmentationTool::mouseMoved( QPointF pt, size_t axis ) {

}

void SegmentationTool::sliceChanged( size_t axis, size_t slice ) {

}


QPixmap SegmentationTool::getLabel( size_t axis ) {
  QPixmap pix( guiImage->width(axis), guiImage->heigth(axis));
  pix.fill(Qt::white);
  return pix;
}
