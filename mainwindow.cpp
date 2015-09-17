#include "controller.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ),
  controller( new Controller( 4, this ) ) {

  ui->setupUi( this );

  controller->setThumbsWidget( ui->thumbsWidget );

  ui->controlsWidget->setController( controller );

  ui->controlsWidget->installImageViewer( ui->imageViewer );

  ui->controlsDock->hide();

  ui->imageViewer->setController(controller);

  ui->actionPrint->setEnabled( false );

  setupLogoview( );

  createConnections( );

  updateMenus( );

  readSettings( );

  createActions( );

  loadQss( );

}

void MainWindow::createConnections( ) {
  /* Layout */
  connect( ui->actionGrid, &QAction::triggered, ui->imageViewer, &ImageViewer::setGridLayout );
  connect( ui->actionHorizontal, &QAction::triggered, ui->imageViewer, &ImageViewer::setHorizontalLayout );
  connect( ui->actionVertical, &QAction::triggered, ui->imageViewer, &ImageViewer::setVerticalLayout );
  connect( ui->actionAxial, &QAction::triggered, ui->imageViewer, &ImageViewer::setView0 );
  connect( ui->actionCoronal, &QAction::triggered, ui->imageViewer, &ImageViewer::setView1 );
  connect( ui->actionSagittal, &QAction::triggered, ui->imageViewer, &ImageViewer::setView2 );

  /* PPM */
  connect( ui->actionAll_channels, &QAction::triggered, ui->imageViewer, &ImageViewer::setView0 );
  connect( ui->actionRed_channel, &QAction::triggered, ui->imageViewer, &ImageViewer::setView1 );
  connect( ui->actionGreen_channel, &QAction::triggered, ui->imageViewer, &ImageViewer::setView2 );
  connect( ui->actionBlue_channel, &QAction::triggered, ui->imageViewer, &ImageViewer::setView3 );
  connect( ui->action3_Views, &QAction::triggered, ui->imageViewer, &ImageViewer::setViews123 );
  connect( ui->action4_Views, &QAction::triggered, ui->imageViewer, &ImageViewer::setViews0123 );

  /* Show/Hide docks. */
  connect( ui->actionShow_controls_dock, &QAction::toggled, ui->controlsDock, &QDockWidget::setVisible );
  connect( ui->actionShow_images_dock, &QAction::toggled, ui->thumbsDock, &QDockWidget::setVisible );
  connect( ui->controlsDock, &QDockWidget::visibilityChanged, ui->actionShow_controls_dock, &QAction::setChecked );
  connect( ui->thumbsDock, &QDockWidget::visibilityChanged, ui->actionShow_images_dock, &QAction::setChecked );

  /* Dynamic functions. */
  connect( ui->imageViewer, &ImageViewer::updateStatus, ui->statusBar, &QStatusBar::showMessage );
  connect( controller, &Controller::currentImageChanged, this, &MainWindow::updateMenus );
  connect( controller, &Controller::containerUpdated, this, &MainWindow::updateMenus );
  connect( controller, &Controller::recentFilesUpdated,this,&MainWindow::updateRecentFileActions);
}

void MainWindow::setupLogoview( ) {
  QGraphicsScene *scn = new QGraphicsScene( this );
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem( QPixmap( ":/icons/logo_shadow.png" ) );
  scn->addItem( pixmapItem );
  scn->setSceneRect( 0, 0, pixmapItem->pixmap( ).width( ), pixmapItem->pixmap( ).height( ) );
  ui->logoView->setScene( scn );
}

MainWindow::~MainWindow( ) {
  delete ui;
}

void MainWindow::on_actionRed_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::red );
}

void MainWindow::on_actionGreen_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::green );
}

void MainWindow::on_actionBlue_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::blue );
}

void MainWindow::on_actionBlack_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::black );
}

void MainWindow::on_actionWhite_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::white );
}

void MainWindow::updateMenus( ) {
  COMMENT( "Updating menus, current image = " << controller->currentImagePos( ), 0 );

  /* Verifying if an Image is present. */
  bool hasImage = ( controller->currentImage( ) != nullptr );

  if(controller->size() <= 1 ) {
    ui->thumbsDock->hide();
//    ui->controlsDock->hide();
  } else {
    ui->thumbsDock->show();
//    ui->controlsDock->show();
  }

  ui->logoView->setVisible( !hasImage );
  ui->imageViewer->setVisible( hasImage );
//  ui->controlsDock->setVisible( hasImage );
//  ui->thumbsDock->setVisible( hasImage );
  ui->menuLayout->setEnabled( hasImage );
  ui->menuOverlay->setEnabled( hasImage );
  ui->actionRemove_current_image->setEnabled( hasImage );
  ui->actionAddLabel->setEnabled( hasImage );
  if( !hasImage ) {
    ui->actionRemove_current_label->setEnabled( false );
  }
  /* TODO : Verify image type and update layout menu. */
}

void MainWindow::on_actionOpen_image_triggered( ) {

  QString fileName = getFileDialog( );
  if( fileName.isEmpty( ) ) {
    return;
  }
  if( !QFile( fileName ).exists( ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open file!\nFile does not exist." ) );
    return;
  }
  if( !loadFile( fileName ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open file!" ) );
    return;
  }
}


QString MainWindow::getFileDialog( ) {
  return( QFileDialog::getOpenFileName( this, tr( "Open" ), defaultFolder,
                                        tr( "All images (*.pbm *.pbm.gz *.pgm *.pgm.gz *.ppm *.ppm.gz *.dcm "
                                            "*.dcm.gz *.nii *.nii.gz);; PBM images (*.pbm *.pbm.gz);; PGM images "
                                            "(*.pgm *.pgm.gz);; PPM images (*.ppm *.ppm.gz);; DICOM images "
                                            "(*.dcm *.dcm.gz);; NIfTI images (*.nii *.nii.gz);; All files (*)" ) ) );
}

bool MainWindow::loadFile( QString filename ) {
  COMMENT( "Loading file: " << filename.toStdString( ), 0 );
  controller->clear( );
  return( controller->addImage( filename ) );
}

bool MainWindow::loadFolder( QString dirname ) {
  QDir folder( dirname );
  COMMENT( "Reding folder: " << folder.absolutePath( ).toStdString( ) << ".", 1 );
  QFileInfoList list = folder.entryInfoList( QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst | QDir::Name );
  bool value = false;
  /*  qDebug() << "list size: " << list.size(); */

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  int size = size = list.size( );

  for( int i = 0; i < size; ++i ) {
    progress.setValue( i );
    if( progress.wasCanceled( ) ) {
      break;
    }
    QFileInfo fileInfo = list.at( i );
    if( fileInfo.isFile( ) ) {
      QString fileName = fileInfo.absoluteFilePath( );
      if( controller->addImage( fileName ) ) {
        value = true;
      } else {
        BIAL_WARNING( std::string( "Could not open file!" ) );
        statusBar( )->showMessage( tr( "Could not open file!" ), 2000 );
        continue;
      }
    }
  }
  progress.setValue( list.size( ) );
  return( value );
}

void MainWindow::setDefaultFolder( ) {
  QDir dir;
  if( !dir.exists( QDir::homePath( ) + "/.bial/" ) ) {
    dir.mkdir( QDir::homePath( ) + "/.bial/" );
  }
  QString temp = QFileDialog::getExistingDirectory( this, tr( "Select default folder" ) );
  if( !temp.isEmpty( ) ) {
    defaultFolder = temp;
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "defaultFolder", defaultFolder );
    settings.endGroup( );
  }
}

void MainWindow::readSettings( ) {
  COMMENT( "Reading QSettings", 1 );
  QSettings settings;

  settings.beginGroup( "MainWindow" );
  defaultFolder = settings.value( "defaultFolder" ).toString( );
  if( defaultFolder.isEmpty( ) ) {
    defaultFolder = QDir::homePath( );
  }
}

void MainWindow::commandLineOpen( int argc, char *argv[] ) {
  COMMENT( "Command Line Open with " << argc << " arguments:", 0 );
  if( ( argc == 3 ) && ( QString( argv[ 1 ] ) == "-d" ) ) {
    /*    LoadDicomdir(QString(argv[2])); */
  } else {
    QFileInfo file;

    for( int img = 1; img < argc; ++img ) {
      QString fileName( argv[ img ] );
      file.setFile( fileName );
      COMMENT( "\tArgument[" << img << "] = " << fileName.toStdString( ), 0 );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          controller->addImage( file.absoluteFilePath( ) );
        } else if( file.isDir( ) ) {
          loadFolder( file.absoluteFilePath( ) );
        }
      } else {
        BIAL_WARNING( "FILE DOES NOT EXISTS! : " << file.absolutePath( ).toStdString( ) );
      }
    }
  }
}

void MainWindow::on_actionQuit_triggered( ) {
  close( );
}


void MainWindow::openRecentFile( ) {
  QAction *action = qobject_cast< QAction* >( sender( ) );
  if( action ) {
    QString fileName = action->data( ).toString( );
    loadFile( fileName );
  }
}

void MainWindow::updateRecentFileActions( ) {
  QSettings settings;
  QStringList files = settings.value( "recentFileList" ).toStringList( );

  int numRecentFiles = qMin( files.size( ), ( int ) Controller::MaxRecentFiles );
  if( numRecentFiles > 0 ) {
    ui->menuRecent_files->setEnabled( true );
  }
  for( int i = 0; i < numRecentFiles; ++i ) {
    QString text = QString( "&%1 %2" ).arg( i + 1 ).arg( QFileInfo( files[ i ] ).fileName( ) );
    recentFileActs[ i ]->setText( text );
    recentFileActs[ i ]->setData( files[ i ] );
    recentFileActs[ i ]->setVisible( true );
  }
  for( int i = numRecentFiles; i < Controller::MaxRecentFiles; ++i ) {
    recentFileActs[ i ]->setVisible( false );
  }
}

void MainWindow::createActions( ) {
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    recentFileActs[ i ] = new QAction( this );
    recentFileActs[ i ]->setVisible( false );
    connect( recentFileActs[ i ], &QAction::triggered, this, &MainWindow::openRecentFile );
    ui->menuRecent_files->addAction( recentFileActs[ i ] );
  }
  updateRecentFileActions( );
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    ui->menuRecent_files->addAction( recentFileActs[ i ] );
  }
}

void MainWindow::loadQss( ) {
  QFile file( ":/qss/stylesheet.qss" );
  file.open( QFile::ReadOnly );
  QString StyleSheet = QLatin1String( file.readAll( ) );
  setStyleSheet( StyleSheet );
}
