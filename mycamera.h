#ifndef MYCAMERA_H
#define MYCAMERA_H

#include"includes/MvCameraControl.h"


#pragma warning( disable : 4819 )
#include <stdio.h>
#include"iostream"
#include"opencv2/opencv.hpp"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<QDebug>
#include<QImage>
#include <string>
#include<QImageReader>
#include<QImage>
#include<QObject>

using namespace std;
using namespace cv;

class Mycamera:public QObject
{
    Q_OBJECT
public:
    explicit Mycamera(QObject *parent = nullptr);
    ~Mycamera();
    static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);//枚举相机设备列表
    int connectCamera(string id);//连接相机
    int setTriggerMode(unsigned int TriggerModeNum);//设置是否为触发模式
    int startCamera();//开启采集
    int softTrigger();//发送软触发
    int ReadBuffer(Mat &image);//读取buffer
    int setHeartBeatTime(unsigned int time);//设置心跳时间
    int setExposureTime(float ExposureTimeNum);//设置曝光时间
    int setTriggerMode();//设置触发模式
    int setAcquisitionMode();//设置采集模式
    int stopGrab();//停止采集
    int closeCamera();//关闭相机
    int callbackRegister();//注册回调函数
    int gainAuto();//开启自动增益
    static void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
private:
    void* m_hDevhandle;
    static Mycamera * mycamera1;//定义一个静态类
public:
    unsigned char* m_pBufForSaveImage=NULL;//用于保存图像的缓存
    unsigned int m_nBufSizeForSaveImage;
    unsigned char* m_pBufForDriver=NULL;//用于从驱动获取图像的缓存
    unsigned int m_nBufSizeForDriver;
signals:
    void sigSendImage(const QImage& image);
    void sigSendImageStatic(const QImage& image);
private slots:
    void slotSendImageStatic(const QImage& image);
};

#endif // MYCAMERA_H
