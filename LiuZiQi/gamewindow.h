#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "QPaintEvent"
#include "qpainter.h"
#include "QMouseEvent"
#include "piece.h"
#include "qdebug.h"
#include "qpalette.h"
#include "QBrush"
#include "QPen"
#include "windialog.h"
#include "QTime"
#include "qtimer.h"

namespace Ui {
class Gamewindow;
}

class Gamewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gamewindow(QWidget *parent = 0);
    const int BORDER_X = 20;
    const int BORDER_Y = 20;
    const int SIZE = 20;
    const int WIDTH = 40;
    void setpvp(bool pvp);
    void seteve(bool eve);
    void End();
    ~Gamewindow();

private:
    Ui::Gamewindow *ui;
    bool iswin;
    bool ispvp;
    bool black;
    bool eve;
    int QiPan[20][20];
    int score[20][20];
    Piece piece[450];
    int piecenum;
    int movex,movey;
    int stepn;
    bool aiturn;
    bool full;

    void aiscore();
    void timerEvent(QTimerEvent *e);
signals:
    void back();

public slots:
    void setFirst(bool );
    void restart();
    void initgame();
    bool Judge_ban(int x,int y);
    bool Judge_isWin(int x,int y);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void aimove();
};

#endif // GAMEWINDOW_H
