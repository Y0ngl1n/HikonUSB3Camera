#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_camera=new Mycamera;
    int nRet=m_camera->EnumDevices(&m_stDevList);
    qDebug()<<"online number:"<<m_stDevList.nDeviceNum;
    //获取相机IP地址
    for(int i=0;i<m_stDevList.nDeviceNum;i++)
    {
        unsigned char* chmn = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chModelName;
        unsigned char* chvn = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chVendorName;
        unsigned char* chguid = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chDeviceGUID;
        unsigned char* chdfnm = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chUserDefinedName;
        qDebug() <<chmn<<"-"<<chvn<<"-"<<chguid<<"-"<<chdfnm;
    }
    // 连接信号槽
    connect(m_camera,&Mycamera::sigSendImage,this,&Widget::processImage);
}

Widget::~Widget()
{
    delete ui;
}

QImage Widget::cvMat2QImage(const Mat &mat)
{

    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage qimage(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        qimage.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            qimage.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = qimage.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return qimage;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

void Widget::on_pushButton_clicked()
{
    cameraName = (char *)m_stDevList.pDeviceInfo[0]->SpecialInfo.stUsb3VInfo.chUserDefinedName;
    int linkCamera = m_camera->connectCamera(cameraName);
    if(linkCamera == 0){
        qDebug() << "连接相机成功";
    }else {
        qDebug() << "连接相机失败";
    }

}

void Widget::on_pushButton_2_clicked()
{
    //关闭设备，释放资源
    int close = m_camera->closeCamera();
    if(close != 0){
        qDebug() << "相机关闭失败";
    }
    else
    {
        qDebug()<<"关闭相机";
    }
}

void Widget::on_pushButton_3_clicked()
{

    //设置相机软触发
        int softTrigger = m_camera->softTrigger();//发送软触发
        if(softTrigger != 0){
            qDebug() << "失败";
        }else {
            qDebug() << "成功触发一次";
        }
        int readInt = m_camera->ReadBuffer(imageMat);
        if(readInt != 0){
            qDebug() << "读取图像失败";
        }
        image = cvMat2QImage(imageMat);
        // 调整图像大小以适应QLabel
        QSize size = ui->label_image->size();
        ui->label_image->setPixmap(QPixmap::fromImage(image).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        if (m_camera->m_pBufForSaveImage != NULL)
        {
            free(m_camera->m_pBufForSaveImage);
            m_camera->m_pBufForSaveImage = NULL; // 将指针置为NULL，避免出现悬空指针
        }
}

void Widget::on_pushButton_4_clicked()
{
    //停止采集
    int stopCamera = m_camera->stopGrab();
    if(stopCamera != 0){
        qDebug() << "停止相机采集失败";
    }else {
        qDebug() << "正在停止采集";
    }
}

void Widget::on_pushButton_5_clicked()
{
    int setExposure = m_camera->setExposureTime(100000);
    if(setExposure != 0){
        qDebug() << "设置曝光度失败";
    }else {
        qDebug() << "成功设置曝光度";
    }
}
//内触发
void Widget::on_pushButton_6_clicked()
{
    int offTriggerMode=m_camera->setTriggerMode(0);
    int acqMode=m_camera->setAcquisitionMode();
    //注册回调函数
    int callBackset = m_camera->callbackRegister();
    m_camera->startCamera();
}

void Widget::processImage(const QImage& image)
{
    QImage imagercv;
    imagercv=image;
    qDebug()<<"Information:"<<imagercv.width();
    //调整图像大小以适应QLabel
    QSize size = ui->label_image->size();
    ui->label_image->setPixmap(QPixmap::fromImage(imagercv).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Widget::on_pushButton_7_clicked()
{
    int nRet = m_camera->gainAuto();
}
