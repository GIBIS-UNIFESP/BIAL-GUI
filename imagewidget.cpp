#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ImageWidget ) {
  ui->setupUi( this );
  m_scene = new QGraphicsScene( this );
  ui->graphicsView->setScene( m_scene );
  setBackgroundColor( Qt::black );
}

ImageWidget::~ImageWidget( ) {
  delete ui;
}

void ImageWidget::hideControls( ) {
  ui->spinBox->hide( );
  ui->horizontalSlider->hide( );
  ui->rotateButton->hide( );
}

void ImageWidget::showControls( ) {
  ui->spinBox->show( );
  ui->horizontalSlider->show( );
  ui->rotateButton->show( );
}

void ImageWidget::setBackgroundColor( const QColor &color ) {
  if( ui->graphicsView->scene( ) ) {
    ui->graphicsView->scene( )->setBackgroundBrush( color );
  }
}

QGraphicsScene * ImageWidget::scene( ) const {
  return( m_scene );
}
