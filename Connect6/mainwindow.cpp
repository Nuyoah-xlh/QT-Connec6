#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>
#include<QPen>
#include<QPaintEvent>
#include<QLabel>
#include<QBitmap>
#include<QPixmap>
#include<QPushButton>
#include<qmath.h>
#include<QBrush>
#include<QDebug>
#include<QMouseEvent>
#include<QMessageBox>
#include <QSound>
#include<QMediaPlayer>
#include<QMenuBar>
#include<QMenu>
#include<QIcon>
#include<qtimer.h>

#define CHESS_PLAY  ":/new/prefix1/sound/chessone.wav"  //落子音的路径
#define WIN_SOUND  ":/new/prefix1/sound/win.wav"   //获胜提示音路径
#define LOSE_SOUND ":/new/prefix1/sound/lose.wav"   //失败提示音路径
#define BACK_SOUND  "://sound/enderdz.wav"   //背景音乐路径

const int chessboard_size=21;  //棋盘线条数
const int boundary=40;  //棋盘距离边缘的距离
const int square_length=40;  //棋盘格子大小
const int mouseOk=20; //鼠标的有效点击距离
const int flag_length=10; //落子标记边长
const int r=17; //棋子半径
const int ai_time=600;  //模拟ai思考时间



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(2*boundary+(chessboard_size-1)*square_length+400,2*boundary+(chessboard_size-1)*square_length);
    this->setWindowIcon(QPixmap(":/new/prefix1/Image/logo.png")); //设置应用图标
    ui->pushButton->setText("双人对决");
    ui->pushButton->setGeometry(1000,200,200,60); //设置按钮位置和尺寸
    ui->pushButton_2->setText("人机对战");
    ui->pushButton_2->setGeometry(1000,400,200,60);
    ui->pushButton_3->setText("智能对决");
    ui->pushButton_3->setGeometry(1000,600,200,60);
    QMenuBar *menubar=menuBar();  //设置菜单栏
    QMenu *menu_1=menubar->addMenu("六子棋"); //设置菜单
    QAction *action_1=menu_1->addAction("规则介绍");  //菜单下的按钮
    QString str=          "规则:\n"
                           "1.对峙双方谁的六个棋子先连在一条线即为胜者。\n"
                           "2.当有四个子连成一条直线时，就应采取防守，堵住四子的一端，否则就会输掉比赛。\n"
                           "3.应当避免在比赛内出现四四禁手、五五禁手等情况，否则就会不小心输掉比赛。\n"
                           "4.关于禁手:指对局中禁止先行一方（黑方）使用的战术，具体包括黑方一子落下时同时形成双四、双五或长连等三种棋形。禁手只对黑方有效，白方无禁手。黑方禁手的位置称为禁手点。";
              //规则介绍
    connect(action_1,QAction::triggered,
            [=]()
            {
                QMessageBox::about(this,"游戏规则",str);
            }
            );   //弹出游戏规则内容
    QAction *action_2=menu_1->addAction("退出游戏");
    connect(action_2,QAction::triggered,this,QMainWindow::close);  //关闭程序
    QMenu *menu_2=menubar->addMenu("了解更多");
    QAction *action_3=menu_2->addAction("关于版本");
    connect(action_3,QAction::triggered,
            [=]()
            {
               QMessageBox::about(this,"版本介绍","版本号：001");
            }
            );
    QAction *action_4=menu_2->addAction("关于作者");
    QString str2="学号：919106840638\n"
                 "Github:...";
    connect(action_4,QAction::triggered,
            [=]()
            {
                QMessageBox::about(this,"关于作者",str2);
            });

    menu_1->addSeparator();  //设置分隔线
    QSound *startsound=new QSound(BACK_SOUND,this);
    startsound->play();//播放背景音乐
    startsound->setLoops(-1); //设置单曲循环
    initgame();  //默认初始化

}

MainWindow::~MainWindow()
{
    delete ui;
    if(game)   //撤销游戏指针
    {
        delete game;
        game = nullptr;
    }
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //画家
    painter.drawPixmap(0,0,this->width(),this->height(),QPixmap("../image/background.png"));
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿,防止图像走样
    QPen pen; //画笔
    pen.setWidth(2);  //画笔的线条宽度
    pen.setColor(Qt::black);
    painter.setPen(pen);  //将画笔交给画家
    for(int i=0;i<chessboard_size;i++)
    {
        painter.drawLine(boundary,boundary+i*square_length,boundary+(chessboard_size-1)*square_length,boundary+i*square_length);  //画横线
        painter.drawLine(boundary+i*square_length,boundary,boundary+i*square_length,boundary+(chessboard_size-1)*square_length);//画竖线

    }
    update();
    QBrush brush; //笔刷
    brush.setStyle(Qt::SolidPattern);
    if(clickPosX>=0&&clickPosX<chessboard_size&&clickPosY>=0&&clickPosY<chessboard_size&&game->state==1&&game->board[clickPosX][clickPosY]==0)  //范围内有效，绘制鼠标将要落子的标记点
    {
        if(game->player)
        {
            brush.setColor(Qt::black);
            painter.setBrush(brush);
            painter.drawRect(boundary + square_length * clickPosX - flag_length / 2, boundary + square_length * clickPosY  - flag_length / 2, flag_length, flag_length);
        }
        else if(game->player==false&&game_type!='r')
        {
            brush.setColor(Qt::white);
            painter.setBrush(brush);
            painter.drawRect(boundary + square_length * clickPosX - flag_length / 2, boundary + square_length * clickPosY  - flag_length / 2, flag_length, flag_length);
        }

    }
    //根据数组情况绘制黑白棋子
    for(int i=0;i<chessboard_size;i++)
    {
        for(int j=0;j<chessboard_size;j++)
        {

            if(game->board[i][j]==1&&game->state==1) //游戏进行中才允许绘制棋子
           {
              brush.setColor(Qt::black);
              painter.setBrush(brush);
              painter.drawEllipse(QPoint(boundary + square_length * i ,boundary + square_length * j),r,r);
           }
            else if(game->board[i][j]==-1&&game->state==1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint(boundary + square_length * i ,boundary + square_length * j),r,r);
            }

        }


    }


}




void MainWindow::initgame()   //初始化
{
    if(game)
    {
        game=new Game;
    }


}
void MainWindow::init(char type)
{
    game->game_type=type;
    game->state=1;
    game->startgame(type);
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //this->setMouseTracking(true);
    int x=event->x();
    int y=event->y();
    //qDebug()<<x<<" "<<y;
    //保证鼠标在有效范围内,且棋盘边缘不落子
    if (x >= boundary-mouseOk  &&
            x <= boundary+(chessboard_size-1)*square_length+mouseOk  &&
            y >= boundary-mouseOk  &&
            y <= boundary+(chessboard_size-1)*square_length+mouseOk&&game->state==1)
    {

        // 获取距离最近的左上角的点
        int row = (x) / square_length;
        int col =( y) / square_length;


        int leftTopPosX = boundary + square_length * row;
        int leftTopPosY = boundary + square_length * col;
        //qDebug()<<leftTopPosX<<" "<<leftTopPosY;
        // 根据距离算出合适的点击位置,一共四个点，根据两点距离选最近的
        clickPosX = -1; // 初始化
        clickPosY = -1;
        int len = 0;

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < mouseOk)  //在有效点范围内
        {
            clickPosX = row;
            clickPosY = col;
        }
        len = sqrt((x - leftTopPosX - square_length) * (x - leftTopPosX - square_length) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < mouseOk)
        {
            clickPosX = row+1;
            clickPosY = col;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - square_length) * (y - leftTopPosY - square_length));
        if (len < mouseOk)
        {
            clickPosX = row;
            clickPosY = col+1;
        }
        len = sqrt((x - leftTopPosX - square_length) * (x - leftTopPosX - square_length) + (y - leftTopPosY - square_length) * (y - leftTopPosY - square_length));
        if (len < mouseOk)
        {
            clickPosX = row + 1;
            clickPosY = col + 1;
        }

    }
    //qDebug()<<clickPosX<<" "<<clickPosY;

    // 存了坐标后更新界面
    update();

}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{


      if (clickPosX != -1 && clickPosY != -1&&game->state==1)
      {

          if(game_type=='s'||game->player==true)
          {
              playByPerson();  //人执行
          }
           if(game_type=='r'&&game->player==false)  //ai操作
          {
              QTimer::singleShot(ai_time, this, SLOT(playByAI()));
          }
      }
      if(clickPosX>=0&&clickPosX<chessboard_size&&clickPosY>=0&&clickPosY<chessboard_size&&game->board[clickPosX][clickPosY]!=0) //判断游戏输赢
      {
          if(game->board[clickPosX][clickPosY]==1&&game->isJinShou(clickPosX,clickPosY)&&game->state==1)  //判断黑棋落子是否禁手
          {
              QMessageBox::StandardButton standar=QMessageBox::information(this,"游戏结束！","禁手！ White-player win！\n 点击Ok返回主界面！",QMessageBox::Ok);
              if(standar==QMessageBox::Ok)
              {
                  game->state=0;
                  game->clear();
                  ui->pushButton->setText("双人对决");
                  ui->pushButton_2->setText("人机对决");
              }
          }
          else if(game->isWin(clickPosX,clickPosY)==true&&game->state==1)  //弹出提示框
          {

              QSound::play(WIN_SOUND);  //获胜提示音
              QString string;
              if(game->board[clickPosX][clickPosY]==1)
              {
                  string="Black-player win!";   //黑棋胜
              }
              else
              {
                  string="White-player win!";  //白棋胜
              }
              QMessageBox::StandardButton standarbutton=QMessageBox::information(this," 游戏结束！",string+"\n"+" 点击 Ok 返回主界面!",QMessageBox::Ok);
              if(standarbutton==QMessageBox::Ok)
              {
                  game->state=0;
                  game->clear();
                  ui->pushButton->setText("双人对决");
              }
          }
          else if(game->isHeQi()&&game->state==1)
          {
              game->state=0;
              QMessageBox::StandardButton standarbutton=QMessageBox::information(this,"游戏结束！","该局和棋啦！\n 点击Ok返回主界面",QMessageBox::Ok);
              if(standarbutton==QMessageBox::Ok)
              {
                  game->state=0;
                  game->clear();
                  ui->pushButton->setText("双人对决");
              }
          }
      }

     //人操作
       //qDebug()<<clickPosX<<" "<<clickPosY<<endl;
       //qDebug()<<game->board[clickPosX*chessboard_size+clickPosY];



}
void MainWindow::playByPerson()
{

    if (clickPosX != -1 && clickPosY != -1 && game->board[clickPosX][clickPosY] == 0)
    {
        game->actionByPerson(clickPosX,clickPosY); //人进行下棋
        QSound::play(CHESS_PLAY);  //设置落子提示音
        update(); //更新界面
    }

}
void MainWindow::playByAI()
{
    game->actionByAI(clickPosX,clickPosY);
    QSound::play(CHESS_PLAY);
    update();
}




void MainWindow::on_pushButton_clicked()
{

     ui->centralWidget->setMouseTracking(true); //激活鼠标追踪
     setMouseTracking(true);  //激活整个窗体的鼠标追踪
     ui->pushButton->setMouseTracking(true); //进入某个按钮时，鼠标追踪属性失效，因此我们也需要激活该按钮的鼠标追踪功能
     game_type='s';  //双人模式类型
     init(game_type);  //初始化
     ui->pushButton->setText("重新开始:双人对决");
     //qDebug()<<"开始";
     update();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->centralWidget->setMouseTracking(true); //激活鼠标追踪
    setMouseTracking(true);  //激活整个窗体的鼠标追踪
    ui->pushButton_2->setMouseTracking(true); //进入某个按钮时，鼠标追踪属性失效，因此我们也需要激活该按钮的鼠标追踪功能
    game_type='r';
    init(game_type);
    ui->pushButton_2->setText("重新开始：人机对决");
    update();
}
