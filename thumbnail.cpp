#include "thumbnail.hpp"
#include <QDebug>
#include <QFileInfo>
#include <qboxlayout.h>
#include <qlabel.h>

Thumbnail::Thumbnail( GuiImage *image, int number, int size,
                      QWidget *parent ) : QFrame( parent ), m_imageNumber( number ) {
  setFrameStyle( QFrame::Raised );
  setFrameShape( QFrame::StyledPanel );
  setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  QPixmap pix;
  if( image->modality( ) == Modality::NIfTI ) {
    pix = image->getSlice( 0, image->depth( 0 ) / 2 - 1 );
  } else {
    pix = image->getSlice( 0, 0 );
  }
  pix = pix.scaled( size, size * 2, Qt::KeepAspectRatio, Qt::SmoothTransformation );

  QLabel *imageLabel = new QLabel;
  imageLabel->setPixmap( pix );

  QFileInfo info( image->fileName( ) );
  QLabel *textLabel = new QLabel;
  textLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
  textLabel->setWordWrap( true );
  QString fname = info.fileName( );
  setToolTip( fname );
  fname.truncate( 15 );
  textLabel->setText( fname );
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget( imageLabel, 0, Qt::AlignCenter );
  layout->addWidget( textLabel, 0, Qt::AlignCenter );
  layout->addStretch( 1 );
  setLayout( layout );
  /*  setFrameShape(QFrame::Panel); */
}

void Thumbnail::mousePressEvent( QMouseEvent* ) {
  COMMENT( "Thumbnail clicked: " << imageNumber( ), 0 )
  emit changeImage( imageNumber( ) );
}

int Thumbnail::imageNumber( ) const {
  return( m_imageNumber );
}

void Thumbnail::setImageNumber( int imageNumber ) {
  m_imageNumber = imageNumber;
}
