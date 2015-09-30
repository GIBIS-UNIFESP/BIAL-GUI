#ifndef GUIIMAGE_H
#define GUIIMAGE_H
#include <Geometrics.hpp>
#include <Image.hpp>
#include <QObject>
#include <QPixmap>
#include <QVector>
#include <Signal.hpp>
#include "displayformat.h"

/**
 * @brief The GuiImage class is a bridge to the Bial::Image data structure to the QImage data structure. <br>
 * It have many important functions, such as the views spatial transforms, histogram equalization and others.
 */
class GuiImage : public QObject {
  Q_OBJECT
  /**
   * @brief m_modality is the image modality.
   * @see Modality enum class.
   */
  Modality m_modality;
  /**
   * @brief image is the input image stored in the
   *        Bial::Image data structure.
   */
  Bial::Image< int > image;
  /**
   * @brief m_fileName is the image absolute path.
   */
  QString m_fileName;
  /**
   * @brief cachedPixmaps holds a copy of the last generated pixmap at each view.
   */
  QVector<QPixmap>cachedPixmaps;
  /**
   * @brief needUpdate is a flag that determines if a pixmap needs to be updated.
   */
  QVector<bool>needUpdate;
  /**
   * @brief equalization is the equalization transform.
   */
  Bial::Signal equalization;
  /**
   * @brief histogram is the input image histogram.
   */
  Bial::Signal histogram;
  /**
   * @brief equalized is the equalized image histogram.
   */
  Bial::Signal equalized;
  /**
   * @brief m_equalizeHistogram determines if the image must be equalized.
   */
  bool m_equalizeHistogram;
  /**
   * @brief transform is the affine transform related to each view.
   */
  QVector< Bial::FastTransform > transform;
  /**
   * @brief bounding is the bounding box that determines each view boundings.
   */
  QVector< Bial::BBox > bounding;
  /**
   * @brief m_currentSlice is the current slice at the z axis of each view.
   */
  QVector< size_t > m_currentSlice;
  /**
   * @brief m_max is the maximum value at the input image.
   */
  int m_max;

private:
  /**
   * @brief updateBoundings is called each time the transformation matrix is updated. <br>
   * This function applys the affone transform to the input image boundings, and fixes
   * negative positions.
   * @param axis
   */
  void updateBoundings(size_t axis);

public:
  /**
   * @brief GuiImage is the GuiImage class constructor.
   * @param fName is the absolute path to the input image.
   * @param parent is the QObject parent.
   */
  explicit GuiImage( QString fName, QObject *parent = 0 );
  /**
   * @brief modality is the image modality getter.
   * @return
   */
  Modality modality( );
  /**
   * @brief fileName is the image absolute path getter.
   * @return
   */
  QString fileName( );
  /**
   * @brief getSlice calculates and returns a QImage with the current slice of the view. <br>
   * @see setCurrentSlice
   * @param axis
   * @return
   */
  QPixmap getSlice( size_t axis );
  /**
   * @brief width is the view width.
   * @param axis
   * @return
   */
  size_t width( size_t axis );
  /**
   * @brief heigth is the view heigth.
   * @param axis
   * @return
   */
  size_t heigth( size_t axis );
  /**
   * @brief depth is the number of slices of the view.
   * @param axis
   * @return
   */
  size_t depth( size_t axis );
  /**
   * @brief currentSlice is the view's current slice.
   * @param axis
   * @return
   */
  size_t currentSlice( size_t axis );
  /**
   * @brief currentSlice sets the view's current slice.
   * @param axis
   * @param slice
   * @return
   */
  void setCurrentSlice( size_t axis, size_t slice );
  /**
   * @brief getPosition transforms the scene position to the input image coordinates.
   * @param pos is the graphicsscene position.
   * @param axis is the view number.
   * @return The input image coordinates.
   */
  Bial::Point3D getPosition( QPointF pos, size_t axis );
  /**
   * @brief getTransform returns the transform matrix of the views.
   * @param axis
   * @return
   */
  Bial::FastTransform getTransform( size_t axis );
  /**
   * @brief getImage returns a reference to the input image.
   * @return
   */
  const Bial::Image< int > &getImage( ) const;
  /**
   * @brief rotateAll90 rotates all views in 90 degrees.
   */
  void rotateAll90();
  /**
   * @brief rotate90 rotates a view in 90 degrees.
   * @param axis View axis
   */
  void rotate90(size_t axis);
  /**
   * @brief max is the maximum intensity of the input image.
   * @return
   */
  int max( );
  /**
   * @brief getEqualizeHistogram returns a boolean value that says if the image must be equalized or not.
   * @return
   */
  bool getEqualizeHistogram() const;
  /**
   * @brief setEqualizeHistogram updates a boolean value that says if the image must be equalized or not.
   * @param equalizeHistogram
   */
  void setEqualizeHistogram(bool equalizeHistogram);
  /**
   * @brief getHistogram returns the image histogram.
   * @return
   */
  const Bial::Signal &getHistogram() const;

signals:
  /**
   * @brief imageUpdated is called each time a internal property is updated,
   * after that the image views are updated.
   */
  void imageUpdated( );
};

#endif /* GUIIMAGE_H */
