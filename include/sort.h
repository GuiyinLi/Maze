/*
 * @Author: liguiyin
 * @Date: 2019-12-28 19:15:16
 * @LastEditTime : 2019-12-29 00:05:34
 * @Description: README
 * @FilePath: \Maze_V2.0\include\sort.h
 */
#ifndef _sort_h
#define _sort_h
#include "maze.h"
typedef int KeyType;
typedef RouteInfoPtr SeaList ;

typedef enum
{
    descending = 0, //½µÐò
    ascending = 1   //ÉýÐò
} sortOrder;
int Partition(SeaList S[], int low, int high, sortOrder sortorder);
void QSort(SeaList S[] ,int low, int high, sortOrder sortorder);
#endif
