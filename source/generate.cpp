/*
 * @Author: liguiyin
 * @Date: 2019-12-28 16:08:36
 * @LastEditTime : 2019-12-28 16:19:42
 * @Description: README
 * @FilePath: \Maze_V2.0\source\generate.c++
 */
#include "maze.h"
#include "time.h"
#include <iostream>
#include<vector>
using namespace std;
//迷宫数组 0->障碍 -1->通过
int mazeRandMap[MAX][MAX] = {0};
int MG[MAX][MAX] =
    {
        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {0, -1, -1,  0, -1, -1, -1,  0, -1,  0},
        {0, -1, -1,  0, -1, -1, -1,  0, -1,  0},
        {0, -1, -1, -1, -1,  0,  0, -1, -1,  0},
        {0, -1,  0,  0,  0, -1, -1, -1, -1,  0},
        {0, -1, -1, -1,  0, -1, -1, -1, -1,  0},
        {0, -1,  0, -1, -1, -1,  0, -1, -1,  0},
        {0, -1,  0,  0,  0, -1,  0,  0, -1,  0},
        {0,  0, -1, -1, -1, -1, -1, -1, -1,  0},
        {0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};
//缓存迷宫信息
int tempMaze[MAX][MAX] ={0};
/**
 * @brief:        迷宫复制函数
 * @param[in]:    原迷宫 目的迷宫
 * @return:       null
 * @note:         null
 */
void copyMazeMap(int mazemap[MAX][MAX],int tomazemap[MAX][MAX])
{
    int i,j;
    for(i=0;i<MAXROW;i++)
    {
        for(j=0;j<MAXCOL;j++)
        {
            tomazemap[i][j]=mazemap[i][j];
        }
    }
}
/**
 * @brief:        迷宫地图清空函数
 * @param[in]:    map
 * @return:       null
 * @note:         null
 */
void clearMap(int map[MAX][MAX])
{
    for (int i = 0;i<MAX;i++)
        for (int j = 0; j < MAX;j++)
            map[i][j] = WALL;
}

/**
 * @brief:        随机迷宫生成函数
 * @param[in]:    row col
 * @return:       迷宫二维数组指针
 * @note:         百分之三十是墙
 */
int (*mapRand(int i,int j))[MAX]
{
    int total;//墙的总数
    int m,n;//控制循环变量
    float scale=WALLRATE;//墙的比例,百分之四十
    int count=0;//计数器，在对迷宫比例进行局部调整的时候使用
    time_t t;//设定时间种子
    srand((unsigned)time(&t));

    for(m=0;m<i;m++)
        for(n=0;n<j;n++)
            mazeRandMap[m][n]=rand()%10;
    for(m=0;m<i;m++)//0代表墙，-1代表路
        for(n=0;n<j;n++)
            if(mazeRandMap[m][n]<5)
                mazeRandMap[m][n]=0;
            else
                mazeRandMap[m][n]=-1;
    //控制墙和路的比例
    for(n=0;n<j;n++)
    {
        mazeRandMap[0][n]=0;
        mazeRandMap[i-1][n]=0;
    }
    for(m=1;m<i-1;m++)
    {
        mazeRandMap[m][0]=0;
        mazeRandMap[m][j-1]=0;
    }
    //将外围置为墙，防止出界
    for(m=1;m<i-1;m++)
        for(n=1;n<j-1;n++)
            if(mazeRandMap[m][n]==0)
                count++;
    //统计墙的个数

    total=(int)(i-2)*(j-2)*scale;
    while(count!=total)
    if(count<total)
    {
        m=rand()%(i-2)+1;
        n=rand()%(j-2)+1;
        if(mazeRandMap[m][n]==-1)
        {
            mazeRandMap[m][n]=0;
            count++;
        }
    }
    else
    {
       m=rand()%(i-2)+1;
       n=rand()%(j-2)+1;
       if(mazeRandMap[m][n]==0)
       {
         mazeRandMap[m][n]=-1;
         count--;
       }
    }
    mazeRandMap[1][1] = -1;     //起点和终点必须可通
    mazeRandMap[i - 2][j - 2] = -1;
    mazeRandMap[1][2] = -1;
    mazeRandMap[2][1] = -1;
    mazeRandMap[i - 3][j - 2] = -1;
    mazeRandMap[i - 2][j - 3] = -1;
    //对迷宫进行局部随机调整，使墙和路的比例合乎规定
    return mazeRandMap;
}
/**
 * @brief:        Prime算法生成随机迷宫
 * @param[in]:    ROW COL
 * @return:       二维数组指针
 * @note:         能够生成具有唯一路径的迷宫，用于时间复杂度的测试
 * 相比于DFS BFS 递归分割，其生成的迷宫越大，越自然，路径越复杂，故选用此算法
 */

void PrimeCreateMaze(int row,int col,int map[MAX][MAX])
{
	int Maze[MAX+2][MAX+2] = { WALL };
    int ROW = row + 2;
    int COL = col + 2;
    clearMap(map);
    for (int i = 0; i < ROW; i++)
    {
		Maze[i][0] = ACCESS;
		Maze[i][COL - 1] = ACCESS;
	}
    for (int j = 0; j < COL;j++)
    {
        Maze[0][j] = ACCESS;
        Maze[ROW - 1][j] = ACCESS;
    }

    vector<int> X;
	vector<int> Y;

	X.push_back(2);
	Y.push_back(2);
    srand((unsigned int)(time(NULL)));
	while (X.size())
    {
		int r = rand() % X.size();
		int x = X[r];
		int y = Y[r];

		int count = 0;
		for (int i = x - 1; i < x + 2; i++)
        {
			for (int j = y - 1; j < y + 2; j++)
            {
				if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == ACCESS)
                {
					++count;
				}
			}
		}

		if (count <= 1)
        {
			Maze[x][y] = ACCESS;
            map[x - 1][y - 1] = ACCESS;
			for (int i = x - 1; i < x + 2; i++)
            {
				for (int j = y - 1; j < y + 2; j++)
                {
					if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == 0)
                    {
						X.push_back(i);
						Y.push_back(j);
					}
				}
			}
		}

		X.erase(X.begin() + r);
		Y.erase(Y.begin() + r);
	}
    Maze[2][1] = ACCESS;
    for (int i = ROW - 3; i >= 2; i--) {
		if (Maze[i][COL - 3] == ACCESS) {
            for (int j = i; j <= ROW - 3;j++)
            {
                Maze[j][COL - 3] = ACCESS;
                map[j - 1][COL - 4] = ACCESS;
            }
            break;
		}
	}
}
