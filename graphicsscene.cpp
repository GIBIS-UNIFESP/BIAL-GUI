#include "graphicsscene.h"

#include <QGraphicsSceneMouseEvent>

GraphicsScene::GraphicsScene( QObject *parent ) : QGraphicsScene( parent ) {

}


void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent *event ) {

}

void GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {
  emit positionChanged( event->scenePos( ) );
}

void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) {
}
