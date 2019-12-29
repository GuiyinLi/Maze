/*
 * @Author: liguiyin
 * @Date: 2019-12-28 16:11:04
 * @LastEditTime: 2019-12-28 16:12:59
 * @Description: README
 * @FilePath: \Maze_V2.0\include\generate.h
 */
#ifndef _generate_h
#define _generate_h
#include "maze.h"

extern int mazeRandMap[MAX][MAX];
extern int tempMaze[MAX][MAX];
extern int MG[MAX][MAX];

//迷宫生成相关函数
void PrimeCreateMaze(int row, int col, int map[MAX][MAX]);
void clearMap(int map[MAX][MAX]);
void copyMazeMap(int mazemap[MAX][MAX],int tomazemap[MAX][MAX]);
int (*mapRand(int i, int j))[MAX];

#endif
