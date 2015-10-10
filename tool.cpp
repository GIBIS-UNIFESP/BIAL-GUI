#include "tool.h"

Tool::Tool( GuiImage *guiImage, ImageViewer *viewer ) : QObject( guiImage ), guiImage( guiImage ), viewer( viewer ) {
  connect( guiImage, &GuiImage::imageUpdated, this, &Tool::imageUpdated );
}

void Tool::setVisible( bool value ) {
  visible = value;
}

bool Tool::isVisible( ) {
  return( visible );
}
