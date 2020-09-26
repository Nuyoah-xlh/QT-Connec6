#include "mainpage.h"
#include<QPainter>
#include<QIcon>
#include<QPaintEvent>
#include<QSound>

#define BACK ":/new/prefix1/image/1.png"
#define BUTTON_PLAY ":/new/prefix1/sound/button.wav"   //按钮提示音

mainpage::mainpage(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Connect Six   六子棋");
    this->setWindowIcon(QIcon(":/new/prefix1/Image/logo.png")); //设置应用图标
    this->setFixedSize(1280,880);
    button=new QPushButton(this);
    button->move(QPoint(550,600));
    button->setFlat(true);  //透明效果
    button->setFixedSize(300,70);  //设定按钮尺寸
    button->setFont(QFont("微软雅黑",20,700));
    button->setText("进入游戏");
    connect(button,&QPushButton::clicked,this,&mainpage::sendslot);  //连接信号

}
void mainpage::paintEvent(QPaintEvent *event)
{
    if(button->underMouse())
    {
        button->setFlat(false);
    }
    else
    {
        button->setFlat(true);
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿,防止图像走样
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap(BACK));  //背景图片
    painter.drawPixmap(450,100,500,300,QPixmap(":/new/prefix1/image/title.png"));

}
void mainpage::sendslot()
{
    QSound::play(BUTTON_PLAY);
    emit mysignal();   //发出信号，等待主窗口接收
}

