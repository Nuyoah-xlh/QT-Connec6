#include "Game.h"
#include"mainwindow.h"
#include <utility>
#include <stdlib.h>
#include <time.h>
#include<QDebug>
#include<QApplication>
#include<iostream>
#include<vector>
#include<QTimer>

using namespace std;

const int chessboard_size=21;  //棋盘尺寸

Game::Game()
{
   black=1;
   white=-1;
   state=0;
}

void Game::startgame(char type)
{
    clear();
    player=true;
    if(type=='r'||type=='z')
    {
        clear2();
    }
}

void Game::clear()
{
    for(int i=0;i<chessboard_size;i++)
    {
        for(int j=0;j<chessboard_size;j++)
        {
            board[i][j]=0;  //数组初始化置0
        }
    }
}
void Game::updateMap(int x,int y)
{
    if(player)  //根据下棋方填充数组
    {
        board[x][y]=1;
    }
    else
    {
         board[x][y]=-1;
    }
    player=!player;


}
void Game::actionByPerson(int x, int y)
{
    updateMap(x,y); //重绘
}
void Game::actionByAI(int &clickX,int &clickY)
{
    getScore();  //计算分值
    //记录最大值的位置，若有多个点，记录最后一个搜索到的点
    int maxScore = 0;
    vector<pair<int, int>> maxPoints;   //使用vector容器放最大值所在位置

    for (int row = 1; row < chessboard_size; row++)
        for (int col = 1; col < chessboard_size; col++)
        {
            // 前提是这个坐标是空的
            if (board[row][col] == 0)
            {
                if (score[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();       //发现新的最大值，清空容器内之前存储的所有元素
                    maxScore = score[row][col];
                    maxPoints.push_back(make_pair(row, col));  //放入最新的最大值坐标
                }
                else if (score[row][col] == maxScore)     // 如果有多个最大的数，都放到容器中
                    maxPoints.push_back(make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));   //随机种子
    int index = rand() % maxPoints.size();  //随机下标，范围在容器容量内

    pair<int, int> pointPair = maxPoints.at(index);  //取出该位置元素
    clickX = pointPair.first;
    clickY = pointPair.second;    //赋值

    updateMap(clickX,clickY);  //更新棋盘数组
}

void Game::playEVE(int &clickx,int &clicky)  //ai对战ai
{
      getScore();  //计分

      actionByAI(clickx,clicky);  //落子

}

bool Game::isWin(int clickX,int clickY)  //判断游戏输赢
{

    for (int i = 0; i < 6; i++)
    {
         if (clickY - i >= 0 &&
            clickY - i + 5 < chessboard_size &&
            board[clickX][clickY - i] == board[clickX][clickY - i + 1] &&
            board[clickX][clickY - i] == board[clickX][clickY - i + 2] &&
            board[clickX][clickY - i] == board[clickX][clickY - i + 3] &&
            board[clickX][clickY - i] == board[clickX][clickY - i + 4]&&
            board[clickX][clickY-i]==board[clickX][clickY-i+5])  //竖直方向
         {
           return true;
         }
         else if (clickX - i >= 0 &&
             clickX - i + 5 < chessboard_size &&
             board[clickX - i][clickY] == board[clickX - i + 1][clickY] &&
             board[clickX - i][clickY] == board[clickX - i + 2][clickY] &&
             board[clickX - i][clickY] == board[clickX - i + 3][clickY] &&
             board[clickX - i][clickY] == board[clickX - i + 4][clickY]&&
             board[clickX - i][clickY] == board[clickX - i + 5][clickY])  //水平方向
         {
             return true;
         }
         else if(clickX+i<chessboard_size&&clickX+i-5>=0&&clickY-i>=0&&clickY-i+5<chessboard_size&&
                 board[clickX+i][clickY-i]==board[clickX+i-1][clickY-i+1]&&
                 board[clickX+i][clickY-i]==board[clickX+i-2][clickY-i+2]&&
                 board[clickX+i][clickY-i]==board[clickX+i-3][clickY-i+3]&&
                 board[clickX+i][clickY-i]==board[clickX+i-4][clickY-i+4]&&
                 board[clickX+i][clickY-i]==board[clickX+i-5][clickY-i+5])  //斜向左下
         {
             return true;
         }
         else if (clickX - i >= 0 &&
                  clickX - i + 5 < chessboard_size &&
                  clickY - i >= 0 &&
                  clickY - i + 5 < chessboard_size &&
                  board[clickX - i][clickY - i] == board[clickX - i + 1][clickY - i + 1] &&
                  board[clickX - i][clickY - i] == board[clickX - i + 2][clickY - i + 2] &&
                  board[clickX - i][clickY - i] == board[clickX - i + 3][clickY - i + 3] &&
                  board[clickX - i][clickY - i] == board[clickX - i + 4][clickY - i + 4]&&
                  board[clickX - i][clickY - i] == board[clickX - i + 5][clickY - i + 5]) //斜向右下
              {
                  return true;
              }

    }
    return false;
}
bool Game::isHeQi()
{
    //棋盘全部占满棋子则和棋，有空位则不是和棋
    for(int i=0;i<chessboard_size;i++)
    {
        for(int j=0;j<chessboard_size;j++)
        {
            if(board[i][j]==0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Game::isJinShou(int x,int y)  //发生四四禁手和五五禁手都属于禁手
{
    int num=0,k=0;
    k=0;
    //向上
    for(int i=0;i<6;i++)
    {
        if(y+i<chessboard_size)
        {
            if(board[x][y+i]==1)
            {
                k++;
            }
            else if(board[x][y+i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    //qDebug()<<num;
    if(k>=4)
    {
        num++;
    }
    k=0;
    if(num>=2)
    {
        return true;
    }
    //向下
    for(int i=0;i<6;i++)
    {
        if(y-i>=0)
        {
            if(board[x][y-i]==1)
            {
                k++;
            }
            else if(board[x][y-i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向左
    for(int i=0;i<6;i++)
    {
        if(x-i>=0)
        {
            if(board[x-i][y]==1)
            {
                k++;
            }
            else if(board[x-i][y]==white)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向右

    for(int i=0;i<6;i++)
    {
        if(x+i<chessboard_size)
        {
            if(board[x+i][y]==1)
            {
                k++;
            }
            else if(board[x+i][y]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向左上
    for(int i=0;i<6;i++)
    {
        if(x-i>=0&&y-i>=0)
        {
            if(board[x-i][y-i]==1)
            {
                k++;
            }
            else if(board[x-i][y-i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向左下
    for(int i=0;i<6;i++)
    {
        if(x-i>=0&&y+i<chessboard_size)
        {
            if(board[x-i][y+i]==1)
            {
                k++;
            }
            else if(board[x-i][y+i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向右上
    for(int i=0;i<6;i++)
    {
        if(x+i<chessboard_size&&y-i>=0)
        {
            if(board[x+i][y-i]==1)
            {
                k++;
            }
            else if(board[x+i][y-i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    k=0;
    //向右下
    for(int i=0;i<6;i++)
    {
        if(x+i<chessboard_size&&y+i<chessboard_size)
        {
            if(board[x+i][y+i]==1)
            {
                k++;
            }
            else if(board[x+i][y+i]==-1)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(k>=4)
    {
        num++;
    }
    if(num>=2)
    {
        return true;
    }
    return false;


}
//活五》死五》活四》活三》死四》活二》死三》死二》其他
//分别求各位置两种棋子的加权值，取最大值位置为最佳落子点
void Game::getScore()
{
    int value=0;
    int value2=0;
    int flag=-1;   //白子
    int flag2=1;    //黑子
    clear2();
    for(int clickx=0;clickx<chessboard_size;clickx++)
    {
        for(int clicky=0;clicky<chessboard_size;clicky++)
        {
            if(board[clickx][clicky]!=0) continue;
            value=0;
            value2=0;

            for(int i=-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    if(clickx+6*i>=0&&clickx+6*i<chessboard_size&&clicky+6*j>=0&&clicky+6*j<chessboard_size&&clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==flag&&board[clickx+i*5][clicky+j*5]==flag&&board[clickx+i*6][clicky+j*6]==0) //活五
                    {
                        value+=600000;
                    }
                    else if(clickx+6*i>=0&&clickx+6*i<chessboard_size&&clicky+6*j>=0&&clicky+6*j<chessboard_size&&clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==flag&&board[clickx+i*5][clicky+j*5]&&board[clickx+i*6][clicky+j*6]==1)   //死五
                    {
                        value+=500000;
                    }
                    else if(clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==flag&&board[clickx+i*5][clicky+j*5]==0)  //活四
                    {
                        value+=400000;
                    }
                    else if(clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==flag&&board[clickx+i*5][clicky+j*5]==1)  //死四
                    {
                        value+=250000;
                    }
                    else if(clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==0)  //活三
                     {
                         value+=300000;
                     }
                    else if(clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==flag
                            &&board[clickx+i*4][clicky+j*4]==1)  //死三
                    {
                        value+=150000;
                    }
                    else if(clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==0)  //活二
                    {
                        value+=200000;
                    }
                    else if(clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag
                            &&board[clickx+i*2][clicky+j*2]==flag&&board[clickx+i*3][clicky+j*3]==1)  //死二
                    {
                        value+=100000;
                    }
                    else if(clickx+1*i>=0&&clickx+1*i<chessboard_size&&clicky+1*j>=0&&clicky+j*1<chessboard_size&&board[clickx+1*i][clicky+1*j]==flag)  //一个棋子
                    {
                        value+=2000;
                    }
                    else  //其他
                    {
                        value+=1000;
                    }
                }
            }

            for(int i=-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    if(clickx+6*i>=0&&clickx+6*i<chessboard_size&&clicky+6*j>=0&&clicky+6*j<chessboard_size&&clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==flag2&&board[clickx+i*5][clicky+j*5]==flag2&&board[clickx+i*6][clicky+j*6]==0) //活五
                    {
                        value2+=600000;
                    }
                    else if(clickx+6*i>=0&&clickx+6*i<chessboard_size&&clicky+6*j>=0&&clicky+6*j<chessboard_size&&clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==flag2&&board[clickx+i*5][clicky+j*5]==flag2&&board[clickx+i*6][clicky+j*6]==-1)   //死五
                    {
                        value2+=500000;
                    }
                    else if(clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==flag2&&board[clickx+i*5][clicky+j*5]==0)  //活四
                    {
                        value2+=400000;
                    }
                    else if(clickx+5*i>=0&&clicky+5*j>=0&&clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+5*i<chessboard_size&&clicky+5*j<chessboard_size&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==flag2&&board[clickx+i*5][clicky+j*5]==-1)  //死四
                    {
                        value2+=250000;
                    }
                    else if(clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==0)  //活三
                     {
                         value2+=300000;
                     }
                    else if(clickx+4*i>=0&&clicky+4*j>=0
                            &&clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+4*i<chessboard_size&&clicky+4*j<chessboard_size
                            &&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==flag2
                            &&board[clickx+i*4][clicky+j*4]==-1)  //死三
                    {
                        value2+=150000;
                    }
                    else if(clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==0)  //活二
                    {
                        value2+=200000;
                    }
                    else if(clickx+3*i>=0&&clicky+3*j>=0&&clickx+2*i>=0&&clicky+2*j>=0&&clickx+i>=0
                            &&clicky+j>=0&&clickx+3*i<chessboard_size&&clicky+3*j<chessboard_size&&clickx+2*i<chessboard_size&&clicky+2*j<chessboard_size&&clickx+i<chessboard_size
                            &&clicky<chessboard_size&&board[clickx+i*1][clicky+j*1]==flag2
                            &&board[clickx+i*2][clicky+j*2]==flag2&&board[clickx+i*3][clicky+j*3]==-1)  //死二
                    {
                        value2+=100000;
                    }
                    else if(clickx+1*i>=0&&clickx+1*i<chessboard_size&&clicky+1*j>=0&&clicky+j*1<chessboard_size&&board[clickx+1*i][clicky+1*j]==flag2)  //一个棋子
                    {
                        value2+=2000;
                    }
                    else        //其他
                    {
                        value2+=1000;
                    }
                }
            }

            score[clickx][clicky]=value>value2?value:value2;
        }
    }
}

void Game::clear2()
{
    for(int i=0;i<chessboard_size;i++)
    {
        for(int j=0;j<chessboard_size;j++)
        {
            score[i][j]=0;

        }
    }
}
