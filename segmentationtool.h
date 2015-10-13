#ifndef SEGMENTATIONTOOL_H
#define SEGMENTATIONTOOL_H

#include "tool.h"


class SegmentationTool : public Tool {
private:
  Bial::Image< char > seeds;
  Bial::Point3D lastPoint;
  bool drawing;

public:
  enum { Type = 1 };
  SegmentationTool( GuiImage *guiImage, ImageViewer *viewer );
  static const int supportedFormats =
    ( ( int ) Modality::BW2D | ( int ) Modality::BW3D );

  /* Tool interface */
public:
  int type( );
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
};

#endif /* SEGMENTATIONTOOL_H */