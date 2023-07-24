#include "opencvtoqt.h"

OpenCVtoQt::OpenCVtoQt()
{
}


// 将Mat转化位QImage
QImage OpenCVtoQt:: cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//这里还有一种写法，最后给出
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

//将Mat转化为QPixmap
QPixmap OpenCVtoQt:: cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

//将QImage转化为Mat
cv::Mat OpenCVtoQt:: QImageToCvMat( const QImage &image, bool cloneImageData)
{
    Mat mat;
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
            break;

        case QImage::Format_RGB32:
            mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
            cvtColor(mat, mat, COLOR_RGBA2BGRA);
            break;

        case QImage::Format_RGB888:
            mat = Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
            cvtColor(mat, mat, COLOR_RGB2BGR);
            break;

        default:
            qWarning() << "Unsupported image format";
            break;
    }
    if (cloneImageData)
        return mat.clone();
    else
        return mat;
}

//将QPixmap转化为Mat
cv::Mat OpenCVtoQt:: QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData)
{
    return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}

cv::Mat OpenCVtoQt::cylindricalProjection(const cv::Mat &image)
{
    cv::Mat imgOut=cv::Mat(image.rows,image.cols,CV_8UC3);
    float w=image.cols;
    float h=image.rows;
    float f=(w/2)/atan(3.1415926/8);
    for(int i=0;i<h;i++)
    {
        for(int j=0;j<w;j++)
        {
            float x=j;
            float y=i;
            float x1=f*atan((x-w/2)/f)+f*atan(w/(2.0f*f));
            float y1=f*(y-h/2.0f)/sqrt((x-w/2.0f)*(x-w/2.0f)+f*f)+h/2.0f;

            int col=(int)(x1+0.5f);//加0.5是为了四舍五入
            int row=(int)(y1+0.5f);//加0.5是为了四舍五入
            if(col<w && row<h)
            {
                imgOut.at<cv::Vec3b>(row,col)[0]=image.at<cv::Vec3b>(i,j)[0];
                imgOut.at<cv::Vec3b>(row,col)[1]=image.at<cv::Vec3b>(i,j)[1];
                imgOut.at<cv::Vec3b>(row,col)[2]=image.at<cv::Vec3b>(i,j)[2];
            }
        }
    }
    return imgOut;
}

