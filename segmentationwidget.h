#ifndef SEGMENTATIONWIDGET_H
#define SEGMENTATIONWIDGET_H


#include "segmentationtool.h"

#include <QWidget>

namespace Ui {
  class Segmentationwidget;
}

class SegmentationWidget : public QWidget {
  Q_OBJECT

public:
  explicit SegmentationWidget(QWidget *parent = 0);
  ~SegmentationWidget();
  void setTool(Tool* sTool);

//signals:
//    void mouseSelected(short mode);
//    void drawSelected(short mode);
//    void eraserSelected(short mode);
//    void clearSelected();

private slots:

  void on_SegmentationButton_clicked();
  void on_eraserButton_clicked();
  void on_drawButton_clicked();
  void on_mouseButton_clicked();
  void on_ClearButton_clicked();

  private:
  Ui::Segmentationwidget *ui;
  SegmentationTool* tool;


};

#endif // SEGMENTATIONWIDGET_H
