#include "mycamera.h"

MV_CC_DEVICE_INFO_LIST m_stDevList;//设备信息列表结构体变量，用来存储设备列表
MV_CC_DEVICE_INFO *m_Device = NULL; //设备对象

Mycamera *Mycamera::mycamera1 = nullptr;

Mycamera::Mycamera(QObject *parent):QObject(parent)
{
    mycamera1=this;
    connect(this,&Mycamera::sigSendImageStatic,this,&Mycamera::slotSendImageStatic);//关联内部信号和槽
    m_hDevhandle=NULL;
}

Mycamera::~Mycamera()
{
    if(m_hDevhandle)
    {
        MV_CC_DestroyHandle(m_hDevhandle);
        m_hDevhandle=NULL;
    }
}

//查询设备列表
int Mycamera::EnumDevices(MV_CC_DEVICE_INFO_LIST *pstDevList)
{
    int temp=MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE,pstDevList);
    if(MV_OK!=temp)
    {
        return -1;
    }
    return 0;
}

//连接相机
int Mycamera::connectCamera(string id)
{
    int temp=EnumDevices(&m_stDevList);
    //设备更新成功接收命令的返回值为0，返回值不为0则为异常
    if(temp!=0)
    {
        return -1;
    }
    //未找到任何相机
    if(m_stDevList.nDeviceNum==0)
    {
        return 2;
    }
    for(unsigned int i=0;i<m_stDevList.nDeviceNum;i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if(NULL==pDeviceInfo){
            continue;
        }
        if(id== (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName||id== (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber)
        {
            m_Device= m_stDevList.pDeviceInfo[i];
            break;
        }else
        {
            continue;
        }
    }
    if(m_Device==NULL)
    {
        //未找到指定名称的相机
        qDebug()<<"未找到指定名称的相机";
        return 3;
    }
    temp=MV_CC_CreateHandle(&m_hDevhandle,m_Device);
    if(temp!=0)
    {
        return -1;
    }
    temp=MV_CC_OpenDevice(m_hDevhandle);
    if(temp!=0)
    {
        MV_CC_DestroyHandle(m_hDevhandle);
        m_hDevhandle=NULL;
        return -1;
    }
    else
    {
        setTriggerMode(1);
        return 0;
    }
    if(m_Device->nTLayerType==MV_GIGE_DEVICE)
    {
        std::cout<<"Gige Camera"<<std::endl;
    }
}
//设置相机触发模式
int Mycamera::setTriggerMode(unsigned int TriggerModeNum)
{
    int nRet=MV_CC_SetTriggerMode(m_hDevhandle,TriggerModeNum);
    if(MV_OK!=nRet)
    {
        return -1;
    }
    return 0;
}
//启动相机采集
int Mycamera::startCamera()
{
    int temp=MV_CC_StartGrabbing(m_hDevhandle);
    if(temp!=0)
    {
        qDebug()<<"MV_CC_StartGrabing Failed";
        return -1;
    }
    else
    {
        qDebug()<<"MV_CC_StartGrabing";
        return 0;
    }
}
//相机的图像采集模式分为内触发模式与外触发模式。其中内触发模式包含连续采集、单帧采集两种形式；外触发模式包含软件触发、硬件外触发。
//外触发：软触发
int Mycamera::softTrigger()
{
    int enumValue=MV_CC_SetEnumValue(m_hDevhandle,"TriggerSource",MV_TRIGGER_SOURCE_SOFTWARE);
    if(enumValue!=0)
    {
        qDebug()<<"设置软触发失败";
        return -1;
    }
    else
    {
        qDebug()<<"设置软触发";
    }
    int comdValue=MV_CC_SetCommandValue(m_hDevhandle,"TriggerSoftware");
    if(comdValue!=0)
    {
        qDebug() << "软触发失败";
        return -1;
    }else
    {
        qDebug() << "软触发一次";
        return 0;
    }
}

int Mycamera::ReadBuffer(Mat &image)
{
    unsigned int nBufSize=0;//缓存大小
    MVCC_INTVALUE stIntvalue;//获取一帧数据的大小
    memset(&stIntvalue,0,sizeof(MVCC_INTVALUE));//将给定地址开始逐个字节初始化为0
    int tempValue=MV_CC_GetIntValue(m_hDevhandle,"PayloadSize",&stIntvalue);
    if (tempValue != 0)
    {
        qDebug() << "GetIntValue失败";
        return -1;
    }
    else
    {
        qDebug() << "GetIntValue成功";
    }
    nBufSize = stIntvalue.nCurValue;//一帧数据的大小当前值
    m_pBufForDriver = (unsigned char *)malloc(nBufSize);//分配空间给 从驱动获取一帧图像的缓存
    MV_FRAME_OUT_INFO_EX stImageInfo;//一帧数据的信息
    memset(&stImageInfo,0,sizeof(MV_FRAME_OUT_INFO));//分配空间给 一帧详细信息的缓存
    qDebug() << MV_CC_StartGrabbing(m_hDevhandle);//抓图
    //采用超时机制获取一帧图片，SDK内部等待直到有数据时返回,成功返回MV_OK,失败返回错误码
    //1.句柄handle，2.图像数据接收指针，3.空间大小，4.图像信息结构体 5.等待超过时间
    int timeout= MV_CC_GetOneFrameTimeout(m_hDevhandle, m_pBufForDriver, nBufSize, &stImageInfo, 1000);
    if(timeout!=0)
    {
        qDebug() << "GetOneFrameTimeout失败";
        return -1;
    }
    m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 4024;//保存图像的缓存
    m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage); //向系统申请M_nBufSizeForSaveImage内存空间
    bool isMono;//判断是否为黑白图像
    switch (stImageInfo.enPixelType) //像素格式
    {
        case PixelType_Gvsp_Mono8:
        case PixelType_Gvsp_Mono10:
        case PixelType_Gvsp_Mono10_Packed:
        case PixelType_Gvsp_Mono12:
        case PixelType_Gvsp_Mono12_Packed:
            isMono=true;
            break;
        default:
            isMono=false;
            break;
    }
    if(isMono)
    {
        image=Mat(stImageInfo.nHeight,stImageInfo.nWidth,CV_8UC1,m_pBufForDriver);
    }
    else
    {
        //转换图像格式为BGR8
        MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
        memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
        stConvertParam.nWidth = stImageInfo.nWidth;                 //ch:图像宽 | en:image width
        stConvertParam.nHeight = stImageInfo.nHeight;               //ch:图像高 | en:image height
        stConvertParam.pSrcData = m_pBufForDriver;                  //ch:输入数据缓存 | en:input data buffer
        stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;         //ch:输入数据大小 | en:input data size
        stConvertParam.enSrcPixelType = stImageInfo.enPixelType;    //ch:输入像素格式 | en:input pixel format
        stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed; //ch:输出像素格式 | en:output pixel format
        stConvertParam.pDstBuffer = m_pBufForSaveImage;                    //ch:输出数据缓存 | en:output data buffer
        stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;            //ch:输出缓存大小 | en:output buffer size
        MV_CC_ConvertPixelType(m_hDevhandle, &stConvertParam);
        image=Mat(stImageInfo.nHeight,stImageInfo.nWidth,CV_8UC3,m_pBufForSaveImage);
        imwrite("1.bmp",image);
    }
    // 释放之前分配的内存块，如果不为NULL的话
    if (m_pBufForDriver != NULL)
    {
        free(m_pBufForDriver);
        m_pBufForDriver = NULL; // 将指针置为NULL，避免出现悬空指针
    }
    return 0;
}

int Mycamera::setHeartBeatTime(unsigned int time)
{

    //心跳时间最小为500ms
    if(time<500)
        time=500;
    int temp=MV_CC_SetIntValue(m_hDevhandle, "GevHeartbeatTimeout", time);
    if(temp!=0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int Mycamera::setExposureTime(float ExposureTimeNum)
{
    int temp= MV_CC_SetFloatValue(m_hDevhandle, "ExposureTime",ExposureTimeNum );
    if(temp!=0)
        return -1;
    return 0;
}

int Mycamera::setTriggerMode()
{
    int offTriggerMode=MV_CC_SetEnumValue(m_hDevhandle,"TriggerMode",MV_TRIGGER_MODE_OFF);
    if(offTriggerMode != 0){
        qDebug() << "设置内触发失败";
    }else {
        qDebug() << "成功设置内触发";
    }
    int nRet = MV_CC_SetFrameRate(m_hDevhandle,30);
    if(nRet != 0){
        qDebug() << "设置帧率失败";
    }else {
        qDebug() << "成功设置帧率为30";
    }
}

int Mycamera::setAcquisitionMode()
{
    int acqMode=MV_CC_SetEnumValue(m_hDevhandle,"AcquisitionMode",MV_ACQ_MODE_CONTINUOUS);
    if(acqMode != 0){
        qDebug() << "设置连续采集失败";
    }else {
        qDebug() << "成功设置连续采集";
    }
}

int Mycamera::stopGrab()
{
    int temp=MV_CC_StopGrabbing(m_hDevhandle);
    if(temp!=0)
    {
        qDebug()<<"停止采集失败";
        return -1;
    }
    else
    {
        qDebug()<<"停止采集成功";
        return 0;
    }
}

int Mycamera::closeCamera()
{
    int nRet = MV_OK;
    if (NULL == m_hDevhandle)
    {
        qDebug() << "没有句柄，不用关闭";
        return -1;
    }
    MV_CC_CloseDevice(m_hDevhandle);
    nRet = MV_CC_DestroyHandle(m_hDevhandle);
    m_hDevhandle = NULL;
    return nRet;
}
//回调函数定义
void Mycamera::ImageCallBackEx(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
   qDebug()<<"number:"<<pFrameInfo->nFrameNum;
    QImage image = QImage(pData, pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);
    emit mycamera1->sigSendImageStatic(image);//发射内部信号
}

void Mycamera::slotSendImageStatic(const QImage& image)
{
    emit mycamera1->sigSendImage(image);//转发信号，发射出去信号
}

int Mycamera::callbackRegister()
{
    int nRet = MV_CC_SetImageNodeNum(m_hDevhandle,MV_GrabStrategy_LatestImagesOnly);
    //注册回调函数
    nRet = MV_CC_RegisterImageCallBackForRGB(m_hDevhandle,ImageCallBackEx,m_hDevhandle);
    return nRet;
}

int Mycamera::gainAuto()
{
    int nRet = MV_CC_SetEnumValue(m_hDevhandle,"GainAuto",1);
    if(nRet != 0){
        qDebug() << "设置自动增益失败";
    }else {
        qDebug() << "成功设置自动增益";
    }
    return nRet;
}
