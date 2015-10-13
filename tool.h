#ifndef TOOL_H
#define TOOL_H

#include "guiimage.h"
#include "imageviewer.h"

#include <QPointF>
#include <QString>

class Tool : public QObject {
  Q_OBJECT
protected:
  GuiImage * guiImage;
  ImageViewer *viewer;
  bool visible;
public:

  Tool( GuiImage *guiImage, ImageViewer *viewer );

  virtual int type( ) = 0;
  virtual void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseMoved( QPointF pt, size_t axis ) = 0;
  virtual void sliceChanged( size_t axis, size_t slice ) = 0;
  virtual void setVisible( bool value );

  bool isVisible( );
};

#endif /* TOOL_H */
