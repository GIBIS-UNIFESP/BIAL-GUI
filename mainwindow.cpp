#include "controller.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsPixmapItem>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ),
  controller( new Controller ) {
  ui->setupUi( this );

  ui->controlsWidget->installImageViewer( ui->imageViewer );

  setupLogoview( );

  createConnections( );

  updateMenus( );

  ui->actionPrint->setEnabled( false );
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
  connect( ui->action4_Views, &QAction::triggered, ui->imageViewer, &ImageViewer::set0123Views );

  /* Show/Hide docks */
  connect( ui->actionShow_controls_dock, &QAction::toggled, ui->controlsDock, &QDockWidget::setVisible );
  connect( ui->actionShow_images_dock, &QAction::toggled, ui->thumbsDock, &QDockWidget::setVisible );
  connect( ui->controlsDock, &QDockWidget::visibilityChanged, ui->actionShow_controls_dock, &QAction::setChecked );
  connect( ui->thumbsDock, &QDockWidget::visibilityChanged, ui->actionShow_images_dock, &QAction::setChecked );

  /* Dynamic functions */
  connect( ui->imageViewer, &ImageViewer::updateStatus, ui->statusBar, &QStatusBar::showMessage );
  connect( controller, &Controller::imageChanged, this, &MainWindow::updateMenus );
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
  ui->imageViewer->setBackgroundColor( Qt::red );
}

void MainWindow::on_actionGreen_background_triggered( ) {
  ui->imageViewer->setBackgroundColor( Qt::green );
}

void MainWindow::on_actionBlue_background_triggered( ) {
  ui->imageViewer->setBackgroundColor( Qt::blue );
}

void MainWindow::on_actionBlack_background_triggered( ) {
  ui->imageViewer->setBackgroundColor( Qt::black );
}

void MainWindow::on_actionWhite_background_triggered( ) {
  ui->imageViewer->setBackgroundColor( Qt::white );
}

void MainWindow::updateMenus( ) {
  //Verifying if an Image is present.
  bool hasImage = ( controller->CurrentImage( ) != nullptr );
  ui->logoView->setVisible( !hasImage );
  ui->imageViewer->setVisible( hasImage );
  ui->controlsDock->setVisible( hasImage );
  ui->thumbsDock->setVisible( hasImage );
  ui->menuLayout->setEnabled( hasImage );
  ui->menuOverlay->setEnabled( hasImage );
  ui->actionRemove_current_image->setEnabled( hasImage );
  ui->actionAddLabel->setEnabled( hasImage );
  if( !hasImage ) {
    ui->actionRemove_current_label->setEnabled( false );
  }

  /* TODO : Verify image type and update layout menu. */
}
