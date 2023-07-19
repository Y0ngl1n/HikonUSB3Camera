#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTimer>

#include"mycamera.h"

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
    string cameraName;
    Mat imageMat;
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

    void processImage(const QImage& image);
    void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
