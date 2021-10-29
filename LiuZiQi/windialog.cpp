#include "windialog.h"
#include "ui_windialog.h"

Windialog::Windialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Windialog)
{
    ui->setupUi(this);
    this->setWindowTitle("提示");
    this->setFixedSize(100,100);
    connect(ui->back,&QPushButton::clicked,this,&Windialog::Back);
    connect(ui->restart,&QPushButton::clicked,this,&Windialog::Restart);
}

Windialog::~Windialog()
{
    delete ui;
}

void Windialog::setContext(QString str)
{
    this->ui->win->setText(str);
    this->ui->win->setAlignment(Qt::AlignCenter);
}

void Windialog::Restart()
{
    this->close();
    emit restart();
}

void Windialog::Back()
{
    this->close();
    emit back();
}
