#include "defaulttool.h"

DefaultTool::DefaultTool(GuiImage *guiImage , ImageViewer * viewer) : Tool( guiImage, viewer ) {

}

QString DefaultTool::name( ) {
  return( "Default" );
}

int DefaultTool::type( ) {
  return( DEFAULT );
}

Modality DefaultTool::supportedFormats( ) {
  return( (Modality)((int) Modality::BW2D | (int) Modality::BW3D | (int) Modality::RGB2D) );
}

void DefaultTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
}

void DefaultTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
}

void DefaultTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
}

void DefaultTool::mouseMoved( QPointF pt, size_t axis ) {
}
