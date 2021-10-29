#include "gamewindow.h"
#include "ui_gamewindow.h"

Gamewindow::Gamewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gamewindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resource/title.jpg"));
    setMouseTracking(true);
    //this->setFixedSize(BORDER_X * 2 + SIZE * WIDTH - WIDTH,BORDER_X * 2 + SIZE * WIDTH - WIDTH);//不希望大小被改变
    initgame();
    ui->p1->setIcon(QIcon(":/resource/0079e8315b5797124c8ae7c040558a2c.jpg"));
    ui->p2->setIcon(QIcon(":/resource/0f53fbc3176aeab8a9f2c49ef116bf12.jpg"));
    connect(ui->back,&QPushButton::clicked,[=](){
        emit back();
    });
    connect(ui->reset,&QPushButton::clicked,this,&Gamewindow::restart);
}

void Gamewindow::setpvp(bool pvp)
{
    ispvp = pvp;
    if(ispvp)
        this->setWindowTitle("人人对局");
    else
        this->setWindowTitle("人机对局");
}

void Gamewindow::setFirst(bool a)
{
    if(a)
        return;
    else
    {

        aimove();
    }
}

void Gamewindow::seteve(bool ev)
{
    this->eve = ev;
    if(eve)
    {
        this->setWindowTitle("机机对局");
        aiturn = true;
        this->startTimer(100);
    }
}

void Gamewindow::timerEvent(QTimerEvent *e)
{
    if(!iswin)
    aimove();
}

void Gamewindow::End()
{
    iswin = true;
    Windialog *wd = new Windialog;
    connect(wd,&Windialog::restart,this,&Gamewindow::restart);
    if(black)
        wd->setContext("黑棋获胜");
    else
        wd->setContext("白棋获胜");
    if(full)
        wd->setContext("平局");
    wd->show();
}

void Gamewindow::initgame()
{
    aiturn = false;
    stepn = 0;
    iswin = false;
    this->setFixedSize(BORDER_X * 2 + SIZE * WIDTH + 4 * WIDTH,BORDER_X * 2 + SIZE * WIDTH - WIDTH);//不希望大小被改变
    movex = -1;
    movey = -1;
    piecenum = 0;
    black = true;
    full = false;
    for(int i = 0;i < SIZE;i++)
    {
        for(int j = 0;j < SIZE;j++)
        {
            QiPan[i][j] = -1;
        }
    }
    update();
}

void Gamewindow::restart()
{
    initgame();
}

Gamewindow::~Gamewindow()
{
    delete ui;
}

bool Gamewindow::Judge_isWin(int x,int y)
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

bool Gamewindow::Judge_ban(int x, int y)
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

void Gamewindow::aiscore()
{
    int empty = 0;
    int ppiece = 0;
    int epiece = 0;
    int pcolor = 0;
    if(black)
        pcolor = 1;
    else
        pcolor = 0;

    for(int  i = 0;i < SIZE;i++)
        for(int j = 0;j < SIZE;j++)
            score[i][j] = 0;
    for(int  i = 0;i < SIZE;i++)
        for(int j = 0;j < SIZE;j++)
        {
            if(QiPan[i][j] != -1)
                continue;
            QiPan[i][j] = 1;
            if(black && Judge_ban(i,j))
                continue;
            QiPan[i][j] = -1;
            for(int dx = -1;dx <= 1;dx++)
            {
                for(int dy = -1;dy <= 1;dy++)
                {
                    empty = 0;
                    ppiece = 0;
                    epiece = 0;
                    if(dx == 0 && dy == 0)
                        continue;
                    for(int t = 1;t <= 5;t++)
                    {
                        if(i + dx * t >= 0 && i + dx * t < SIZE
                                && j + dy * t >= 0 && j + dy * t < SIZE)
                        {
                            int temp = QiPan[i + dx * t][j + dy * t];
                            if(temp == !pcolor)
                                ppiece++;
                            else if(temp == pcolor)
                                break;
                            else if(temp == 0)
                            {
                                empty++;
                                break;
                            }
                        }
                        else
                            break;
                    }
                    for(int t = 1;t <= 5;t++)
                    {
                        if(i - dx * t >= 0 && i - dx * t < SIZE
                                && j - dy * t >= 0 && j - dy * t < SIZE)
                        {
                            int temp = QiPan[i - dx * t][j - dy * t];
                            if(temp == !pcolor)
                                ppiece++;
                            else if(temp == pcolor)
                                break;
                            else if(temp == -1)
                            {
                                empty++;
                                break;
                            }
                        }
                        else
                            break;
                    }

                    if(ppiece == 1)
                        score[i][j] += 1;
                    if(ppiece == 1 && empty == 2)
                        score[i][j] += 19;
                    else if(ppiece == 2)
                    {
                        if(empty == 1)
                            score[i][j] += 30;
                        else if(empty == 2)
                            score[i][j] += 120;
                    }
                    else if(ppiece == 3)
                    {
                        if(empty == 1)
                            score[i][j] += 200;
                        else if(empty == 2)
                            score[i][j] += 720;
                    }
                    else if(ppiece == 4)
                    {
                        if(empty == 1)
                        score[i][j] += 3200;
                        else if(empty == 2)
                        score[i][j] += 6400;
                    }
                    else if(ppiece >= 5)
                    {
                        score[i][j] += 40000;
                    }
                   empty = 0;

                   for(int t = 1;t <= 5;t++)
                   {
                       if(i + dx * t >= 0 && i + dx * t < SIZE
                               && j + dy * t >= 0 && j + dy * t < SIZE)
                       {
                           int temp = QiPan[i + dx * t][j + dy * t];
                           if(temp == pcolor)
                               epiece++;
                           else if(temp == !pcolor)
                               break;
                           else if(temp == -1)
                           {
                               empty++;
                               break;
                           }
                       }
                       else
                           break;
                   }
                   for(int t = 1;t <= 5;t++)
                   {
                       if(i - dx * t >= 0 && i - dx * t < SIZE
                               && j - dy * t >= 0 && j - dy * t < SIZE)
                       {
                           int temp = QiPan[i - dx * t][j - dy * t];
                           if(temp == pcolor)
                               epiece++;
                           else if(temp == !pcolor)
                               break;
                           else if(temp == -1)
                           {
                               empty++;
                               break;
                           }
                       }
                       else
                           break;
                   }

                   if(epiece == 1)
                   {
                       score[i][j] += 1;
                       if(empty == 2)
                       score[i][j] += 20;
                   }
                   else if(epiece == 2)
                   {
                       if(empty == 1)
                           score[i][j] += 25;
                       else if(empty == 2)
                           score[i][j] += 130;
                   }
                   else if(epiece == 3)
                   {
                       if(empty == 1)
                           score[i][j] += 190;
                       else if(empty == 2)
                           score[i][j] += 500;
                   }
                   else if(epiece == 4)
                   {
                       if(empty == 1)
                           score[i][j] += 1000;
                       else if(empty == 2)
                       score[i][j] += 3000;
                   }
                   else if(epiece >= 5)
                       score[i][j] += 80000;
                }
            }
        }
}

void Gamewindow::aimove()
{
    if(iswin)
        return;
    if(stepn == 0)
    {
        //step[stepn] = {SIZE / 2,SIZE / 2};
        if(black)
            QiPan[SIZE / 2][SIZE / 2] = 1;
        else
            QiPan[SIZE / 2][SIZE / 2] = 0;
        piece[piecenum] = {SIZE / 2,SIZE / 2,black};
        piecenum++;
        stepn++;
        black = !black;
        return;
    }
    aiscore();
    int maxlistn = 1;
    QPoint maxlist[SIZE * SIZE];
    maxlist[0] = {0,0};

    int maxscore = 0;
    for(int i = 0;i < SIZE;i++)
    {
        for(int j = 0;j < SIZE;j++)
        {
            if(QiPan[i][j] != -1)
                continue;
            if(score[i][j] > maxscore)
            {
                maxlist[0] = {i,j};
                maxlistn = 1;
                maxscore = score[i][j];
            }
            else if(score[i][j] == maxscore)
            {
                maxlist[maxlistn] = {i,j};
                maxlistn++;
            }
        }
    }
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    int r = qrand() % maxlistn;
    if(black)
        QiPan[maxlist[r].x()][maxlist[r].y()] = 1;
    else
        QiPan[maxlist[r].x()][maxlist[r].y()] = 0;
    //laststep = {maxlist[r].x(),maxlist[r].y()};
    piece[piecenum] = {maxlist[r].x(),maxlist[r].y(),black};
    piecenum++;
    qDebug("ai下棋中");
    qDebug()<<maxlist[r].x()<<maxlist[r].y()<<maxscore;
    //qDebug()<<QiPan[maxlist[r].x()][maxlist[r].y()];
    update();
    //step[stepn] = {maxlist[r].x(),maxlist[r].y()};
    stepn++;
    if(Judge_isWin(maxlist[r].x(),maxlist[r].y()))
    {
        iswin = true;
        End();
        return;
    }
    if(piecenum == 399)
    {
        full = true;
        //update();
        End();
    }
    black = !black;
    if(!eve)
    aiturn = false;
}

void Gamewindow::paintEvent(QPaintEvent *ev)
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
        if(black && !aiturn)
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

void Gamewindow::mouseMoveEvent(QMouseEvent *ev)
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

void Gamewindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if(iswin || aiturn)
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
    stepn++;
    piecenum++;
    if(piecenum == 399)
    {
        full = true;
        update();
        End();
    }
    if(!ispvp)
    {
        aiturn = true;
        QTimer::singleShot(1000,this,SLOT(aimove()));
    }
    update();
}
