#include "showimgthread.h"


Showimgthread::Showimgthread(QLabel *label, QImage image,int id)
{
    m_label=label;
    m_imagercv=image;
    m_id=id;
}

void Showimgthread::run()
{
        //调整图像大小以适应QLabel
        QSize size = m_label->size();
        m_label->setPixmap(QPixmap::fromImage(m_imagercv).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
