#ifndef GUIIMAGE_H
#define GUIIMAGE_H
#include <Geometrics.hpp>
#include <Image.hpp>
#include <QObject>
#include <QPixmap>
#include <QVector>
#include <Signal.hpp>
#include "displayformat.h"
class GuiImage : public QObject {
  Q_OBJECT

  Modality m_modality;
  Bial::Image< int > image;
  QString m_fileName;
  QVector<QPixmap>cachedPixmaps;
  QVector<bool>needUpdate;
  Bial::Signal equalization, histogram, equalized;
  bool m_equalizeHistogram;
  void updateBoundings(size_t axis);

public:
  explicit GuiImage( QString fName, QObject *parent = 0 );
  Modality modality( );
  QString fileName( );
  QPixmap getSlice( size_t axis );
  size_t width( size_t axis );
  size_t heigth( size_t axis );
  size_t depth( size_t axis );
  bool hasLabels( );
  size_t currentSlice( size_t axis );
  void setCurrentSlice( size_t axis, size_t slice );

  Bial::Point3D getPosition( QPointF pos, size_t axis );
  Bial::FastTransform getTransform( size_t axis );

  const Bial::Image< int > &getImage( ) const;

  void rotate90(size_t axis);
  void rotateAll90();

  int max( );
  bool getEqualizeHistogram() const;
  void setEqualizeHistogram(bool equalizeHistogram);

  const Bial::Signal &getHistogram() const;

signals:
  void imageUpdated( );

private:
  QVector< Bial::FastTransform > transform;
  QVector< Bial::BBox > bounding;
  QVector< size_t > m_currentSlice;
  int m_max;
};

#endif /* GUIIMAGE_H */
