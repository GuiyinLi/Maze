/*
 * @Author: liguiyin
 * @Date: 2019-11-22 10:01:27
 * @LastEditTime : 2019-12-28 22:27:37
 * @Description: README
 * @FilePath: \Schtb\source\main.c
 */
#include "Init.h"
#include "maze.h"
#include "stack.h"
#include "queue.h"
#include "generate.h"
#include "windows.h"
//暂时不用
void gotoxy(int x, int y)      //光标移动到（x,y）
{
    COORD coord;
    coord.X = y;
    coord.Y = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int main()
{
    mazeInfoPtr maze;
    PrintInit();
    if(!GetInput())
    {
        system("pause");
        return 0;
    }
    do{
        getchar();
        if(!TestMode)
        {
            MazeInit(&maze);
            printTip((char *)"求解迷宫图");
            MazePrint(*maze);
            printTip((char *)"深度优先求单条路径");
            MazePath(maze,maze->route[0]);
            if(GETRANDMAZE)
                copyMazeMap(tempMaze,mazeRandMap);
            else
                copyMazeMap(MG,mazeRandMap);
            printTip((char *)"广度优先求最短路径");
            ShortestPath(maze,maze->route[1]);
            printTip((char *)"递归法求多条路径");
            MazeAllPath(maze);
            free(maze);
        }
        else
            TimeTest();
        if(!conOperate())
            break;
        PrintInit();
    }while(GetInput());
    system("pause");
    return 0;
}
