#include "pixmaplabelitem.h"

#include <QPainter>
#include "iostream"

PixmapLabelItem::PixmapLabelItem(QGraphicsItem * parent) : QGraphicsItem( parent ) {
}

QPixmap PixmapLabelItem::image() const {
  return m_image;
}

void PixmapLabelItem::setImage(const QPixmap & image) {
  setTransformOriginPoint(image.height()/2,image.width()/2);
  m_image = image;
}

QPixmap PixmapLabelItem::label() const {
  return m_label;
}

void PixmapLabelItem::setLabel(const QPixmap & label) {
  m_label = label;
}

QRectF PixmapLabelItem::boundingRect() const {
  return m_image.rect();
}

void PixmapLabelItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * ) {
  if(!m_image.isNull()) {
    painter->drawPixmap(0,0,m_image);
    if(!m_label.isNull()) {
      painter->drawPixmap(0,0,m_label);
    }
  }
}
