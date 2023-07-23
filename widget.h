#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTimer>
#include<QDateTime>
#include"mycamera.h"
#include"grabimgthread.h"
#include"showimgthread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Mycamera *m_camera;
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    string cameraId;
    Mat imageMat;
    QImage imagercv;
    QImage imagercv2;
    QImage cvMat2QImage(const cv::Mat& mat);
    QImage image;
    QTimer* timer;
private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void processImage(const QImage& image,int id);

    void processImage2(const QImage& image,int id);

    void on_pushButton_10_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
