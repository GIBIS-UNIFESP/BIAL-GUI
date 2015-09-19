#include "graphicsscene.h"
#include <QPainter>

void GraphicsScene::setOverlayPen( const QPen &overlayPen ) {
  m_overlayPen = overlayPen;
  update( );
}

void GraphicsScene::updateOverlay( QPointF pos ) {
  m_pos = pos;
  update( );
}

GraphicsScene::GraphicsScene( QObject *parent ) : QGraphicsScene( parent ) {
  m_overlayPen = QPen( Qt::green );
}

void GraphicsScene::setOverlay( bool overlay ) {
  m_overlay = overlay;
  update( );
}

void GraphicsScene::drawForeground( QPainter *painter, const QRectF &rect ) {
  if( m_overlay ) {
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( m_overlayPen );
    painter->drawLine( m_pos.x( ), 0, m_pos.x( ), height( ) );   /* vertical */
    painter->drawLine( 0, m_pos.y( ), width( ), m_pos.y( ) );   /* horizontal */
  }
  QGraphicsScene::drawForeground( painter, rect );
}
