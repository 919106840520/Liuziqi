#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QMainWindow>

namespace Ui {
class Windialog;
}

class Windialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Windialog(QWidget *parent = 0);
    ~Windialog();
    void setContext(QString);
signals:
    void restart();
    void back();
private:
    Ui::Windialog *ui;
    void Restart();
    void Back();
};

#endif // WINDIALOG_H
