#ifndef DEFAULTTOOL_H
#define DEFAULTTOOL_H

#include "tool.h"


class DefaultTool : public Tool {
public:
  DefaultTool( GuiImage * guiImage, ImageViewer * viewer );

  // Tool interface
public:
  QString name();
  int type();
  Modality supportedFormats();

  void mouseReleased(QPointF pt, Qt::MouseButtons buttons, size_t axis);
  void mouseClicked(QPointF pt, Qt::MouseButtons buttons, size_t axis);
  void mouseDragged(QPointF pt, Qt::MouseButtons buttons, size_t axis);
  void mouseMoved(QPointF pt, size_t axis);

};

#endif /* DEFAULTTOOL_H */
