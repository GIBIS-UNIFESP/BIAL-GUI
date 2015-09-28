#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"

#include <QApplication>
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
  bool loadDicomdir( QString dicomFName );

signals:
  void updateProgress( int p );

private slots:
  void on_actionRed_background_triggered( );
  void on_actionGreen_background_triggered( );
  void on_actionBlue_background_triggered( );
  void on_actionBlack_background_triggered( );
  void on_actionWhite_background_triggered( );
  void on_actionOpen_image_triggered( );
  void on_actionQuit_triggered( );

  void currentImageChanged( );
  void imageUpdated( );
  void containerUpdated( );
  void readSettings( );
  void openRecentFile( );
  void updateRecentFileActions( );

  void on_actionAddLabel_triggered( );
  void on_actionOpen_folder_triggered( );
  void on_actionOpen_DicomDir_triggered( );
  void on_actionAdd_image_triggered( );
  void on_actionRemove_current_image_triggered( );
  void on_actionSelect_default_folder_triggered( );
  void on_actionRemove_current_label_triggered( );

  void mouseMoved( QPointF scnPos, size_t axis );

  void on_actionAxial_triggered( );
  void on_actionCoronal_triggered( );
  void on_actionSagittal_triggered( );
  void on_action3_Views_triggered( );
  void on_action4_Views_triggered( );
  void on_actionVertical_triggered( );
  void on_actionHorizontal_triggered( );
  void on_actionGrid_triggered( );

  void on_actionWhitePen_triggered( );
  void on_actionRedPen_triggered( );
  void on_actionBluePen_triggered( );
  void on_actionGreenPen_triggered( );
  void on_actionBlackPen_triggered( );


private:
  Ui::MainWindow *ui;
  Controller *controller;
  QString defaultFolder;
  QAction *recentFileActs[ Controller::MaxRecentFiles ];
};

class CursorChanger {
public:
  CursorChanger( const QCursor &cursor ) {
    QApplication::setOverrideCursor( cursor );
  }
  ~CursorChanger( ) {
    QApplication::restoreOverrideCursor( );
  }
};


#endif /* MAINWINDOW_H */
