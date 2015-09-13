#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
  class ControlsWidget;
}

class ImageViewer;

class ControlsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ControlsWidget( QWidget *parent = 0 );
  ~ControlsWidget( );

  void installImageViewer( ImageViewer *viewer );

private:
  Ui::ControlsWidget *ui;
};

#endif /* CONTROLSWIDGET_H */
