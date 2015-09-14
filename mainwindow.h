#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define MaxRecentFiles 10

namespace Ui {
  class MainWindow;
}

class Controller;

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
  void setRecentFile( QString fileName );
  void updateRecentFileActions( );
  void createActions( );
  void loadQss();

private slots:
  void on_actionRed_background_triggered( );
  void on_actionGreen_background_triggered( );
  void on_actionBlue_background_triggered( );
  void on_actionBlack_background_triggered( );
  void on_actionWhite_background_triggered( );

  void updateMenus( );
  void setDefaultFolder( );
  void readSettings( );
  void openRecentFile( );

  void on_actionOpen_image_triggered( );

  void on_actionQuit_triggered( );

private:
  Ui::MainWindow *ui;
  Controller *controller;
  QString defaultFolder;
  QAction *recentFileActs[ MaxRecentFiles ];
};

#endif /* MAINWINDOW_H */
