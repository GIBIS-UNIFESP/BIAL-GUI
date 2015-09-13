#ifndef THUMBSWIDGET_H
#define THUMBSWIDGET_H

#include <QWidget>

namespace Ui {
  class ThumbsWidget;
}

class ThumbsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ThumbsWidget( QWidget *parent = 0 );
  ~ThumbsWidget( );

private:
  Ui::ThumbsWidget *ui;
};

#endif /* THUMBSWIDGET_H */
