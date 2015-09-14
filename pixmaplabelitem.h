#ifndef PIXMAPLABELITEM_H
#define PIXMAPLABELITEM_H

#include <qgraphicsitem.h>

class PixmapLabelItem : public QGraphicsItem {
  QPixmap m_image;
  QPixmap m_label;
public:
  PixmapLabelItem( QGraphicsItem * parent = 0);

  // QGraphicsItem interface
public:
  QRectF boundingRect() const;
  void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *);
  QPixmap image() const;
  void setImage(const QPixmap & image);
  QPixmap label() const;
  void setLabel(const QPixmap & label);
};

#endif // PIXMAPLABELITEM_H
