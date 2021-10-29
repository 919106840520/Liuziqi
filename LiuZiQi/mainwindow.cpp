#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QtGui"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowIcon(QIcon(":/resource/title.jpg"));
    ui->setupUi(this);
    this->setWindowTitle("六子棋@author黄子进");
    QPixmap pixmap(":/resource/Liuziqi.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setFixedSize(500,300);
    connect(ui->pvpbtn,&QPushButton::clicked,this,&MainWindow::pvp);
    connect(ui->pvebtn,&QPushButton::clicked,this,&MainWindow::pve);
    connect(ui->evebtn,&QPushButton::clicked,this,&MainWindow::eve);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pvp()
{
    this->close();
    gw = new Gamewindow;
    connect(gw,&Gamewindow::back,this,&MainWindow::back);
    gw->setpvp(true);
    gw->seteve(false);
    gw->show();
}

void MainWindow::pve()
{
    this->close();
    gw = new Gamewindow;
    connect(gw,&Gamewindow::back,this,&MainWindow::back);
    gw->setpvp(false);
    gw->seteve(false);
    int a = QMessageBox::question(this,"先手","请问你是否先手",QMessageBox::Yes | QMessageBox::No);
    if(a == QMessageBox::Yes)
        gw->setFirst(true);
    else
        gw->setFirst(false);
    gw->show();
}

void MainWindow::eve()
{
    this->close();
    gw = new Gamewindow;
    connect(gw,&Gamewindow::back,this,&MainWindow::back);
    gw->setpvp(false);
    gw->seteve(true);
    gw->show();
}

void MainWindow::back()
{
    gw->close();
    disconnect(gw,&Gamewindow::back,this,&MainWindow::back);
    if(gw != NULL)
        delete gw;
    this->show();
}
