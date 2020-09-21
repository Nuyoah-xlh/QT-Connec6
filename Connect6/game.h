#ifndef GAME_H
#define GAME_H

//#include <Gmae>

class Game
{

public:
    Game();
    char game_type; //游戏类型
    int state; //游戏状态，1为正在进行，0为未进行
    bool player; //下棋方
    void clear();  //清理棋盘
    bool isWin(int x,int y);//判断是否输赢
    bool isHeQi();  //判断是否和棋
    bool isJinShou(int x,int y); //判断是否禁手
    void startgame(char type);  //开始游戏
    void updateMap(int x, int y); //更新棋盘

    void actionByPerson(int x,int  y); //人进行操作
    void getScore();
    int board[21][21];  //数组记录棋盘棋子，空白为0，白子为-1，黑子为1
    int score[21][21];  //数组记录棋盘落子得分
    int score2[21][21];  //数组记录第二个得分情况
    void clear2();  //清空score数组
    int white,black;
    void playEVE(int &clickx,int &clicky);
    void actionByAI(int &clickX,int &clickY); //AI操作
    void actionByAI_2(int &clickx,int &clicky);  //ai二号棋手

};

#endif // GAME_H
