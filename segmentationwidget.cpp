#include "segmentationwidget.h"
#include "ui_segmentationwidget.h"

Segmentationwidget::Segmentationwidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Segmentationwidget)
{
  ui->setupUi(this);
}

Segmentationwidget::~Segmentationwidget()
{
  delete ui;
}

void Segmentationwidget::on_SegmentationButton_clicked()
{

}
