#include "point.h"
#include "gamewindow.h"
#include "QDebug"

Point::Point()
{

}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}
Point& Point::operator=(Point const&P)
{
    this->x = P.x;
    this->y = P.y;
    return *this;
}
bool Point::transform()
{
    this->x -= 10;
    this->y -= 10;      //减去鼠标的误差
    int tx = (this->x - BORDER_X) / WIDTH;
    int ty = (this->y - BORDER_Y) / WIDTH;

    //选距离最近的点
    const int der[4][2] = {1,0,1,1,0,1,0,0};
    int min = (this->x - tx) * (this->x - tx) + (this->y - ty) * (this->y - ty),mind = 3;
    for(int i = 0;i < 3;i++)
    {
        int ttx = (tx + der[i][0]) * WIDTH + BORDER_X;
        int tty = (ty + der[i][1]) * WIDTH + BORDER_Y;
        int n = (this->x - ttx) * (this->x - ttx) + (this->y - tty) * (this->y - tty);
        if(n < min)
        {
            min = n;
            mind = i;
        }
    }
    this->x = tx + der[mind][0];
    this->y = ty + der[mind][1];
    //qDebug()<<x<<y;
    return true;
}

int Point::getx()
{
    return this->x;
}

int Point::gety()
{
    return this->y;
}
