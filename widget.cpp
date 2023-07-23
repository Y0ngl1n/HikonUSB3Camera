#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //Mycamera对象
    m_camera = new Mycamera;

    int nRet=m_camera->EnumDevices(&m_stDevList);
    qDebug()<<"Online Camera:"<<m_stDevList.nDeviceNum;
    //获取相机IP地址
    for(int i=0;i<m_stDevList.nDeviceNum;i++)
    {
        unsigned char* chmn = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chModelName;
        unsigned char* chvn = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chVendorName;
        unsigned char* chguid = m_stDevList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chDeviceGUID;

        qDebug() <<QString::fromLatin1(reinterpret_cast<const char*>(chmn))<<"-"<<
                   QString::fromLatin1(reinterpret_cast<const char*>(chvn))<<"-"<<
                   QString::fromLatin1(reinterpret_cast<const char*>(chguid));
    }
    // 连接信号槽
    connect(m_camera,&Mycamera::sigSendImage,this,&Widget::processImage);
    connect(m_camera,&Mycamera::sigSendImage2,this,&Widget::processImage2);
    qDebug()<<"Successfully Initialized!";
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
//连接相机1
void Widget::on_pushButton_clicked()
{
    int linkCamera = m_camera->connectCamera(0);
    if(linkCamera == 0){
        qDebug() << "连接相机1成功";
    }else {
        qDebug() << "连接相机1失败";
    }
}
//关闭相机1
void Widget::on_pushButton_2_clicked()
{
    //关闭设备，释放资源
    int close = m_camera->closeCamera(0);
    if(close != 0){
        qDebug() << "相机1关闭失败";
    }
    else
    {
        qDebug()<<"关闭相机1";
    }
}
//相机1软触发
void Widget::on_pushButton_3_clicked()
{

    //设置相机1软触发
        int softTrigger = m_camera->softTrigger(0);//发送软触发
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
//相机1停止采集
void Widget::on_pushButton_4_clicked()
{
    //停止采集
    int stopCamera = m_camera->stopGrab(0);
    if(stopCamera != 0){
        qDebug() << "停止相机采集失败";
    }else {
        qDebug() << "正在停止采集";
    }
}
//相机1曝光度设置
void Widget::on_pushButton_5_clicked()
{
    int setExposure = m_camera->setExposureTime(150000,0);
    if(setExposure != 0){
        qDebug() << "设置曝光度失败";
    }else {
        qDebug() << "成功设置曝光度";
    }
}
//相机1设置内触发，回调函数注册,开始连续采集
void Widget::on_pushButton_6_clicked()
{
    GrabimgThread *gthread = new GrabimgThread(0,m_camera);
    gthread->start();
}
//相机1连续采集的图像接收并显示在QLabel上
void Widget::processImage(const QImage& image,int id)
{
    imagercv = image;
    Showimgthread *sthread = new Showimgthread(ui->label_image,image,0);
    sthread->start();
}

void Widget::processImage2(const QImage &image, int id)
{
    imagercv2 = image;
    Showimgthread *sthread1 = new Showimgthread(ui->label_image_2,image,1);
    sthread1->start();
}
//相机1自动增益
void Widget::on_pushButton_7_clicked()
{
    int nRet = m_camera->gainAuto(0);
}
//保存相机1连续采集的最新一张图片
void Widget::on_pushButton_8_clicked()
{
    //将QImage保存
    //获取当前日期和时间，并将其格式化为字符串（例如：2023-07-19_134512）
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

    // 构造文件名，例如 "image_2023-07-19_134512.bmp"
    QString filename = "image_" + timestamp + ".bmp";

    //将图像保存为文件
    if (imagercv.save(filename)) {
        qDebug() << "保存成功：" << filename;
    } else {
        qDebug() << "保存失败：" << filename;
    }
}
//连接相机2
void Widget::on_pushButton_9_clicked()
{
    int linkCamera = m_camera->connectCamera(1);
    if(linkCamera == 0){
        qDebug() << "连接相机2成功";
    }else {
        qDebug() << "连接相机2失败";
    }
}
//设置相机2软触发
void Widget::on_pushButton_10_clicked()
{
    //设置相机2软触发
        int softTrigger = m_camera->softTrigger(1);//发送软触发
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
//相机2曝光度设置
void Widget::on_pushButton_15_clicked()
{
    int setExposure = m_camera->setExposureTime(150000,1);
    if(setExposure != 0){
        qDebug() << "设置曝光度失败";
    }else {
        qDebug() << "成功设置曝光度";
    }
}
//相机2停止采集
void Widget::on_pushButton_14_clicked()
{
    //停止采集
    int stopCamera = m_camera->stopGrab(1);
    if(stopCamera != 0){
        qDebug() << "停止相机采集失败";
    }else {
        qDebug() << "正在停止采集";
    }
}
//关闭相机2
void Widget::on_pushButton_13_clicked()
{
    //关闭设备，释放资源
    int close = m_camera->closeCamera(1);
    if(close != 0){
        qDebug() << "相机关闭失败";
    }
    else
    {
        qDebug()<<"关闭相机2";
    }
}
//相机2自动增益
void Widget::on_pushButton_12_clicked()
{
    int nRet = m_camera->gainAuto(1);
}
//保存相机2连续采集的最新一张图片
void Widget::on_pushButton_11_clicked()
{
    //将QImage保存
    //获取当前日期和时间，并将其格式化为字符串（例如：2023-07-19_134512）
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

    // 构造文件名，例如 "image_2023-07-19_134512.bmp"
    QString filename = "image2_" + timestamp + ".bmp";

    //将图像保存为文件
    if (imagercv2.save(filename)) {
        qDebug() << "保存成功：" << filename;
    } else {
        qDebug() << "保存失败：" << filename;
    }
}
//相机2设置内触发，回调函数注册,开始连续采集
void Widget::on_pushButton_16_clicked()
{
    GrabimgThread *gthread1 = new GrabimgThread(1,m_camera);
    gthread1->start();
}
//拼接imagercv和imagercv1
void Widget::on_pushButton_17_clicked()
{

}
