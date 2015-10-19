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

void SegmentationWidget::setTool(Tool *sTool)
{
    tool = dynamic_cast<SegmentationTool*>(sTool);
    if(tool){

        setEnabled(true);
        //atualiza os dados da interface
        //ui->AlphaSpinBox->setValue(tool->getAlpha());
//        switch ( tool->getMode() ) {
//        case 0:
//            ui->Button->setChecked(true);
//            break;
//        case 1:
//        case 2:
//            ui->drawButton->setChecked(true);
//            break;
//        }
    }
    else
        setEnabled(false);

}

void SegmentationWidget::on_SegmentationButton_clicked() {
//  double alpha = ui->AlphaSpinBox->value();
//  double beta = ui->BetaSpinBox->value();
}

void SegmentationWidget::on_eraserButton_clicked()
{


}

void SegmentationWidget::on_drawButton_clicked()
{
    tool->setDrawType(255);
}

void SegmentationWidget::on_mouseButton_clicked()
{

}

void SegmentationWidget::on_ClearButton_clicked()
{

}
