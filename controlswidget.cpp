#include "controller.h"
#include "controlswidget.h"
#include "imageviewer.h"
#include "ui_controlswidget.h"

ControlsWidget::ControlsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ControlsWidget ) {
  ui->setupUi( this );
}

ControlsWidget::~ControlsWidget( ) {
  delete ui;
}

void ControlsWidget::installImageViewer( ImageViewer *viewer ) {

  /* Layout */

  connect( ui->pushButtonGrid, &QPushButton::clicked, viewer, &ImageViewer::setGridLayout );
  connect( ui->pushButtonHorizontal, &QPushButton::clicked, viewer, &ImageViewer::setHorizontalLayout );
  connect( ui->pushButtonVertical, &QPushButton::clicked, viewer, &ImageViewer::setVerticalLayout );

  /* NIfTI */

  connect( ui->pushButtonAxial, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButtonCoronal, &QPushButton::clicked, viewer, &ImageViewer::setView1 );
  connect( ui->pushButtonSagittal, &QPushButton::clicked, viewer, &ImageViewer::setView2 );

  connect( ui->pushButton1View, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton3Views, &QPushButton::clicked, viewer, &ImageViewer::setViews012 );
  connect( ui->pushButton4Views, &QPushButton::clicked, viewer, &ImageViewer::set0123Views );

  connect( ui->pushButton1View, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::show );
  connect( ui->pushButton3Views, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::hide );
  connect( ui->pushButton4Views, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::hide );

  connect( ui->pushButton1View, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::hide );
  connect( ui->pushButton3Views, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );
  connect( ui->pushButton4Views, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );

  /* PPM */

  connect( ui->pushButton_1RGB, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, viewer, &ImageViewer::setViews123 );
  connect( ui->pushButton_4RGB, &QPushButton::clicked, viewer, &ImageViewer::set0123Views );

  connect( ui->pushButton_1RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::show );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::hide );
  connect( ui->pushButton_4RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::hide );

  connect( ui->pushButton_1RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::hide );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );
  connect( ui->pushButton_4RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );

  connect( ui->pushButton_RGB, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton_R, &QPushButton::clicked, viewer, &ImageViewer::setView1 );
  connect( ui->pushButton_G, &QPushButton::clicked, viewer, &ImageViewer::setView2 );
  connect( ui->pushButton_B, &QPushButton::clicked, viewer, &ImageViewer::setView3 );
}

void ControlsWidget::setController( Controller *value ) {
  controller = value;
  connect( controller, &Controller::imageChanged, this, &ControlsWidget::imageChanged );
  connect( controller, &Controller::containerUpdated, this, &ControlsWidget::updateRange );
}

void ControlsWidget::imageChanged( ) {
  GuiImage *img = controller->currentImage( );

  updateRange( );

  ui->folderHorizontalSlider->setValue( controller->currentImagePos( ) );
  ui->folderSpinBox->setValue( controller->currentImagePos( ) );
  if( img->modality( ) == Modality::NIfTI ) {
    ui->groupBoxNiftiViews->show( );
    ui->groupBoxNiftiAxis->hide( );
    ui->groupBoxOrientation->show( );
    ui->groupBoxPpmViews->hide( );
    ui->groupBoxPpmChannels->hide( );
    ui->pushButton3Views->click( );
  }
  else if( img->modality( ) == Modality::RGB ) {

  }
  else if( img->modality( ) == Modality::BW ) {
    ui->groupBoxNiftiViews->hide( );
    ui->groupBoxNiftiAxis->hide( );
    ui->groupBoxOrientation->hide( );
    ui->groupBoxPpmViews->hide( );
    ui->groupBoxPpmChannels->hide( );
  }
  ui->groupBoxLabels->setVisible( img->hasLabels( ) );
}

void ControlsWidget::updateRange( ) {
  size_t images = controller->size( );
  if( images > 1 ) {
    ui->rotateAll->show( );
    ui->rotate->hide( );
    ui->folderSpinBox->setEnabled( true );
    ui->folderSpinBox->setMaximum( images );
    ui->folderHorizontalSlider->setEnabled( true );
    ui->folderHorizontalSlider->setMaximum( images );
    ui->folderHorizontalSlider->setMinimum( 0 );
  }
  else if( images == 0 ) {
    /*    controller->setCurrentImagePos(images - 1); */
    ui->folderSpinBox->setEnabled( false );
    ui->folderSpinBox->setMaximum( 0 );
    ui->folderHorizontalSlider->setEnabled( false );
    ui->folderHorizontalSlider->setMaximum( 0 );
    ui->folderHorizontalSlider->setMinimum( 0 );
  }
}
