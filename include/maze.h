/*
 * @Author: liguiyin
 * @Date: 2019-11-22 11:17:37
 * @LastEditTime : 2019-12-28 20:54:40
 * @Description: README
 * @FilePath: \Maze\include\schtb.h
 */
#ifndef _maze_h
#define _maze_h
#include "config.h"

#define MAX 400
#define WALLRATE 0.30f   //随机迷宫中墙所占比例

extern int GETRANDMAZE; //0->规定迷宫 1->随机迷宫
extern int MAXROW ;     //迷宫宽度 80*80
extern int MAXCOL;      //迷宫长度
extern int TestCount;   //用于测试时间复杂度的样例数量

#define MAXPRINT 20
#define getMazeMap() mapRand(MAXROW,MAXCOL)
#define MAXROUTE 1000
#define START_POS(start) start.row=1,start.col=1
#define END_POS(end)     end.row=MAXROW-2,end.col=MAXCOL-2
#define ACCESS  -1   //原本可通
#define WALL    0    //墙壁
#define GONE    -2  //走过并且不通(原来是-1)

#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define YELLOW FOREGROUND_GREEN|FOREGROUND_RED
#define PURPLE FOREGROUND_RED|FOREGROUND_BLUE
#define CYAN FOREGROUND_GREEN|FOREGROUND_BLUE
#define DEFAULT 7

typedef int order;  //序号类型
//位置信息
typedef struct{
    int row;    //行坐标 0-MAXROW
    int col;    //列坐标 0-MAXCOL
} posType;

//方向信息
typedef enum
{
    null = 0,
    east = 1,   //东 col+1
    south = 2,  //南 row+1
    west = 3,   //西 col-1
    north = 4,   //北 row-1
	error
} directionType;

//通道块信息
typedef struct{
    order ord;          //通道块在路径上的序号
    posType seat;       //通道块在迷宫中的坐标信息
    directionType di;   //从此通道块走向下一通道块的方向
} CBlockType, *CBlockTypePtr;

//堆栈信息
typedef CBlockType SElemType;
//堆栈采用链式存储结构
typedef struct StackNode{
    SElemType elem;
    struct StackNode *next;
} stackNode, *stackNodePtr;
typedef struct LinkStack{
    stackNodePtr top; 	//栈顶指针 始终指向栈顶元素
    int step;          //记录总步数
} linkStack,*linkStackPtr;

//队列信息
typedef CBlockType QElemType;
typedef struct{
    QElemType *base; //动态数组
    int front, rear;
    int step;
    int size;
} Queue;

//路径信息
typedef linkStackPtr RouteInfoPtr;
typedef linkStack RouteInfo;
typedef Queue RouteShortest;
//迷宫信息
typedef struct{
    posType start;                  //起点坐标
    posType end;                    //终点坐标
    int (*mazeMap)[MAX];         //迷宫地图常量指针(用于指向常量 指向的值不可改变，但指向的地址可以改变)
    RouteInfoPtr route[MAXROUTE+1]; //存储所有路径
    RouteInfoPtr routeshortest;    //存储一条最短路径
    int routeNum;                   //路径数目 0表示没有通路
} mazeInfo,*mazeInfoPtr;

extern int TestMode;  //1-->时间复杂度比较 0-->普通迷宫求解

//相关初始化函数
Status MazeInit(mazeInfoPtr *maze);
CBlockType CBlockInit(void);
void setCBlockInfo(CBlockType *CB, order ord, posType seat, directionType di);
//相关打印函数
void MazePrint(mazeInfo maze);
void PathPrint(RouteInfo r);
Status MazeMapPrint(mazeInfo maze, RouteInfo r);
//非递归求解单条路径相关函数(链栈实现)
void FootPrint(posType pos, int curstep, int mazeMap[MAX][MAX]);
void MarkPrint(posType pos, int mazeMap[MAX][MAX]);
posType NextPos(posType pos, directionType di);
Status Pass(posType pos, int mazeMap[MAX][MAX]);
Status MazePath(mazeInfoPtr maze, RouteInfoPtr r);
//递归求解所有路径相关函数
Status MazeAllPath(mazeInfoPtr maze);
void FindPath(mazeInfoPtr maze);
//递归回溯法求解单条路径相关函数
void FindSimpleRoute(mazeInfoPtr maze,RouteInfoPtr r);
Status MazeSimmplePath(mazeInfoPtr maze, RouteInfoPtr r);
//广度优先求解最短路径(队列实现)
Status ShortestPath(mazeInfoPtr maze, RouteInfoPtr r);
//时间复杂度比较相关函数
int getSearchTime(mazeInfoPtr maze, RouteInfoPtr r, Status (*SearchRoute)(mazeInfoPtr maze, RouteInfoPtr r));
void TimeTest(void);
//C++directionType类型数据处理相关函数
directionType operator++(directionType &di,int);    //自增运算符重载函数
directionType Transfrom(int d);

#endif
