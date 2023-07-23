#ifndef GRABIMGTHREAD_H
#define GRABIMGTHREAD_H

#include <QObject>
#include<QThread>
#include"mycamera.h"

class GrabimgThread : public QThread
{
    Q_OBJECT
public:
    explicit GrabimgThread(int id,Mycamera *camera);
protected:
    void run();
signals:

private:
    int m_cameraId;
    Mycamera *m_camera;
public slots:
};

#endif // GRABIMGTHREAD_H
