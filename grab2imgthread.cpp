#include "grab2imgthread.h"

grab2imgthread::grab2imgthread(QImage img1,QImage img2)
{
    qDebug()<<"构造函数执行";
    m_img1=img1;
    m_img2=img2;
}

void grab2imgthread::run()
{
    imageCylindrical = OpenCVtoQt::cylindricalProjection(OpenCVtoQt::QImageToCvMat(m_img1));
    imageCylindrical2 = OpenCVtoQt::cylindricalProjection(OpenCVtoQt::QImageToCvMat(m_img2));
    imwrite("Cylindrical.bmp",imageCylindrical);
    imwrite("Cylindrical2.bmp",imageCylindrical2);
}
