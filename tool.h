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
  bool m_visible;
  bool m_hasLabel = false;
public:

  Tool( GuiImage *guiImage, ImageViewer *viewer );

  virtual int type( ) = 0;
  virtual void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseMoved( QPointF pt, size_t axis ) = 0;
  virtual void sliceChanged( size_t axis, size_t slice ) = 0;
  void setVisible( bool value );
  virtual QPixmap getLabel( size_t axis );

  bool visible( ) const;
  bool hasLabel( ) const;

protected:
  void setHasLabel( bool sHasLabel );
};

#endif /* TOOL_H */
