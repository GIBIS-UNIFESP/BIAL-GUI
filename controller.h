#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "guiimage.h"
#include "pixmaplabelitem.h"

#include <QObject>
#include <QVector>

class ThumbsWidget;

/**
 * @brief The Controller class is one of the most important classes of the User Interface,
 * and is responsible to manage the images.
 */
class Controller : public QObject {
  Q_OBJECT

  DisplayFormat * bwFormat;
  DisplayFormat * rgbFormat;
  DisplayFormat * niftiFormat;
  /**
   *
   * @brief m_images holds all opened images.
   *
   */
  QVector< GuiImage* > m_images;
  /**
   *
   * @brief m_pixmapItems holds all pixmapLabelItems.
   *        This class holds two pixmaps, one for the image,
   *        and one for the label.
   *
   */
  QVector< QGraphicsPixmapItem * > m_pixmapItems;
  /**
   *
   * @brief m_currentImagePos holds the current image position.
   *
   */
  int m_currentImagePos;
  /**
   * @brief m_thumbsWidget is a pointer to the thumbnails dock.
   */
  ThumbsWidget *m_thumbsWidget;
  /**
   * @brief scale
   */
  double scale;
public:
  enum { MaxRecentFiles = 10 };
  /**
   *
   * @brief Controller's constructor
   * @param views is the number of the views of the imageViewer.
   * @param parent is the parent object.
   *
   */
  explicit Controller( int views, QObject *parent = 0 );
  /**
   *
   * @brief currentImage
   * @return A pointer to the current guiimage.
   *
   */
  GuiImage* currentImage( );
  /**
   *
   * @brief currentImage
   * @return A pointer to the current guiimage.
   *
   */
  GuiImage* imageAt( int pos );
  /**
   *
   * @brief currentImagePos
   * @return The current image position
   *
   */
  int currentImagePos( ) const;
  /**
   *
   * @brief addImage Adds an image to vector.
   * @param fname is the file name of the Image to be opened.
   *
   */
  bool addImage( QString fname );
  /**
   *
   * @brief addLabel Adds a label to the current image.
   * @param label is the file name of the label to be opened.
   *
   */
  bool addLabel( QString label );
  /**
   *
   * @brief removeCurrentLabel removes the current label from current image.
   * @return true if removed successfully.
   *
   */
  bool removeCurrentLabel( );
  /**
   *
   * @brief removeCurrentImage removes the current image from vector.
   *
   */
  void removeCurrentImage( );
  /**
   *
   * @brief isEmpty
   * @return true the vector is empty
   *
   */
  bool isEmpty( );
  /**
   *
   * @brief clear Clears the image vector, and resets thumbnails.
   *
   */
  void clear( );
  /**
   *
   * @brief size
   * @return The image vector size.
   *
   */
  int size( );
  /**
   * @brief setThumbsWidget setThumbsWidget sets the pointer to the thumbnails dock.
   * @param thumbsWidget
   */
  void setThumbsWidget( ThumbsWidget *thumbsWidget );
  /**
   * @brief currentFormat
   */
  DisplayFormat * currentFormat( );

  QGraphicsPixmapItem* getPixmapItem( size_t axis );
  DisplayFormat * getNiftiFormat() const;
  void setNiftiFormat(DisplayFormat * value);

signals:
  /**
   * @brief This signal is emmited every time the current image changes.
   */
  void currentImageChanged( );
  /**
   * @brief This signal is emmited every time the current image is updated.
   */
  void imageUpdated( );
  /**
   * @brief This signal is emmited avery time the m_images vector is updated.
   */
  void containerUpdated( );
  /**
   * @brief recentFilesUpdated
   */
  void recentFilesUpdated( );
public slots:
  /**
   *
   * @brief update updates the image pixmaps.
   *
   */
  void update( );
  /**
   *
   * @brief setCurrentImagePos
   * @param position is the position of the image in the images vector.
   *
   */
  void setCurrentImagePos( int position );
  /**
   *
   * @brief loadNextImage is a slot called from controlsdock
   * that loads the next image ( like an circular list ).
   *
   */
  void loadNextImage( );
  /**
   *
   * @brief changeOthersSlices is called when the mouse is clicked over
   *  an 3d image view (axial, sagittal and coronal).
   * @param posF is the mouse pointer position in the scene.
   * @param axis is the number of the view's axis.
   *
   */
  void changeOthersSlices(QPointF posF, size_t axis );
  /**
   * @brief setCurrentSlice is called by the imageViewer when the slider or
   *  the spinbox have theis values updated.
   * @param axis
   * @param slice
   */
  void setCurrentSlice( size_t axis, size_t slice );
  /**
   * @brief setZoom
   * @param value
   */
  void setZoom( int value );
private:
  /**
   * @brief setRecentFile
   * @param file
   */
  void setRecentFile( QString fname );
};

#endif /** CONTROLLER_H */
