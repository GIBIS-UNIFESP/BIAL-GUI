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

private slots:
  void imageChanged( );
  void imageUpdated( );
  void updateRange( );

  void on_spinBoxSpeed_valueChanged(int arg1);

  void on_buttonPlay_clicked(bool checked);

private:
  Ui::ControlsWidget *ui;
  Controller *controller;
  QTimer * timer;
};

#endif /* CONTROLSWIDGET_H */
