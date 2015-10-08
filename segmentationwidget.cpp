#include "segmentationwidget.h"
#include "ui_segmentationwidget.h"

SegmentationWidget::SegmentationWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Segmentationwidget) {
  ui->setupUi(this);
}

SegmentationWidget::~SegmentationWidget() {
  delete ui;
}

void SegmentationWidget::on_SegmentationButton_clicked() {
//  double alpha = ui->AlphaSpinBox->value();
//  double beta = ui->BetaSpinBox->value();
}

void SegmentationWidget::on_eraserButton_clicked()
{
//    ui->drawButton->setChecked(false);
//    ui->mouseButton->setChecked(false);
}

void SegmentationWidget::on_drawButton_clicked()
{

}

void SegmentationWidget::on_mouseButton_clicked()
{

}

void SegmentationWidget::on_ClearButton_clicked()
{

}
