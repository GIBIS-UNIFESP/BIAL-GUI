#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene {
  bool m_overlay;
  QPen m_overlayPen;
  QPointF m_pos;
public:
  explicit GraphicsScene( QObject *parent = 0 );
  void setOverlay( bool overlay );
  void setOverlayPen( const QPen &overlayPen );
  void updateOverlay( QPointF pos );

protected:
  void drawForeground( QPainter *painter, const QRectF &rect );
};

#endif /* GRAPHICSSCENE_H */
