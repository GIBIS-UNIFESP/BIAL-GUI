#ifndef DISPLAYFORMAT_H
#define DISPLAYFORMAT_H

#include <QVector>

enum class Modality {
  BW, RGB, NIfTI, NONE
};
enum class Layout {
  GRID, HORIZONTAL, VERTICAL
};
enum class Views {
  SHOW1, SHOW2, SHOW3, SHOW4, SHOW012, SHOW123, SHOW0123
};

class DisplayFormat {
public:
  DisplayFormat(Modality modality);

  Modality modality;
  Layout currentLayout;
  Views currentViews;

  bool viewerControls;
  bool enableTools;

  QVector<Views> availbleViewOptions;
  QVector<Views> availbleLayoutOptions;
};

#endif // DISPLAYFORMAT_H
