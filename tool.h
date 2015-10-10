#ifndef TOOL_H
#define TOOL_H

#include "guiimage.h"
#include "imageviewer.h"

#include <QPointF>
#include <QString>

class Tool {
protected:
  GuiImage *guiImage;
  ImageViewer * viewer;
public:
  enum{ DEFAULT };
  Tool( GuiImage *guiImage, ImageViewer * viewer );

  virtual QString name() = 0;
  virtual int type() = 0;
  virtual Modality supportedFormats() = 0;

  virtual void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseMoved( QPointF pt, size_t axis ) = 0;
};

#endif /* TOOL_H */
