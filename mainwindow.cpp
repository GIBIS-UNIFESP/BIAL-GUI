#include "controller.h"
#include "dicomdir.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfoList>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ),
  controller( new Controller( 4, this ) ) {

  ui->setupUi( this );

  controller->setThumbsWidget( ui->thumbsWidget );

  ui->controlsWidget->setController( controller );
  ui->controlsDock->hide( );
  ui->imageViewer->setController( controller );
  ui->actionPrint->setEnabled( false );

  setupLogoview( );
  createConnections( );
  currentImageChanged( );
  readSettings( );
  createActions( );
  loadQss( );
  containerUpdated( );

#ifndef LIBGDCM
  ui->actionOpen_DicomDir->setVisible( false );
#endif
}

void MainWindow::createConnections( ) {
  /* Show/Hide docks. */
  connect( ui->actionShow_controls_dock, &QAction::toggled, ui->controlsDock, &QDockWidget::setVisible );
  connect( ui->actionHistogram_dock, &QAction::toggled, ui->dockWidgetHistogram, &QDockWidget::setVisible );
  connect( ui->actionShow_images_dock, &QAction::toggled, ui->thumbsDock, &QDockWidget::setVisible );
  connect( ui->controlsDock, &QDockWidget::visibilityChanged, ui->actionShow_controls_dock, &QAction::setChecked );
  connect( ui->thumbsDock, &QDockWidget::visibilityChanged, ui->actionShow_images_dock, &QAction::setChecked );
  connect( ui->dockWidgetHistogram, &QDockWidget::visibilityChanged , ui->actionHistogram_dock, &QAction::setChecked);

  /* Controller. */
  connect( controller, &Controller::currentImageChanged, this, &MainWindow::currentImageChanged );
  connect( controller, &Controller::imageUpdated, this, &MainWindow::imageUpdated );
  connect( controller, &Controller::containerUpdated, this, &MainWindow::containerUpdated );
  connect( controller, &Controller::recentFilesUpdated, this, &MainWindow::updateRecentFileActions );

  /* ImageViewer */
  connect( ui->imageViewer, &ImageViewer::mouseMoved, this, &MainWindow::mouseMoved );

  /* Overlay */
  connect( ui->actionToggle_overlay, &QAction::triggered, ui->imageViewer, &ImageViewer::toggleOverlay );
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

void MainWindow::currentImageChanged( ) {
  if( controller->currentImage( ) ) {
    DisplayFormat *format = controller->currentFormat( );

    ui->menuLayout->setEnabled( format->modality( ) != Modality::BW );
    ui->menuOverlay->setEnabled( format->hasOverlay( ) );

    ui->actionGrid->setVisible( format->showOrientation( ) );
    ui->actionHorizontal->setVisible( format->showOrientation( ) );
    ui->actionVertical->setVisible( format->showOrientation( ) );

    ui->actionAxial->setVisible( format->modality( ) == Modality::NIfTI );
    ui->actionCoronal->setVisible( format->modality( ) == Modality::NIfTI );
    ui->actionSagittal->setVisible( format->modality( ) == Modality::NIfTI );

    ui->actionRed_channel->setVisible( format->modality( ) == Modality::RGB );
    ui->actionGreen_channel->setVisible( format->modality( ) == Modality::RGB );
    ui->actionBlue_channel->setVisible( format->modality( ) == Modality::RGB );
    ui->actionAll_channels->setVisible( format->modality( ) == Modality::RGB );

    ui->action3_Views->setVisible( format->has3Views( ) );
    ui->action4_Views->setVisible( format->has4Views( ) );
  }
}

void MainWindow::imageUpdated( ) {
  const Bial::Signal &hist = controller->currentImage( )->getHistogram( );
  QCustomPlot *plot = ui->histogramWidget;
  QVector< double > x( hist.size( ) ), y( hist.size( ) );
  for( size_t bin = 0; bin < hist.size( ); ++bin ) {
    x[ bin ] = hist.Data( bin );
    y[ bin ] = hist[ bin ];
  }
  plot->addGraph( );
  plot->graph( 0 )->setData( x, y );
  plot->xAxis->setLabel( "Intensity" );
  plot->yAxis->setLabel( "Frequency" );
  plot->rescaleAxes( true );
  plot->replot( );
}

void MainWindow::containerUpdated( ) {
  COMMENT( "MainWindow::containerUpdated( )", 0 );
  if( controller->size( ) <= 1 ) {
    ui->thumbsDock->hide( );
  }
  else {
    ui->thumbsDock->show( );
  }
  bool hasImage = ( controller->currentImage( ) != nullptr );
  COMMENT( "Has Image = " << hasImage, 0 );
  ui->menuWindow->setEnabled( hasImage );
  ui->controlsDock->setVisible( hasImage );
  ui->logoView->setVisible( !hasImage );
  ui->imageViewer->setVisible( hasImage );
  ui->menuLayout->setEnabled( hasImage );
  ui->menuOverlay->setEnabled( hasImage );
  ui->actionRemove_current_image->setEnabled( hasImage );
  ui->actionAddLabel->setEnabled( hasImage );
  if( !hasImage ) {
    ui->actionRemove_current_label->setEnabled( false );
    ui->dockWidgetHistogram->hide( );
  }
  ui->actionShow_controls_dock->setEnabled( hasImage );
  ui->actionShow_images_dock->setEnabled( hasImage );
  currentImageChanged( );
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
                                            "*.dcm.gz *.nii *.nii.gz *.scn *.scn.gz);; PBM images (*.pbm *.pbm.gz);; PGM images "
                                            "(*.pgm *.pgm.gz);; PPM images (*.ppm *.ppm.gz);; DICOM images "
                                            "(*.dcm *.dcm.gz);; NIfTI images (*.nii *.nii.gz);; SCN Files (*.scn *.scn.gz);; All files (*)" ) ) );
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
  CursorChanger c( Qt::WaitCursor );

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  int size = list.size( );
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
      }
      else {
        BIAL_WARNING( std::string( "Could not open file!" ) );
        statusBar( )->showMessage( tr( "Could not open file!" ), 2000 );
        continue;
      }
    }
  }
  progress.setValue( list.size( ) );

  return( value );
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
    loadDicomdir( QString( argv[ 2 ] ) );
  }
  else {
    QFileInfo file;
    for( int img = 1; img < argc; ++img ) {
      QString fileName( argv[ img ] );
      file.setFile( fileName );
      COMMENT( "\tArgument[" << img << "] = " << fileName.toStdString( ), 0 );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          controller->addImage( file.absoluteFilePath( ) );
        }
        else if( file.isDir( ) ) {
          loadFolder( file.absoluteFilePath( ) );
        }
      }
      else {
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

bool MainWindow::loadDicomdir( QString dicomFName ) {
  COMMENT( "Loading DicomDir file", 1 );
  CursorChanger c( Qt::WaitCursor );
  DicomDir dicomdir;
  if( !dicomdir.open( dicomFName ) ) {
    statusBar( )->showMessage( tr( "Could not open dicomdir" ), 2000 );
    return( false );
  }
  const QStringList files = dicomdir.getImages( );
  if( files.size( ) > 0 ) {
    controller->clear( );
    QProgressDialog progress( tr( "Reading dicomdir files..." ), tr( "Abort" ), 0, files.size( ), this );
    progress.setWindowModality( Qt::WindowModal );
    for( int i = 0, size = files.size( ); i < size; ++i ) {
      progress.setValue( i );
      if( progress.wasCanceled( ) ) {
        break;
      }
      controller->addImage( files[ i ].trimmed( ) );
    }
    progress.setValue( files.size( ) );
    if( controller->size( ) < 1 ) {
      statusBar( )->showMessage( tr( "Could not load any dicomdir images" ), 2000 );
      return( false );
    }
    return( true );
  }
  statusBar( )->showMessage( tr( "Empty dicomdir!" ), 2000 );
  BIAL_WARNING( "Empty dicomdir!" );

  return( false );
}

void MainWindow::on_actionAddLabel_triggered( ) {
  controller->addLabel( getFileDialog( ) );
}

void MainWindow::on_actionOpen_folder_triggered( ) {
  QString folderString = QFileDialog::getExistingDirectory( this, tr( "Open folder" ), defaultFolder );
  COMMENT( "Opening folder: \"" << folderString.toStdString( ) << "\"", 1 )
  if( !folderString.isEmpty( ) ) {
    controller->clear( );
    if( !loadFolder( folderString ) ) {
      BIAL_WARNING( "Could not read folder!" )
      QMessageBox::warning( this, "Warning", tr( "Could not read folder!" ) );
    }
  }
}

void MainWindow::on_actionOpen_DicomDir_triggered( ) {
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Open" ), defaultFolder,
                                                   tr( "*" ) );
  loadDicomdir( fileName );
}

void MainWindow::on_actionAdd_image_triggered( ) {
  controller->addImage( getFileDialog( ) );
}

void MainWindow::on_actionRemove_current_image_triggered( ) {
  controller->removeCurrentImage( );
}

void MainWindow::on_actionSelect_default_folder_triggered( ) {
  QString temp = QFileDialog::getExistingDirectory( this, tr( "Select default folder" ), QDir::homePath( ) );
  if( !temp.isEmpty( ) ) {
    defaultFolder = temp;
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "defaultFolder", defaultFolder );
    settings.endGroup( );
  }
}

void MainWindow::on_actionRemove_current_label_triggered( ) {
  controller->removeCurrentLabel( );
}

void MainWindow::mouseMoved( QPointF scnPos, size_t axis ) {
  GuiImage *img = controller->currentImage( );
  if( img != nullptr ) {
    Bial::Point3D pt = img->getPosition( scnPos, axis );
    QString msg;
    int max = img->max( );
    if( img->modality( ) == Modality::NIfTI ) {
      if( img->getImage( ).ValidPixel( pt.x, pt.y, pt.z ) ) {
        int color = img->getImage( ).at( pt.x, pt.y, pt.z );
        msg = QString( "Axis %1 : (%2, %3, %4) = %5/%6" ).arg( axis ).arg( ( int ) pt.x ).arg(
          ( int ) pt.y ).arg( ( int ) pt.z ).arg( color ).arg( max );
      }
    }
    else if( img->modality( ) == Modality::BW ) {
      if( img->getImage( ).ValidPixel( pt.x, pt.y ) ) {
        int color = img->getImage( ).at( pt.x, pt.y );
        msg = QString( "(%1, %2) = %3/%4" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( color ).arg( max );
      }
    }
    else if( img->modality( ) == Modality::RGB ) {
      if( img->getImage( ).ValidPixel( pt.x, pt.y ) ) {
        int r = img->getImage( ).at( pt.x, pt.y, 0 );
        int g = img->getImage( ).at( pt.x, pt.y, 1 );
        int b = img->getImage( ).at( pt.x, pt.y, 2 );
        msg =
          QString( "(%1, %2) = (%3, %4, %5)/%6" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( r ).arg( g ).arg( b ).arg(
            max );
      }
    }
    ui->statusBar->showMessage( msg, 10000 );
  }
}

void MainWindow::on_actionAxial_triggered( ) {
  controller->currentFormat( )->setNumberOfViews( 1 );
  controller->currentFormat( )->setCurrentViews( Views::SHOW0 );
}

void MainWindow::on_actionCoronal_triggered( ) {
  controller->currentFormat( )->setNumberOfViews( 1 );
  controller->currentFormat( )->setCurrentViews( Views::SHOW1 );
}

void MainWindow::on_actionSagittal_triggered( ) {
  controller->currentFormat( )->setNumberOfViews( 1 );
  controller->currentFormat( )->setCurrentViews( Views::SHOW2 );
}

void MainWindow::on_action3_Views_triggered( ) {
  controller->currentFormat( )->setNumberOfViews( 3 );
}

void MainWindow::on_action4_Views_triggered( ) {
  controller->currentFormat( )->setNumberOfViews( 4 );
}

void MainWindow::on_actionVertical_triggered( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::VERTICAL );
}

void MainWindow::on_actionHorizontal_triggered( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::HORIZONTAL );
}

void MainWindow::on_actionGrid_triggered( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::GRID );
}

void MainWindow::on_actionWhitePen_triggered( ) {
  controller->currentFormat( )->setOverlayColor( Qt::white );
}

void MainWindow::on_actionRedPen_triggered( ) {
  controller->currentFormat( )->setOverlayColor( Qt::red );
}

void MainWindow::on_actionBluePen_triggered( ) {
  controller->currentFormat( )->setOverlayColor( Qt::blue );
}

void MainWindow::on_actionGreenPen_triggered( ) {
  controller->currentFormat( )->setOverlayColor( Qt::green );
}

void MainWindow::on_actionBlackPen_triggered( ) {
  controller->currentFormat( )->setOverlayColor( Qt::black );
}
