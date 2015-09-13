#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );

private slots:
  void on_actionRed_background_triggered();
  void on_actionGreen_background_triggered();
  void on_actionBlue_background_triggered();
  void on_actionBlack_background_triggered();
  void on_actionWhite_background_triggered();

private:
  Ui::MainWindow *ui;
};

#endif /* MAINWINDOW_H */
