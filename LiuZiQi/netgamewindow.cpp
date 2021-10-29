#include "netgamewindow.h"
#include "ui_netgamewindow.h"

NetGamewindow::NetGamewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetGamewindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    socket = new QTcpSocket(this);
    //this->setFixedSize(BORDER_X * 2 + SIZE * WIDTH - WIDTH,BORDER_X * 2 + SIZE * WIDTH - WIDTH);//不希望大小被改变
    initgame();
}

NetGamewindow::~NetGamewindow()
{
    delete ui;
}

void NetGamewindow::initgame()
{
    iswin = false;
    ismyturn = true;
    full = false;
    this->setFixedSize(BORDER_X * 2 + SIZE * WIDTH + 4 * WIDTH,BORDER_X * 2 + SIZE * WIDTH - WIDTH);//不希望大小被改变
    movex = -1;
    movey = -1;
    piecenum = 0;
    black = true;
    for(int i = 0;i < SIZE;i++)
    {
        for(int j = 0;j < SIZE;j++)
        {
            QiPan[i][j] = -1;
        }
    }
}

void NetGamewindow::paintEvent(QPaintEvent *ev)
{
    QPalette palette;
    QPainter painter(this);
    QPen pen = painter.pen();
    painter.setPen(Qt::darkGreen);

    centralWidget()->setMouseTracking(true);  //使得mousemoveevent不用按下左键

    this->setAutoFillBackground(true);//自动根据颜色设置背景
    //qDebug("Enter");

    //设置背景
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);

    //画棋盘
    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(BORDER_X / 2, BORDER_Y / 2, BORDER_X + SIZE * WIDTH - WIDTH,BORDER_Y + SIZE * WIDTH - WIDTH);
    //棋盘线
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i = 0;i < SIZE;i++)
    {
        painter.drawLine(BORDER_X  + i * WIDTH,BORDER_Y,BORDER_X + i * WIDTH,BORDER_Y + SIZE * WIDTH - WIDTH);
    }
    for(int i = 0;i < SIZE;i++)
    {
        painter.drawLine(BORDER_X,BORDER_Y + i * WIDTH,BORDER_X + SIZE * WIDTH - WIDTH,BORDER_Y + i * WIDTH);
    }
    //画棋子
    for(int i = 0;i < piecenum;i++)
    {
        int x = piece[i].getPoint().getx();
        int y = piece[i].getPoint().gety();
        bool black_0 = piece[i].getColor();
        if(black_0)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        x = x * WIDTH + BORDER_X;
        y = y * WIDTH + BORDER_Y;
        painter.drawEllipse(x - WIDTH / 4,y - WIDTH / 4,WIDTH / 2,WIDTH / 2);
    }
    //画下棋辅助线
    if(movex < SIZE && movex >= 0 && movey < SIZE && movey >= 0)
    {
        int x = movex * WIDTH + BORDER_X;
        int y = movey * WIDTH + BORDER_Y;
        pen.setWidth(3);
        if(black)
            pen.setColor(Qt::black);
        else
            pen.setColor(Qt::white);
        painter.setPen(pen);
        painter.drawLine(x - WIDTH / 2 , y - WIDTH / 2 ,x - WIDTH / 2, y - WIDTH / 3);
        painter.drawLine(x - WIDTH / 2 , y - WIDTH / 2 ,x - WIDTH / 3, y - WIDTH / 2);
        painter.drawLine(x - WIDTH / 2 , y + WIDTH / 2 ,x - WIDTH / 3, y + WIDTH / 2);
        painter.drawLine(x - WIDTH / 2 , y + WIDTH / 2 ,x - WIDTH / 2, y + WIDTH / 3);
        painter.drawLine(x + WIDTH / 2 , y + WIDTH / 2 ,x + WIDTH / 3, y + WIDTH / 2);
        painter.drawLine(x + WIDTH / 2 , y + WIDTH / 2 ,x + WIDTH / 2, y + WIDTH / 3);
        painter.drawLine(x + WIDTH / 2 , y - WIDTH / 2 ,x + WIDTH / 3, y - WIDTH / 2);
        painter.drawLine(x + WIDTH / 2 , y - WIDTH / 2 ,x + WIDTH / 2, y - WIDTH / 3);
    }
}

void NetGamewindow::setserver(bool se)
{
    this->isserver = se;
    if(se)
    {
        Server server;
        ismyturn = true;
    }
    else
    {
        socket->connectToHost("127.0.0.1",8899);
        ismyturn = false;
    }
}

void NetGamewindow::mouseMoveEvent(QMouseEvent *ev)
{
    if(iswin)
    {
        return;
        //qDebug()<<1;
    }
    Point p = {ev->x(),ev->y()};
    p.transform();
    movex = p.getx();
    movey = p.gety();
    update();
}

bool NetGamewindow::Judge_isWin(int x,int y)
{
        //qDebug()<<3;
        // 水平方向

        for (int i = 0; i < 6; i++)
        {
            if (y - i >= 0 && y - i + 5 < SIZE &&
                QiPan[x][y - i] == QiPan[x][y - i + 1] &&
                QiPan[x][y - i] == QiPan[x][y - i + 2] &&
                QiPan[x][y - i] == QiPan[x][y - i + 3] &&
                QiPan[x][y - i] == QiPan[x][y - i + 4] &&
                QiPan[x][y - i] == QiPan[x][y - i + 5]
                    )
                return true;
        }
        // 竖直方向
        for (int i = 0; i < 6; i++)
        {
            if (x - i >= 0 && x - i + 5 < SIZE &&
                QiPan[x - i][y] == QiPan[x - i + 1][y] &&
                QiPan[x - i][y] == QiPan[x - i + 2][y] &&
                QiPan[x - i][y] == QiPan[x - i + 3][y] &&
                QiPan[x - i][y] == QiPan[x - i + 4][y] &&
                QiPan[x - i][y] == QiPan[x - i + 5][y]
                    )
                return true;
        }

        // 左斜方向
        for (int i = 0; i < 6; i++)
        {
            if (x + i < SIZE && x + i - 5 >= 0 &&
                y - i >= 0 && y - i + 5 < SIZE &&
                QiPan[x + i][y - i] == QiPan[x + i - 1][y - i + 1] &&
                QiPan[x + i][y - i] == QiPan[x + i - 2][y - i + 2] &&
                QiPan[x + i][y - i] == QiPan[x + i - 3][y - i + 3] &&
                QiPan[x + i][y - i] == QiPan[x + i - 4][y - i + 4] &&
                QiPan[x + i][y - i] == QiPan[x + i - 5][y - i + 5]
                    )
                return true;
        }

        // 右斜方向
        for (int i = 0; i < 6; i++)
        {
            if (x - i >= 0 && x - i + 5 < SIZE &&
                y - i >= 0 && y - i + 5 < SIZE &&
                QiPan[x - i][y - i] == QiPan[x - i + 1][y - i + 1] &&
                QiPan[x - i][y - i] == QiPan[x - i + 2][y - i + 2] &&
                QiPan[x - i][y - i] == QiPan[x - i + 3][y - i + 3] &&
                QiPan[x - i][y - i] == QiPan[x - i + 4][y - i + 4] &&
                QiPan[x - i][y - i] == QiPan[x - i + 5][y - i + 5]
                    )
                return true;
        }

        return false;
}

bool NetGamewindow::Judge_ban(int x, int y)
{
    //活四1_****__
    //活四2_***_*_
    //活四3_**_**_
    //活四4_*_***_
    //活五_*****_
    //冲五1&*****_
    //冲五2&****_*
    //冲五3&**_***
    //长连1*****x*
    //长连2****x**
    //长连
    // 水平方向
    for (int i = 0; i < 7; i++)
    {
        if (y - i >= 0 && y - i + 6 < SIZE &&
            QiPan[x][y - i] == QiPan[x][y - i + 1] &&
            QiPan[x][y - i] == QiPan[x][y - i + 2] &&
            QiPan[x][y - i] == QiPan[x][y - i + 3] &&
            QiPan[x][y - i] == QiPan[x][y - i + 4] &&
            QiPan[x][y - i] == QiPan[x][y - i + 5] &&
            QiPan[x][y - i] == QiPan[x][y - i + 6]
                )
            return true;
    }
    // 竖直方向
    for (int i = 0; i < 7; i++)
    {
        if (x - i >= 0 && x - i + 6 < SIZE &&
            QiPan[x - i][y] == QiPan[x - i + 1][y] &&
            QiPan[x - i][y] == QiPan[x - i + 2][y] &&
            QiPan[x - i][y] == QiPan[x - i + 3][y] &&
            QiPan[x - i][y] == QiPan[x - i + 4][y] &&
            QiPan[x - i][y] == QiPan[x - i + 5][y] &&
            QiPan[x - i][y] == QiPan[x - i + 6][y]
                )
            return true;
    }
    // 左斜方向
    for (int i = 0; i < 7; i++)
    {
        if (x + i < SIZE && x + i - 6 >= 0 &&
            y - i >= 0 && y - i + 6 < SIZE &&
            QiPan[x + i][y - i] == QiPan[x + i - 1][y - i + 1] &&
            QiPan[x + i][y - i] == QiPan[x + i - 2][y - i + 2] &&
            QiPan[x + i][y - i] == QiPan[x + i - 3][y - i + 3] &&
            QiPan[x + i][y - i] == QiPan[x + i - 4][y - i + 4] &&
            QiPan[x + i][y - i] == QiPan[x + i - 5][y - i + 5] &&
            QiPan[x + i][y - i] == QiPan[x + i - 6][y - i + 6]
                )
            return true;
    }
    // 右斜方向
    for (int i = 0; i < 7; i++)
    {
        if (x - i >= 0 && x - i + 6 < SIZE &&
            y - i >= 0 && y - i + 6 < SIZE &&
            QiPan[x - i][y - i] == QiPan[x - i + 1][y - i + 1] &&
            QiPan[x - i][y - i] == QiPan[x - i + 2][y - i + 2] &&
            QiPan[x - i][y - i] == QiPan[x - i + 3][y - i + 3] &&
            QiPan[x - i][y - i] == QiPan[x - i + 4][y - i + 4] &&
            QiPan[x - i][y - i] == QiPan[x - i + 5][y - i + 5] &&
            QiPan[x - i][y - i] == QiPan[x - i + 6][y - i + 6]
                )
            return true;
    }

    //活四1_****__
    //活四2_***_*_
    //活四3_**_**_
    //活四4_*_***_
    //活四4__****_
    //活五_*****_
    //冲五1&*****_
    //冲五2****_*
    //冲五3**_***
    //冲五4*_****
    //冲五5_*****&

    int four = 0;
    int five = 0;

    // 水平方向 活四
    for (int i = 1; i < 6; i++)
    {
        if (y - i >= 0 && y - i + 6 < SIZE &&
            QiPan[x][y - i + 0] ==  -1 &&
            QiPan[x][y - i + 6] ==  -1
            )
        {
            int bn = 0;
            for(int j = 1;j < 6;j++)
                if(QiPan[x][y - i + j] == 0)
                {
                    bn = 0;
                    break;
                }
                else if(QiPan[x][y - i + j] == 1)
                    bn++;
            if(bn == 4)
            {
                four++;
                break;
            }
        }
    }

    //竖直方向活四
    for (int i = 1; i < 6; i++)
    {
        if (x - i >= 0 && x - i + 6 < SIZE &&
            QiPan[x - i + 0][y] == -1 &&
            QiPan[x - i + 6][y] == -1)
        {
                int bn = 0;
                for(int j = 1;j < 6;j++)
                    if(QiPan[x - i + j][y] == 0)
                    {
                        bn = 0;
                        break;
                    }
                    else if(QiPan[x - i + j][y] == 1)
                        bn++;
                if(bn == 4)
                {
                    four++;
                    break;
                }
        }
    }

    //左斜活四
    for (int i = 1; i < 6; i++)
    {
        if(x + i < SIZE && x + i - 6 >= 0 &&
                y - i >= 0 && y - i + 6 < SIZE &&
                QiPan[x + i - 0][y - i + 0] == -1 &&
                QiPan[x + i - 6][y - i + 6] == -1
           )
        {
                int bn = 0;
                for(int j = 1;j < 6;j++)
                    if(QiPan[x + i - j][y - i + j] == 0)
                    {
                        bn = 0;
                        break;
                    }
                    else if(QiPan[x + i - j][y - i + j] == 1)
                        bn++;
                if(bn == 4)
                {
                    four++;
                    break;
                }
        }
    }

    //右斜活四
    for (int i = 1; i < 6; i++)
    {
        if(x - i >= 0 && x - i + 6 < SIZE &&
                y - i >= 0 && y - i + 6 < SIZE &&
                QiPan[x - i + 0][y - i + 0] == -1 &&
                QiPan[x - i + 6][y - i + 6] == -1
           )
        {
                int bn = 0;
                for(int j = 1;j < 6;j++)
                    if(QiPan[x - i + j][y - i + j] == 0)
                    {
                        bn = 0;
                        break;
                    }
                    else if(QiPan[x - i + j][y - i + j] == 1)
                        bn++;
                if(bn == 4)
                {
                    four++;
                    break;
                }
        }
    }

    qDebug()<<four;

    if(four >= 2)
        return true;

    //竖直
    for (int i = 0; i < 8; i++)
    {
        if (y - i >= -1 && y - i + 6 <= SIZE)
        {
            int bn = 0;
            if(QiPan[x][y - i] == 0 || y - i == -1)
            {
                for(int j = 1;j <= 6;j++)
                    if(QiPan[x][y - i + j] == 0 || y - i + j == SIZE)
                    {
                        bn = 0;
                        break;
                    }
                    else if(QiPan[x][y - i + j] == 1)
                        bn++;
            }
            else if(QiPan[x][y - i + 6] == 0 || y - i + 6 == SIZE)
            {
                 qDebug()<<"88888";
                for(int j = 0;j < 6;j++)
                    if(QiPan[x][y - i + j] == 0)
                    {
                        bn = 0;
                        break;

                    }
                    else if(QiPan[x][y - i + j] == 1)
                        bn++;
            }
            else if(y - i + 6 != SIZE &&
                        y - i != -1 )
            {
                if(QiPan[x][y - i + 6] == -1 &&
                        QiPan[x][y - i] == -1)
                {
                    for(int j = 1;j < 6;j++)
                        if(QiPan[x][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x][y - i + 7] == -1 ||
                        QiPan[x][y - i] == -1)
                {
                    for(int j = 1;j < 7;j++)
                        if(QiPan[x][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x][y - i + j] == 1)
                            bn++;
                }
            }
            if(bn == 5)
            {
                five++;
                break;
            }
        }
    }
    //水平
    for (int i = 0; i < 8; i++)
    {
        if (x - i >= -1 && x - i + 6 <= SIZE)
        {
                int bn = 0;
                if(x - i == -1 || QiPan[x - i][y] == 0)
                {
                    for(int j = 1;j <= 6;j++)
                        if(QiPan[x - i + j][y] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y] == 1)
                            bn++;
                }
                else if(x - i + 6 == SIZE || QiPan[x - i + 6][y] == 0)
                {
                    for(int j = 0;j < 6;j++)
                        if(QiPan[x - i + j][y] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y] == 1)
                            bn++;
                }
                else if(x - i + 6 != SIZE &&
                            x - i != -1 )
                {
                    if(QiPan[x - i + 6][y] == -1 &&
                            QiPan[x - i][y] == -1)
                    {
                        for(int j = 1;j < 6;j++)
                            if(QiPan[x - i + j][y] == 0)
                            {
                                bn = 0;
                                break;
                            }
                            else if(QiPan[x - i + j][y] == 1)
                                bn++;
                    }
                   else if(QiPan[x - i + 7][y] == -1 ||
                           QiPan[x - i][y] == -1)
                    {
                        for(int j = 1;j < 7;j++)
                            if(QiPan[x - i + j][y] == 0)
                            {
                                bn = 0;
                                break;
                            }
                            else if(QiPan[x - i + j][y] == 1)
                                bn++;
                    }
                }
                if(bn == 5)
                {
                    five++;
                    break;
                }
        }
    }
    //右斜
    for (int i = 0; i < 8; i++)
    {
        if(x + i < SIZE && x + i - 6 >= 0 &&
                y - i >= 0 && y - i + 6 < SIZE
           )
        {
                int bn = 0;
                if(QiPan[x + i][y - i] == 0)
                {
                    for(int j = 1;j <= 6;j++)
                        if(QiPan[x + i - j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x + i - j][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x + i - 6][y - i + 6] == 0)
                {
                    for(int j = 0;j < 6;j++)
                        if(QiPan[x + i - j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x + i - j][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x + i - 6][y - i + 6] == -1 &&
                            QiPan[x + i][y - i] == -1)
                {
                    for(int j = 1;j < 6;j++)
                        if(QiPan[x + i - j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x + i - j][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x + i - 7][y - i + 7] == -1 ||
                        QiPan[x + i][y - i] == -1)
                {
                    for(int j = 1;j < 7;j++)
                        if(QiPan[x + i - j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x + i - j][y - i + j] == 1)
                            bn++;
                }
                if(bn == 5)
                {
                    five++;
                    break;
                }
        }
        int bn = 0;
        if(x + i < SIZE && x + i - 6 >= 0 &&
                y - i == -1 && y - i + 6 < SIZE)
        {
            for(int j = 1;j < 7;j++)
                if(QiPan[x + i - j][y - i + j] == 0)
                {
                    bn = 0;
                    break;
                }
                else if(QiPan[x + i - j][y - i + j] == 1)
                    bn++;
        }
        else if(x + i == SIZE && x + i - 6 >= 0 &&
                y - i >= 0 && y - i + 6 < SIZE)
        {
            for(int j = 1;j < 7;j++)
                if(QiPan[x + i - j][y - i + j] == 0)
                {
                    bn = 0;
                    break;
                }
                else if(QiPan[x + i - j][y - i + j] == 1)
                    bn++;
        }
        if(bn == 5)
        {
            five++;
            break;
        }
    }
    //左斜
    for (int i = 0; i < 8; i++)
    {
        if(x - i < SIZE && x - i + 6 >= 0 &&
                y - i >= 0 && y - i + 6 < SIZE
           )
        {
                int bn = 0;
                if(QiPan[x - i][y - i] == 0)
                {
                    for(int j = 1;j <= 6;j++)
                        if(QiPan[x - i + j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x - i + 6][y - i + 6] == 0)
                {
                    for(int j = 0;j < 6;j++)
                        if(QiPan[x - i + j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y - i + j] == 1)
                            bn++;
                }

                else if(QiPan[x - i + 6][y - i + 6] == -1 &&
                            QiPan[x - i][y - i] == -1)
                {
                    for(int j = 1;j < 6;j++)
                        if(QiPan[x - i + j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y - i + j] == 1)
                            bn++;
                }
                else if(QiPan[x - i + 7][y - i + 7] == -1 ||
                        QiPan[x - i][y - i] == -1)
                {
                    for(int j = 1;j < 7;j++)
                        if(QiPan[x - i + j][y - i + j] == 0)
                        {
                            bn = 0;
                            break;
                        }
                        else if(QiPan[x - i + j][y - i + j] == 1)
                            bn++;
                }
                if(bn == 5)
                {
                    five++;
                    break;
                }
        }
        int bn = 0;
        if(x - i < SIZE && x - i - 6 >= 0 &&
                y - i == -1 && y - i + 6 < SIZE)
        {
            for(int j = 1;j < 7;j++)
                if(QiPan[x - i + j][y - i + j] == 0)
                {
                    bn = 0;
                    break;
                }
                else if(QiPan[x - i + j][y - i + j] == 1)
                    bn++;
        }
        if(bn == 5)
        {
            five++;
            break;
        }
    }

    qDebug()<<"five"<<five;
    if(five >= 2)
        return true;

    return false;
}

void NetGamewindow::End()
{

}

void NetGamewindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if(iswin || !ismyturn)
        return;
    Point p = {ev->x(),ev->y()};
    p.transform();
    if(p.getx() < 0 || p.getx() >= SIZE || p.gety() < 0 || p.gety() >= SIZE)
        return;
    if(QiPan[p.getx()][p.gety()] != -1)
        return;
    piece[piecenum] = {p,black};
    if(black)
    QiPan[p.getx()][p.gety()] = 1;
    else
    QiPan[p.getx()][p.gety()] = 0;
    if(black && Judge_ban(p.getx(),p.gety()))
    {
        iswin = true;
        black = !black;
        End();
    }
    if(Judge_isWin(p.getx(),p.gety()))
    {
        movex = -1;
        movey = -1;
        update();
        //
        End();
    }
    black = !black;
    piecenum++;
    if(piecenum == 399)
    {
        full = true;
        update();
        End();
    }
    QString q = "";
    for(int i = 0;i < 20;i++)
        for(int j = 0;j < 20;j++)
            q += (char)(QiPan[i][j] + 4);
    socket->write(q.toLatin1());
    ismyturn = false;
    update();
}
