#ifndef DISPLAYFORMAT_H
#define DISPLAYFORMAT_H

#include <QVector>

enum class Modality {
  BW, RGB, NIfTI
};
enum class Layout {
  NONE, GRID, HORIZONTAL, VERTICAL
};
enum class Views {
  NONE, SHOW0, SHOW1, SHOW2, SHOW3, SHOW012, SHOW123, SHOW0123
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

  QVector<Views> availbleViewOptions;
};

#endif // DISPLAYFORMAT_H
