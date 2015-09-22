#ifndef DISPLAYFORMAT_H
#define DISPLAYFORMAT_H

#include <QVector>

enum class Modality {
  BW, RGB, NIfTI
};
enum class Layout {
  GRID, HORIZONTAL, VERTICAL
};
enum class Views {
  SHOW0, SHOW1, SHOW2, SHOW3, SHOW012, SHOW123, SHOW0123
};

class DisplayFormat {
public:
  DisplayFormat(Modality modality);

  Modality modality;
  Layout currentLayout;
  Views currentViews;

  bool viewerControls;
  bool enableTools;
  bool rotateAll;
  bool rotateSingle;
  bool hasOverlay;

  bool showNiftiViews;
  bool showNiftiAxis;
  bool showOrientation;
  bool showPpmViews;
  bool showPpmChannels;
};

#endif // DISPLAYFORMAT_H
