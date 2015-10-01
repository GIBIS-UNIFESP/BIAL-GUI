#ifndef SEGMENTATIONWIDGET_H
#define SEGMENTATIONWIDGET_H

#include <QWidget>

namespace Ui {
  class Segmentationwidget;
}

class Segmentationwidget : public QWidget
{
    Q_OBJECT

  public:
    explicit Segmentationwidget(QWidget *parent = 0);
    ~Segmentationwidget();

  private slots:


    void on_SegmentationButton_clicked();
    void iftOgs();
  private:
    Ui::Segmentationwidget *ui;
    Bial::Image seeds;

};

#endif // SEGMENTATIONWIDGET_H
