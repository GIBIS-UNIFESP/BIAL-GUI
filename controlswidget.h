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
  void imageUpdated( );
  void updateRange( );

  void on_buttonPlay_clicked();

  void on_buttonStop_clicked();

private slots:
  void on_spinBoxSpeed_valueChanged(int arg1);

private:
  Ui::ControlsWidget *ui;
  Controller *controller;
  QTimer * timer;
};

#endif /* CONTROLSWIDGET_H */
