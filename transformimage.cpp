#include "transformimage.h"


Transformimage::Transformimage()
{

}

Mat Transformimage::cylindricalProjection(const Mat &image)
{
    m_image4cylindrical=image;

    Mat imgOut=Mat(m_image4cylindrical.rows,m_image4cylindrical.cols,CV8UC3);
    float w=m_image4cylindrical.cols;
    float h=m_image4cylindrical.rows;
    float f=(w/2)/atan(Pl/8);
    for(inti=0;i<h;i++)
    {
        for(intj=0;j<w;j++)
        {

            float x=j;
            float y=i;
            float x1=f*atan((x-w/2)/f)+f*atan(w/(2.0f*f));
            float y1=f*(y-h/2.0f)/sqrt((x-w/2.0f)*(x-w/2.0f)+f*f)+h/2.0f;

            int col=(int)(x1+0.5f);//加0.5是为了四舍五入
            int row=(int)(y1+0.5f);//加0.5是为了四舍五入
            if(col<w && row<h)
            {
                imgOut.at<Vec3b>(row,col)[0]=m_image4cylindrical.at<Vec3b>(i,j)[0];
                imgOut.at<Vec3b>(row,col)[1]=m_image4cylindrical.at<Vec3b>(i,j)[1];
                imgOut.at<Vec3b>(row,col)[2]=m_image4cylindrical.at<Vec3b>(i,j)[2];
            }
        }
    }
    imshow("imgOut",imgOut);
    return imgOut;
}

Mat Transformimage::qImage2matImage(const QImage &image)
{

}
