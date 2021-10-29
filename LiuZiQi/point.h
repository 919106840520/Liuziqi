#ifndef POINT_H
#define POINT_H


class Point
{
private:
    int x,y;
public:
    const int BORDER_X = 20;
    const int BORDER_Y = 20;
    const int SIZE = 20;
    const int WIDTH = 40;
    Point();
    Point(int x,int y);
    Point& operator=(Point const&);
    int getx();
    int gety();
    bool transform();      //将qt坐标改为棋盘坐标
};

#endif // POINT_H
