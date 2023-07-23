#include "grabimgthread.h"

GrabimgThread::GrabimgThread(int id,Mycamera *camera)
{
    m_cameraId = id;
    m_camera = camera;
}

void GrabimgThread::run()
{
    if(m_cameraId==0)
    {
        int offTriggerMode=m_camera->setTriggerMode(0,0);
        int acqMode=m_camera->setAcquisitionMode();
        //注册回调函数
        int callBackset = m_camera->callbackRegister(0);
        m_camera->startCamera(0);
    }
    else if(m_cameraId==1)
    {
        int offTriggerMode=m_camera->setTriggerMode(0,1);
        int acqMode=m_camera->setAcquisitionMode();
        //注册回调函数
        int callBackset = m_camera->callbackRegister(1);
        m_camera->startCamera(1);
    }
}
