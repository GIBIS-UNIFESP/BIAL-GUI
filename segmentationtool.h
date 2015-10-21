#ifndef SEGMENTATIONTOOL_H
#define SEGMENTATIONTOOL_H

#include "tool.h"
#include "Common.hpp"

class SegmentationTool : public Tool {
private:
  Bial::Image< char > seeds;
  Bial::Image< char > mask;

  Bial::Point3D lastPoint;
  bool drawing;
  int drawType;
  double alpha;
  double beta;
  bool seedsVisible;
  bool maskVisible;
  QTime timer;
  std::array<QPixmap, 4> pixmaps;

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
  QPixmap getLabel( size_t axis );

  void drawSeed( Bial::Point3D last, Bial::Point3D actual );
  void setDrawType( int type );
  void clearSeeds( );
  Bial::Image< char > segmentationOGS( double alpha, double beta );

  double getAlpha( ) const;
  void setAlpha( double value );
  double getBeta( ) const;
  void setBeta( double value );
  int getDrawType( ) const;

  void setSeedsVisibility(bool vis);
  void setMaskVisibility(bool vis);

  bool getSeedsVisible() const;
  bool getMaskVisible() const;
};

#endif /* SEGMENTATIONTOOL_H */
