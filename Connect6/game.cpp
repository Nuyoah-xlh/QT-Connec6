#include "Game.h"
#include"mainwindow.h"
#include <utility>
#include <stdlib.h>
#include <time.h>
#include<QDebug>
#include<QApplication>
#include<iostream>
#include<vector>

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
    if(type=='r')
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
        QCoreApplication::processEvents();
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
    toScore();  //计算分值
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
void Game::toScore()
{
    int person_num=0; //人的可连成子的棋子个数
    int ai_num=0; //ai的可连成子的棋子个数
    int empty_num=0;  //空白棋子的个数

    clear2();  //清空积分数组
    //成米字形8个方向遍历,且为完全遍历,可以通过改变某些情况的分值来调整ai的棋力
    for(int clickx=0;clickx<chessboard_size;clickx++)
    {
        for(int clicky=0;clicky<chessboard_size;clicky++)
        {
            if(board[clickx][clicky]==0)  //只计算空白点
            {
                //用i,j控制正反方向
                for(int i=-1;i<=1;i++)
                {
                   for(int j=-1;j<=1;j++)
                   {
                       person_num=0;
                       ai_num=0;
                       empty_num=0;
                       if(i==0&&j==0)
                       {
                           continue;
                       }
                       //对人的棋子进行计算
                       for(int t=1;t<6;t++)
                       {
                           if(clickx+i*t>0&&clickx+i*t<chessboard_size&&clicky+i*t>0&&clicky+i*t<chessboard_size&&board[clickx+i*t][clicky+i*t]==1) //人的棋子
                           {
                               person_num++;
                           }
                           else if(clickx+i*t>0&&clickx+i*t<chessboard_size&&clicky+i*t>0&&clicky+i*t<chessboard_size&&board[clickx+i*t][clicky+i*t]==0) //空白
                           {
                               empty_num++;
                           }
                           else
                           {
                               break;
                           }
                       }
                       if (person_num == 1)                      // 杀二
                           score[clickx][clicky] += 20;
                       else if (person_num == 2)                 // 杀三
                       {
                           if (empty_num == 1)
                               score[clickx][clicky] += 35;
                           else if (empty_num == 2)
                               score[clickx][clicky] += 45;
                           else if(empty_num == 3)
                           {
                               score[clickx][clicky]+=55;
                           }
                       }
                       else if (person_num == 3)                 // 杀四
                       {

                           if (empty_num == 1)
                               score[clickx][clicky] += 65;
                           else if (empty_num == 2)
                               score[clickx][clicky] += 115;
                        }
                       else if (person_num == 4)                 // 杀五
                       {
                           if(empty_num==1)
                           score[clickx][clicky]+=10100;
                       }
                       else if(person_num==5)    //杀六
                       {
                           score[clickx][clicky]+=20200;
                       }

                       //对ai计分
                       empty_num=0;
                       for(int t=1;t<6;t++)
                       {
                           if(clickx+i*t>0&&clickx+i*t<chessboard_size&&clicky+i*t>0&&clicky+i*t<chessboard_size&&board[clickx+i*t][clicky+i*t]==-1) //ai的棋子
                           {
                               ai_num++;
                           }
                           else if(clickx+i*t>0&&clickx+i*t<chessboard_size&&clicky+i*t>0&&clicky+i*t<chessboard_size&&board[clickx+i*t][clicky+i*t]==0) //空白
                           {
                               empty_num++;
                           }
                           else
                           {
                               break;
                           }
                       }
                       if (ai_num == 0)
                       {
                                                                // 普通下子
                           score[clickx][clicky] += 5;
                       }
                       else if (ai_num == 1)
                       {
                           score[clickx][clicky] += 10;
                       }
                       else if (ai_num == 2)
                       {
                           if (empty_num == 1)
                               score[clickx][clicky] += 25;
                           else if (empty_num == 2)
                               score[clickx][clicky] += 50;
                           else  if(empty_num==3)
                           {
                               score[clickx][clicky]+=75;
                           }
                       }
                       else if (ai_num == 3)
                       {
                           if (empty_num == 1)
                               score[clickx][clicky] += 80;
                           else if (empty_num == 2)
                               score[clickx][clicky] += 125;
                       }
                       else if (ai_num == 4)
                           score[clickx][clicky] += 10000;
                       else if(ai_num==5)
                       {
                           score[clickx][clicky]+=25000;
                       }
                   }
                }
            }
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
