#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QWidget>
#include"Game.h"
#include<QMouseEvent>
#include<QEvent>
#include"mainpage.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Game *game; //游戏指针
    QTimer *time1;  //定时器
    void paintEvent(QPaintEvent *); //绘图
    void mouseReleaseEvent(QMouseEvent *event);  //鼠标点击
    void mouseMoveEvent(QMouseEvent *event);
    char game_style;  //游戏类型
    int clickPosX, clickPosY; //鼠标将点击位置
    void initgame(); //初始化游戏
    char game_type; //游戏类型
    void init(char type);//初始化PVP
    void start_EVE();
    QPushButton pushButton; //人人
    QPushButton pushButton2;  //人机
    QPushButton pushButton3;  //机机
    void isEnd();  //判断游戏是否结束



private slots:
    void playByPerson();  //人下棋
    void playByAI();  //AI下棋
    void flag();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void myslot();


public slots:
    void topage();
private:
    Ui::MainWindow *ui;
    mainpage page;
    QPushButton  *back;
};

#endif // MAINWINDOW_H
