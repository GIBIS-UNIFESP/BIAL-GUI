#include "Draw.hpp"
#include "Geometrics.hpp"
#include "guiimage.h"
#include "segmentationtool.h"
#include "File.hpp"
#include <QDebug>
#include <QPointF>

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage,
                                                                                      viewer ),
    seeds( guiImage->getImage( ).Dim( ) ) {
    drawType = 0;
    drawing = false;
    setObjectName("SegmentationTool");

}

int SegmentationTool::type( ) {
    return( SegmentationTool::Type );
}

void SegmentationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
    drawing = true;

    if( (drawType == 1) || (drawType == 2) ){

        switch (buttons) {
        case Qt::LeftButton:
            drawType = 255;
            break;
        case Qt::RightButton:
            drawType = 2;
            break;
        default:
            drawType = 0;
            break;
        }
    }



    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    lastPoint = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    qDebug() << "Mouse clicked at " << pt;
}


void SegmentationTool::mouseMoved( QPointF pt, size_t axis ) {
    if((drawing)){
        const Bial::FastTransform &transf = guiImage->getTransform( axis );
        Bial::Point3D actual = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );

        drawSeed(lastPoint, actual);
        lastPoint = actual;
    }
}

void SegmentationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
    drawing = false;
    Bial::File::Write(seeds,"C:/image.nii.gz",guiImage->fileName().toStdString());

    Q_UNUSED( buttons );
    Q_UNUSED( axis );
    Q_UNUSED( pt );
}

void SegmentationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
    Q_UNUSED( buttons );
    Q_UNUSED( axis );
    Q_UNUSED( pt );
    /* nothing happens */
}

void SegmentationTool::sliceChanged( size_t axis, size_t slice ) {
    Q_UNUSED( slice );
    Q_UNUSED( axis );
    /* nothing happens */
}

void SegmentationTool::drawSeed(Bial::Point3D last, Bial::Point3D actual)
{
    Bial::Vector< float > vLast(3);
    vLast = {{(float)last[0], (float)last[1], (float)last[2]}};
    Bial::Vector< float > vActual;
    vActual = {{(float)actual[0], (float)actual[1], (float)actual[2]}};


    Bial::Line imgLine(vLast, vActual);
    imgLine.Draw(seeds,drawType);
}


void SegmentationTool::setDrawType(int type)
{
    drawType = type;
}

void SegmentationTool::clearSeeds()
{
    for(unsigned int i = 0; i < seeds.Size() ; i++)
        seeds[i] = 0;
}

