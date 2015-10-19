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
  explicit SegmentationWidget( QWidget *parent = 0 );
  ~SegmentationWidget( );
  void setTool( Tool *sTool );


private slots:

  void on_SegmentationButton_clicked( );
  void on_eraserButton_clicked( );
  void on_drawButton_clicked( );
  void on_ClearButton_clicked( );

  void on_AlphaSpinBox_valueChanged( double arg1 );

  void on_BetaSpinBox_valueChanged( double arg1 );

private:
  Ui::Segmentationwidget *ui;
  SegmentationTool *tool;


};

#endif /* SEGMENTATIONWIDGET_H */
