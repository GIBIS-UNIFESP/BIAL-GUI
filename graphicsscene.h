#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>

class GraphicsScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit GraphicsScene( QObject *parent = 0 );

signals:
  void positionChanged(QPointF pt);

public slots:

  // QGraphicsScene interface
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent * event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
};

#endif /* GRAPHICSSCENE_H */
