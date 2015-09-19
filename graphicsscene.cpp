#include "graphicsscene.h"
#include <QPainter>

GraphicsScene::GraphicsScene( QObject *parent ) : QGraphicsScene( parent ) {
  m_overlay = false;
  m_overlayPen = QPen( Qt::green );
  m_pos = QPointF(-1,-1);
}

void GraphicsScene::setOverlayPen( const QPen &overlayPen ) {
  m_overlayPen = overlayPen;
  update( );
}

void GraphicsScene::updateOverlay( QPointF pos ) {
  m_pos = pos;
  update( );
}

bool GraphicsScene::overlay( ) const {
  return( m_overlay );
}

void GraphicsScene::setOverlay( bool overlay ) {
  m_overlay = overlay;
  update( );
}

void GraphicsScene::drawForeground( QPainter *painter, const QRectF &rect ) {
  if( m_overlay ) {
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( m_overlayPen );
    painter->setOpacity(0.5);
    painter->drawLine( m_pos.x( ), 0, m_pos.x( ), height( ) ); /* vertical */
    painter->drawLine( 0, m_pos.y( ), width( ), m_pos.y( ) ); /* horizontal */
  }
  QGraphicsScene::drawForeground( painter, rect );
}
