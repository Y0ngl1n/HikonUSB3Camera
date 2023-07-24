#ifndef GRAB2IMGTHREAD_H
#define GRAB2IMGTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include"opencvtoqt.h"
#include"opencv2/opencv.hpp"
using namespace cv;
class grab2imgthread : public QThread
{
    Q_OBJECT
public:
    explicit grab2imgthread(QImage img1,QImage img2);
protected:
    void run();
signals:
private:
    QImage m_img1;
    QImage m_img2;
    Mat imagercvMat;
    Mat imagercv2Mat;
    Mat imageCylindrical;
    Mat imageCylindrical2;
public slots:
};

#endif // GRAB2IMGTHREAD_H
