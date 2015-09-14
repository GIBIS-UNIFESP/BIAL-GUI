#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
  class ControlsWidget;
}

class ImageViewer;
class Controller;

class ControlsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ControlsWidget( QWidget *parent = 0 );
  ~ControlsWidget( );

  void installImageViewer( ImageViewer *viewer );

  void setController( Controller *value );

protected slots:
  void imageChanged( );
  void updateRange( );

private:
  Ui::ControlsWidget *ui;
  Controller *controller;
};

#endif /* CONTROLSWIDGET_H */
