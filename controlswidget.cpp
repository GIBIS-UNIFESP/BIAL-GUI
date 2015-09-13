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

  //Layout

  connect( ui->pushButtonGrid, &QPushButton::clicked, viewer, &ImageViewer::setGridLayout );
  connect( ui->pushButtonHorizontal, &QPushButton::clicked, viewer, &ImageViewer::setHorizontalLayout );
  connect( ui->pushButtonVertical, &QPushButton::clicked, viewer, &ImageViewer::setVerticalLayout );

  //NIfTI

  connect( ui->pushButtonAxial, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButtonCoronal, &QPushButton::clicked, viewer, &ImageViewer::setView1 );
  connect( ui->pushButtonSagittal, &QPushButton::clicked, viewer, &ImageViewer::setView2 );

  connect( ui->pushButton1View, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton3Views, &QPushButton::clicked, viewer, &ImageViewer::set3Views );
  connect( ui->pushButton4Views, &QPushButton::clicked, viewer, &ImageViewer::set4Views );

  connect( ui->pushButton1View, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::show );
  connect( ui->pushButton3Views, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::hide );
  connect( ui->pushButton4Views, &QPushButton::clicked, ui->groupBoxNiftiAxis, &QGroupBox::hide );

  connect( ui->pushButton1View, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::hide );
  connect( ui->pushButton3Views, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show);
  connect( ui->pushButton4Views, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );

  //PPM

  connect( ui->pushButton_1RGB, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, viewer, &ImageViewer::set3Views );
  connect( ui->pushButton_4RGB, &QPushButton::clicked, viewer, &ImageViewer::set4Views );

  connect( ui->pushButton_1RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::show );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::hide );
  connect( ui->pushButton_4RGB, &QPushButton::clicked, ui->groupBoxPpmChannels, &QGroupBox::hide );

  connect( ui->pushButton_1RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::hide );
  connect( ui->pushButton_3RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show);
  connect( ui->pushButton_4RGB, &QPushButton::clicked, ui->groupBoxOrientation, &QGroupBox::show );

  connect( ui->pushButton_RGB, &QPushButton::clicked, viewer, &ImageViewer::setView0 );
  connect( ui->pushButton_R, &QPushButton::clicked, viewer, &ImageViewer::setView1 );
  connect( ui->pushButton_G, &QPushButton::clicked, viewer, &ImageViewer::setView2 );
  connect( ui->pushButton_B, &QPushButton::clicked, viewer, &ImageViewer::setView3 );
}
