#include "thumbswidget.h"
#include "ui_thumbswidget.h"

ThumbsWidget::ThumbsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ThumbsWidget ) {
  ui->setupUi( this );
}

ThumbsWidget::~ThumbsWidget( ) {
  delete ui;
}
