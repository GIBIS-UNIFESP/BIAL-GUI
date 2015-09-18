#ifndef GUIIMAGE_H
#define GUIIMAGE_H
#include <Geometrics.hpp>
#include <Image.hpp>
#include <QObject>
#include <QVector>

#include "displayformat.h"
class GuiImage : public QObject {
  Q_OBJECT

  Modality m_modality;
  Bial::Image< int > image;
  QString m_fileName;

public:
  explicit GuiImage( QString fName, QObject *parent = 0 );
  Modality modality( );
  QString fileName( );
  QPixmap getSlice( size_t axis, size_t slice );
  size_t width( size_t axis );
  size_t heigth( size_t axis );
  size_t depth( size_t axis );
  bool hasLabels( );
  size_t currentSlice( size_t axis );
  void setCurrentSlice( size_t axis, size_t slice );

  Bial::Point3D getPosition( QPointF pos, size_t axis );
  Bial::Transform3D getTransform( size_t axis );

  const Bial::Image< int > &getImage( ) const;

  int max( );

signals:
  void imageUpdated( );

public slots:

private:
  QVector< Bial::Transform3D > transform;
  QVector< Bial::BBox > bounding;
  QVector< size_t > m_currentSlice;
  int m_max;
};

#endif /* GUIIMAGE_H */
