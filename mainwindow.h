#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow( QWidget *parent = 0 );
  ~MainWindow( );

  void commandLineOpen( int argc, char *argv[] );

private:
  void createConnections( );
  void setupLogoview( );
  QString getFileDialog( );
  bool loadFile( QString filename );
  bool loadFolder( QString dirname );
  void createActions( );
  void loadQss( );

private slots:
  void on_actionRed_background_triggered( );
  void on_actionGreen_background_triggered( );
  void on_actionBlue_background_triggered( );
  void on_actionBlack_background_triggered( );
  void on_actionWhite_background_triggered( );
  void on_actionOpen_image_triggered( );
  void on_actionQuit_triggered( );

  void currentImageChanged( );
  void containerUpdated( );
  void readSettings( );
  void openRecentFile( );
  void updateRecentFileActions( );


  void on_actionAddLabel_triggered( );
  void on_actionOpen_folder_triggered( );
  void on_actionAdd_image_triggered( );
  void on_actionRemove_current_image_triggered( );
  void on_actionSelect_default_folder_triggered( );
  void on_actionRemove_current_label_triggered( );

private:
  Ui::MainWindow *ui;
  Controller *controller;
  QString defaultFolder;
  QAction *recentFileActs[ Controller::MaxRecentFiles ];
};

#endif /* MAINWINDOW_H */
