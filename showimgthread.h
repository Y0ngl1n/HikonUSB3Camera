#ifndef SHOWIMGTHREAD_H
#define SHOWIMGTHREAD_H
#include<QThread>
#include<QWidget>
#include<QImage>
#include<QLabel>
#include"mycamera.h"

class Showimgthread:public QThread
{
    Q_OBJECT
public:
    explicit Showimgthread(QLabel *label,QImage image,int id);
private:
    QLabel *m_label;
    QImage m_imagercv;
    int m_id;
protected:
    void run();
};

#endif // SHOWIMGTHREAD_H
