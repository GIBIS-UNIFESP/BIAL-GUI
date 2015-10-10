#ifndef DEFAULTTOOL_H
#define DEFAULTTOOL_H

#include "tool.h"


class DefaultTool : public Tool {

public:
  DefaultTool( GuiImage *guiImage, ImageViewer *viewer );

  /* Tool interface */
public:
  int type( );
  Modality supportedFormats( );

  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );

public slots:
  void imageUpdated( );

private:
  /**
   *
   * @brief changeOthersSlices is called when the mouse is clicked over
   *  an 3d image view (axial, sagittal and coronal).
   * @param posF is the mouse pointer position in the scene.
   * @param view is the number of the view's axis.
   *
   */
  void changeOtherSlices( QPointF posF, size_t view );
  void updateOverlay( QPointF pt, size_t axis );

};

#endif /* DEFAULTTOOL_H */
