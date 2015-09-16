#include "imagewidget.h"
#include "ui_imagewidget.h"
#include <Common.hpp>

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
  ui->frameControls->hide( );
}

void ImageWidget::showControls( ) {
  ui->frameControls->show( );
}

void ImageWidget::setBackgroundColor( const QColor &color ) {
  if( ui->graphicsView->scene( ) ) {
    ui->graphicsView->scene( )->setBackgroundBrush( color );
  }
}

QGraphicsScene* ImageWidget::scene( ) const {
  return( m_scene );
}

size_t ImageWidget::viewNumber( ) const {
  return( m_viewNumber );
}

void ImageWidget::setViewNumber( const size_t &viewNumber ) {
  m_viewNumber = viewNumber;
}

void ImageWidget::setRange( int start, int end ) {
  if( start > end ) {
    throw std::invalid_argument( BIAL_ERROR( "start should be smaller than end" ) );
  }
  ui->spinBox->setMinimum( start );
  ui->spinBox->setMaximum( end );

  ui->horizontalSlider->setMinimum( start );
  ui->horizontalSlider->setMaximum( end );

}

void ImageWidget::setSlice( int slice ) {
  if( ui->spinBox->value( ) != slice ) {
    ui->spinBox->setValue( slice );
  }
  if( ui->horizontalSlider->value( ) != slice ) {
    ui->horizontalSlider->setValue( slice );
  }
}

void ImageWidget::on_spinBox_valueChanged( int position ) {
  emit sliceChanged( m_viewNumber, position );
}

void ImageWidget::on_rotateButton_clicked( ) {

}

void ImageWidget::on_horizontalSlider_valueChanged( int position ) {
  emit sliceChanged( m_viewNumber, position );

}

void ImageWidget::resizeEvent( QResizeEvent* ) {
  QGraphicsScene *scn = ui->graphicsView->scene();
  if(scn){
    ui->graphicsView->fitInView(scn->itemsBoundingRect() ,Qt::KeepAspectRatio);
  }
}
