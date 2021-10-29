#ifndef NETGAMEWINDOW_H
#define NETGAMEWINDOW_H

#include <QMainWindow>
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
#include "server.h"

namespace Ui {
class NetGamewindow;
}

class NetGamewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetGamewindow(QWidget *parent = 0);
    ~NetGamewindow();
    const int BORDER_X = 20;
    const int BORDER_Y = 20;
    const int SIZE = 20;
    const int WIDTH = 40;
    void initgame();
    bool isserver;
    void setserver(bool se);
    void mouseReleaseEvent(QMouseEvent *ev);
    bool Judge_ban(int x, int y);
    void End();
    bool Judge_isWin(int x,int y);

private:
    Ui::NetGamewindow *ui;
    int QiPan[20][20];
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QTcpSocket *socket;
    bool ismyturn;
    bool iswin;
    bool black;
    bool eve;
    int score[20][20];
    Piece piece[450];
    int piecenum;
    int movex,movey;
    bool full;
};

#endif // NETGAMEWINDOW_H
