#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
  ui->setupUi( this );

  ui->logoView->hide( );

  ui->controlsWidget->installImageViewer(ui->imageViewer);

  connect( ui->actionGrid, &QAction::triggered, ui->imageViewer, &ImageViewer::setGridLayout );
  connect( ui->actionHorizontal, &QAction::triggered, ui->imageViewer, &ImageViewer::setHorizontalLayout );
  connect( ui->actionVertical, &QAction::triggered, ui->imageViewer, &ImageViewer::setVerticalLayout );

  connect( ui->actionAxial, &QAction::triggered, ui->imageViewer, &ImageViewer::setView0 );
  connect( ui->actionCoronal, &QAction::triggered, ui->imageViewer, &ImageViewer::setView1 );
  connect( ui->actionSagittal, &QAction::triggered, ui->imageViewer, &ImageViewer::setView2 );

  connect( ui->action3_Views, &QAction::triggered, ui->imageViewer, &ImageViewer::set3Views );
  connect( ui->action4_Views, &QAction::triggered, ui->imageViewer, &ImageViewer::set4Views );
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
