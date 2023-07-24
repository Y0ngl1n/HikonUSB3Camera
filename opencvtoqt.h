#ifndef OPENCVTOQT_H
#define OPENCVTOQT_H
#include <QLabel>
#include <QDebug>
#include"opencv2/opencv.hpp"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <vector>
using namespace cv;
class OpenCVtoQt
{
public:
    OpenCVtoQt();
    static void LabeltoMat(QLabel *label, cv::Mat &mat);
    static QImage cvMatToQImage( const cv::Mat &inMat );
    static QPixmap cvMatToQPixmap( const cv::Mat &inMat);
    static cv::Mat QImageToCvMat( const QImage &image, bool inCloneImageData = true );
    static cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );
    static cv::Mat cylindricalProjection(const cv::Mat &image);
};

#endif // OPENCVTOQT_H

