#ifndef TRANSFORMIMAGE_H
#define TRANSFORMIMAGE_H

#include "opencv2/opencv.hpp"
#include<QImage>

using namespace cv;

class Transformimage
{
public:
    Transformimage();

private:
    Mat cylindricalProjection(const Mat &image);

    Mat qImage2matImage(const QImage &image);

    Mat m_image4cylindrical;
};

#endif // TRANSFORMIMAGE_H
