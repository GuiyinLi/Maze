/*
 * @Author: liguiyin
 * @Date: 2019-12-20 15:36:18
 * @LastEditTime: 2020-04-09 15:59:13
 * @Description: README
 * @FilePath: \Maze_V3.0\source\maze.cpp
 */
#include "maze.h"
#include "generate.h"
#include "queue.h"
#include "sort.h"
#include "stack.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"

int TestMode = 0;
int TestCount = 0;
int compare = 0;
/**
 * @brief:        初始化通道块
 * @param[in]:    CBlockTypePtr
 * @return:       Status
 * @note:         null
 */
CBlockType CBlockInit(void)
{
    CBlockType CB;
    CB.di = null;
    CB.ord = 0;
    CB.seat.col = 0;
    CB.seat.row = 0;
    return CB;
}
/**
 * @brief:        迷宫信息初始化
 * @param[in]:    mazeInfoPtr
 * @return:       Status
 * @note:         default_start=(1,1) default_end=(MAXROW-2,MAXCOL-2)
 */
Status MazeInit(mazeInfoPtr *maze)
{
    int i;
    *maze = (mazeInfoPtr)malloc(sizeof(mazeInfo));
    if (maze)
    {
        for (i = 0; i < (MAXROUTE + 1); i++)
            if (InitStack(&(*maze)->route[i]) != OK)
                return OVERFLOW;
        if (InitStack(&(*maze)->routeshortest) != OK)
            return OVERFLOW;
        START_POS((*maze)->start);
        END_POS((*maze)->end);
        if (TestMode)
            PrimeCreateMaze(MAXROW, MAXCOL, mazeRandMap);
        else if (!GETRANDMAZE)
            copyMazeMap(MG, mazeRandMap);
        else
            getMazeMap();
        (*maze)->mazeMap = mazeRandMap;
        copyMazeMap(mazeRandMap, tempMaze);
        (*maze)->routeNum = 0; //默认没有通路
        return OK;
    }
    else
    {
        printError((char *)"内存分配失败");
        return OVERFLOW;
    }
}
/**
 * @brief:        设置通道块信息
 * @param[in]:    通道块结构体
 * @return:       null
 * @note:         null
 */
void setCBlockInfo(CBlockType *CB, order ord, posType seat, directionType di)
{
    CB->ord = ord;
    CB->seat = seat;
    CB->di = di;
}
/**
 * @brief:        打印输出迷宫(原始迷宫打印)
 * @param[in]:    mazeInfo
 * @return:       null
 * @note:         "■"表示障碍，"□"表示无障碍，"○"表示起点，"●"表示终点，
 *                "☆"表示走过但是不通 "★"表示正确路径
 */
void MazePrint(mazeInfo maze)
{
    int i, j;
    printf("\n");
    for (i = 0; i < MAXROW; i++)
    {
        for (j = 0; j < MAXCOL; j++)
        {
            if (i == maze.start.row && j == maze.start.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                printf("○");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (i == maze.end.row && j == maze.end.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                printf("●");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (maze.mazeMap[i][j] == WALL)
                printf("■");
            else if (maze.mazeMap[i][j] == ACCESS)
                printf("□");
            else if (maze.mazeMap[i][j] == GONE)
                printf("☆");
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
                printf("★");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
        }
        printf("\n");
    }
    printf("\n");
}
/**
 * @brief:        打印最终路径
 * @param[in]:    mazeInfo
 * @return:       null
 * @note:         以三元组形式打印输出
 */
void PathPrint(RouteInfo r)
{
    int i = 0;
    stackNodePtr p;
    char d[5] = {'X', 'E', 'S', 'W', 'N'}; //东南西北
    p = r.top;
    while (r.top->next != NULL)
    {
        printf("(%2d,%2d, %c)->", p->elem.seat.row, p->elem.seat.col, d[p->elem.di]);
        r.top = r.top->next;
        p = r.top;
        if ((++i) % 5 == 0)
            printf("\n");
    }
    printf("(%2d,%2d, %c)\nAll %3d Step\n\n", p->elem.seat.row, p->elem.seat.col, d[p->elem.di], r.step);
}
/**
 * @brief:        根据路径打印迷宫地图
 * @param[in]:    mazeInfo RouteInfo
 * @return:       Status
 * @note:         可验证路径的正确性
 */
Status MazeMapPrint(mazeInfo maze, RouteInfo r)
{
    int i, j;
    int tempMap[MAX][MAX] = {0};
    int end = 5;
    int step = 1;
    char dir[4][5] = {"→\0", "↓\0", "←\0", "↑\0"};
    if (r.top->elem.seat.row != maze.start.row || r.top->elem.seat.col != maze.start.col) //起点不符
    {
        printf("此路径无法走通迷宫\n");
        return ERROR;
    }
    while (r.top->next) //先用二维数组存放正确路线
    {
        tempMap[r.top->elem.seat.row][r.top->elem.seat.col] = r.top->elem.di;
        r.top = r.top->next;
    }
    tempMap[r.top->elem.seat.row][r.top->elem.seat.col] = end; //终点用5表示
    printf("\n");
    for (i = 0; i < MAXROW; i++)
    {
        for (j = 0; j < MAXCOL; j++)
        {
            if (i == maze.start.row && j == maze.start.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
                printf("○");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                continue;
            }
            else if (i == maze.end.row && j == maze.end.col)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
                printf("●");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                if (tempMap[i][j] != end) //二者终点不符
                {
                    printf("此路径无法走通迷宫\n");
                    return ERROR;
                }
                continue;
            }
            else if (maze.mazeMap[i][j] == WALL)
            {
                if (tempMap[i][j] > 0) //在障碍处出现路径->此路径无法走通迷宫
                {
                    printf("此路径无法走通迷宫\n");
                    return ERROR;
                }
                printf("■");
            }
            else if (maze.mazeMap[i][j] == ACCESS)
            {
                if (tempMap[i][j] > 0) //无障碍处可出现路径
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
                    printf("%s", &dir[tempMap[i][j] - 1][0]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
                }
                else
                    printf("□");
            }
            else if (maze.mazeMap[i][j] == GONE)
                printf("☆");
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
                printf("%s", &dir[tempMap[i][j] - 1][0]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
            }
        }
        printf("\n");
    }
    printf("\n");
    return OK;
}
/**
 * @brief:        留下足迹
 * @param[in]:    posType curstep mazeMap
 * @return:       null
 * @note:         当前步数大于0
 */
void FootPrint(posType pos, int curstep, int mazeMap[MAX][MAX])
{
    mazeMap[pos.row][pos.col] = curstep;
}
/**
 * @brief:        标记为不可行
 * @param[in]:    posType mazeMap
 * @return:       null
 * @note:         留下不能通过的标记
 */
void MarkPrint(posType pos, int mazeMap[MAX][MAX])
{
    mazeMap[pos.row][pos.col] = GONE; //将走不通的块标记为GONE
}
/**
 * @brief:        下一个位置
 * @param[in]:    posType directionType
 * @return:       OK INFEASLIBLE
 * @note:         改变坐标值 代表移动
 */
posType NextPos(posType pos, directionType di)
{
    switch (di)
    {
    case east: //向东
        pos.col++;
        break;
    case south: //向南
        pos.row++;
        break;
    case west: //向西
        pos.col--;
        break;
    case north: //向北
        pos.row--;
        break;
    }
    return pos;
}
/**
 * @brief:        判断当前是否可通
 * @param[in]:    posType mazeMap
 * @return:       TRUE->可通 FALSE->不可通
 * @note:         前提是未曾走过并且可通的路径
 */
Status Pass(posType pos, int mazeMap[MAX][MAX])
{
    if (mazeMap[pos.row][pos.col] == ACCESS)
        return TRUE;
    else
        return FALSE;
}
/**
 * @brief:        迷宫求解算法
 * @param[in]:    mazeInfoPtr RouteInfoPtr
 * @return:       Status
 * @note:         采用堆栈进行求解(回溯法)，并不是最优解 会改变原迷宫数组
 */
Status MazePath(mazeInfoPtr maze, RouteInfoPtr r)
{
    linkStackPtr S;             //存放路径
    posType curpos;             //当前位置
    SElemType e = CBlockInit(); //通道块缓存变量
    clearStack(r);              //清空原路线
    int curstep;                //当前步数
    InitStack(&S);
    curpos = maze->start; //设定“当前位置”为“入口位置”
    curstep = 1;          //探索第一步
    if (!TestMode)
    {
        printf("\n起点:(%d,%d)->", maze->start.row, maze->start.col);
        printf("终点:(%d,%d)\n", maze->end.row, maze->end.col);
    }
    do
    {
        if (Pass(curpos, maze->mazeMap))
        {                                              //当前位置可以通过，即是未曾走到过的通道块
            FootPrint(curpos, curstep, maze->mazeMap); //留下足迹
            setCBlockInfo(&e, curstep, curpos, east);
            Push(S, e); //加入路径
            if (curpos.row == maze->end.row && curpos.col == maze->end.col)
            { //到达终点
                maze->routeNum = 1;
                S->top->elem.di = null;
                while (!StackIsEmpty(*S)) //将路径存入到mazeInfo中
                {
                    Pop(S, &e);
                    Push(r, e);
                }
                if (!TestMode)
                {
                    MazeMapPrint(*maze, *r);
                    PathPrint(*r);
                }
                destoryStack(S);
                return TRUE;
            }
            curpos = NextPos(curpos, east); //下一位置是当前位置的东邻
            compare++;
            curstep++; //探索下一步
        }              //if
        else
        { //当前位置不能通过
            if (!StackIsEmpty(*S))
            {
                Pop(S, &e);
                while (e.di == north && !StackIsEmpty(*S))
                {
                    MarkPrint(e.seat, maze->mazeMap);
                    Pop(S, &e);
                    curstep--;
                } //while
                if (e.di < north)
                {
                    compare++;
                    e.di++;
                    Push(S, e);
                    curpos = NextPos(e.seat, e.di); //设定当前位置是该新方向上的相邻块
                }                                   //if
            }                                       //if
        }                                           //else
    } while (!StackIsEmpty(*S));
    maze->routeNum = 0;
    printf("此图为死局！\n");
    printf("\n");
    return FALSE;
}
//以下变量用于递归
linkStackPtr S;
posType curpos;
order curstep;
SElemType e;
/**
 * @brief:        递归求所有/单条路径
 * @param[in]:    mazeInfoPtr mode(0-->simple 1-->all)
 * @return:       Status
 * @note:         基于回溯法 不会改变原迷宫数组
 */
Status MazeAllPath(mazeInfoPtr maze)
{
    clearStack(maze->route[0]);
    maze->routeNum = 0; //路径初始化
    InitStack(&S);
    curpos = maze->start; //设定“当前位置”为“入口位置”
    curstep = 1;          //探索第一步

    printf("\n起点:(%d,%d)->", maze->start.row, maze->start.col);
    printf("终点:(%d,%d)\n", maze->end.row, maze->end.col);
    //递归查找路径函数
    FindPath(maze);
    //最多储存上限MAXROUTE
    if (maze->routeNum)
    {
        printf("\n----------------------------共%d条路径----------------------------\n", maze->routeNum);
        QSort(maze->route, 1, maze->routeNum, ascending); //将所有路径从短到长排列
        printf("---------------------------- 最短路径 ----------------------------\n");
        MazeMapPrint(*maze, *maze->route[1]);
        PathPrint(*maze->route[1]);
        printf("---------------------------- 最长路径 ----------------------------\n");
        MazeMapPrint(*maze, *maze->route[maze->routeNum]);
        PathPrint(*maze->route[maze->routeNum]);
        return TRUE;
    }
    else
    {
        printf("抱歉，未找到一条从起点到终点的路径\n");
        return FALSE;
    }
    printf("\n");
}
/**
 * @brief:        递归求解子函数
 * @param[in]:    mazeInfoPtr
 * @return:       null
 * @note:         递归求解
 */
void FindPath(mazeInfoPtr maze)
{
    posType pos;             //临时储存试探位置
    directionType di = east; //默认从东开始试探
    static int i = 0;
    if (curpos.row == maze->end.row && curpos.col == maze->end.col)
    {
        //到达终点
        if (maze->routeNum < MAXROUTE)
        {
            maze->routeNum++;
            setCBlockInfo(&e, curstep, curpos, null); //将终点信息打包到通道信息
            Push(S, e);                               //将终点信息压栈
            //将当前路径存到maze中
            StackCopy(*S, &(maze->route[maze->routeNum]));
            if (maze->routeNum <= MAXPRINT)
            {
                //输出图、路径信息
                printf("----------------------------第%d条路径----------------------------\n", maze->routeNum);
                MazePrint(*maze);
                PathPrint(*maze->route[maze->routeNum]);
            }
            Pop(S, &e); //将终点信息弹出
        }
        curpos = getStackTop(*S).seat; //返回上一个位置
        curstep--;
        return;
    }
    //将当前位置信息打包压栈
    //留下足迹
    //方向暂定为东
    setCBlockInfo(&e, curstep, curpos, di);
    Push(S, e);
    FootPrint(curpos, curstep, maze->mazeMap);
    //找四个方向
    for (di; di <= north; di++)
    {
        //试探位置
        if (maze->routeNum > MAXROUTE)
            return;
        i++;
        if (i % 3000000 == 0)
        {
            i = 0;
            printf(". ");
        }
        pos = NextPos(curpos, di);
        //判断是否可行
        if (Pass(pos, maze->mazeMap))
        {
            if (di != east)
            {
                Pop(S, &e);
                e.di = di;
                Push(S, e);
            }
            curstep++; //记步数
            //更新位置
            curpos.col = pos.col;
            curpos.row = pos.row;
            //递归调用
            FindPath(maze);
        } //if
    }     //for
    //将已走过的路径重置为可通过
    //弹栈
    maze->mazeMap[curpos.row][curpos.col] = -1;
    Pop(S, &e);
    curstep--; //退位置，减步数
    //堆栈不为空更新位置
    if (!StackIsEmpty(*S))
    {
        curpos = getStackTop(*S).seat;
    }
}
/**
 * @brief:        递归求单条路径
 * @param[in]:    mazeInfoPtr mode(0-->simple 1-->all)
 * @return:       Status
 * @note:         基于回溯法 不会改变原迷宫数组
 */
Status MazeSimmplePath(mazeInfoPtr maze, RouteInfoPtr r)
{
    clearStack(r);        //清空路线原来存储的信息
    InitStack(&S);        //初始化用于递归的缓存链栈
    maze->routeNum = 0;   //路径数目清零
    curpos = maze->start; //设定“当前位置”为“入口位置”
    curstep = 1;          //探索第一步
    if (!TestMode)
    {
        printf("\n起点:(%d,%d)->", maze->start.row, maze->start.col);
        printf("终点:(%d,%d)\n", maze->end.row, maze->end.col);
    }
    FindSimpleRoute(maze, r);
    if (maze->routeNum)
    {
        if (!TestMode)
        {
            MazeMapPrint(*maze, *r);
            PathPrint(*r);
        }
        if (GETRANDMAZE)
            copyMazeMap(tempMaze, mazeRandMap);
        else
            copyMazeMap(MG, mazeRandMap);
        return TRUE;
    }
    else
    {
        printf("此迷宫为死局\n");
        return FALSE;
    }
}
/**
 * @brief:        递归求解单条路径
 * @param[in]:    mazeInfoPtr 存放路线
 * @return:       null
 * @note:         此处使用堆栈的目的是将路径信息保存到maze的链栈中
 */
void FindSimpleRoute(mazeInfoPtr maze, RouteInfoPtr r)
{
    posType pos;             //临时储存试探位置
    directionType di = east; //默认从东开始试探
    if (curpos.row == maze->end.row && curpos.col == maze->end.col)
    {
        //到达终点
        maze->routeNum++;
        setCBlockInfo(&e, curstep, curpos, null); //将终点信息打包到通道信息
        Push(S, e);                               //将终点信息压栈
        //将当前路径存到maze中
        StackCopy(*S, &r);
        Pop(S, &e);                    //将终点信息弹出
        curpos = getStackTop(*S).seat; //返回上一个位置
        curstep--;
        return;
    }
    //将当前位置信息打包压栈
    //留下足迹
    //方向暂定为东
    setCBlockInfo(&e, curstep, curpos, di);
    Push(S, e);
    FootPrint(curpos, curstep, maze->mazeMap);
    //找四个方向
    for (di; di <= north; di++)
    {
        compare++;
        //试探位置
        if (maze->routeNum == 1) //找到一条路径则返回
            return;
        pos = NextPos(curpos, di);
        //判断是否可行
        if (Pass(pos, maze->mazeMap))
        {
            FootPrint(pos, curstep, maze->mazeMap);
            if (di != east)
            {
                Pop(S, &e);
                e.di = di;
                Push(S, e);
            }
            curstep++; //记步数
            //更新位置
            curpos.col = pos.col;
            curpos.row = pos.row;
            //递归调用
            FindSimpleRoute(maze, r);
        }                          //if
    }                              //for
    MarkPrint(pos, maze->mazeMap); //标记走过未走通的路径
    Pop(S, &e);                    //弹栈
    curstep--;                     //退位置，减步数
    //堆栈不为空更新位置
    if (!StackIsEmpty(*S))
    {
        curpos = getStackTop(*S).seat;
    }
}
/**
 * @brief:        方向倒退函数
 * @param[in]:    directionType
 * @return:       null
 * @note:         根据当前通道存储的方向求出上一通道的下一步走向
 */
directionType directionGoback(directionType di)
{
    return Transfrom(di - (di > 2 ? 1 : -1) * 2);
}
/**
 * @brief:        广度优先遍历求一条最短路径
 * @param[in]:    mazeInfoPtr RouteInfoPtr
 * @return:       Status
 * @note:         用队列实现 不会改变原迷宫数组
 */
Status ShortestPath(mazeInfoPtr maze, RouteInfoPtr r)
{
    QElemType e;
    Queue Q;
    posType curPos;
    int mark[MAX][MAX] = {0}; //用来缓存通道信息
    order step = 1;
    InitQueue(&Q);
    clearStack(r);
    curPos = maze->start;
    setCBlockInfo(&e, step, curPos, null);
    EnQueue(&Q, e); //将起点加入队列
    mark[curPos.row][curPos.col] = e.di;
    if (!TestMode)
    {
        printf("\n起点:(%d,%d)->终点:(%d,%d)\n", maze->start.row, maze->start.col, maze->end.row, maze->end.col);
    }
    while (!QueueuIsEmpty(Q))
    {
        step++;
        DeQueue(&Q, &e);
        curPos = e.seat;                //获取上一位置信息
        curPos = NextPos(curPos, east); //向东
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //右邻元素可通(可通 未访问)
        {
            setCBlockInfo(&e, step, curPos, west); //west此时用来寻回上一个方向
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //到达终点
            {
                break;
            }
        }
        curPos = NextPos(curPos, west); //返回上次位置

        curPos = NextPos(curPos, south); //向南
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //下邻元素可通
        {
            setCBlockInfo(&e, step, curPos, north);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //到达终点
            {
                break;
            }
        }
        curPos = NextPos(curPos, north); //返回上次位置

        curPos = NextPos(curPos, west); //向西
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //左邻元素可通
        {
            setCBlockInfo(&e, step, curPos, east);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //到达终点
            {
                break;
            }
        }
        curPos = NextPos(curPos, east); //返回上次位置

        curPos = NextPos(curPos, north); //向北
        compare++;
        if (!mark[curPos.row][curPos.col] && Pass(curPos, maze->mazeMap)) //上邻元素可通
        {
            setCBlockInfo(&e, step, curPos, south);
            EnQueue(&Q, e);
            mark[curPos.row][curPos.col] = e.di;
            if (curPos.row == maze->end.row && curPos.col == maze->end.col) //到达终点
            {
                break;
            }
        }
    }
    if (!QueueuIsEmpty(Q)) //队列非空->找到最短路径 此时队尾元素为终点
    {
        int i = 2;
        directionType tempdi, tempdi_last;
        e = Q.base[Q.rear];
        tempdi_last = e.di;
        tempdi = directionGoback(tempdi_last); //算出上一通道的下一步方向
        e.di = null;
        e.ord = 1;
        Push(r, e);
        while (!(curPos.row == maze->start.row && curPos.col == maze->start.col))
        {
            curPos = NextPos(curPos, tempdi_last);                 //退回上一步
            tempdi_last = Transfrom(mark[curPos.row][curPos.col]); //缓存当前步的上一步的下一步方向
            mark[curPos.row][curPos.col] = tempdi;                 //替换成正方向
            tempdi = directionGoback(tempdi_last);                 //算出上一通道的下一方向
            setCBlockInfo(&e, i++, curPos, Transfrom(mark[curPos.row][curPos.col]));
            Push(r, e);
        }
        if (!TestMode)
        {
            MazeMapPrint(*maze, *r);
            PathPrint(*r);
        }
        return OK;
    }
    else
    {
        printf("抱歉，此图为死局！\n");
        return FALSE;
    }
}
/**
 * @brief:        获取迷宫求解时间
 * @param[in]:    mazeMap fun
 * @return:       求解时长
 * @note:         传入可走通的迷宫
 */
int getSearchTime(mazeInfoPtr maze, RouteInfoPtr r, Status (*SearchRoute)(mazeInfoPtr maze, RouteInfoPtr r))
{
    int t_start = 0;
    int t_end = 0;
    t_start = clock();
    SearchRoute(maze, r);
    t_end = clock();
    return t_end - t_start;
}
/**
 * @brief:        比较各个算法的时间复杂度
 * @param[in]:    mazeMap fun
 * @return:       求解时长
 * @note:         传入可走通的迷宫
 */
void TimeTest(void)
{
    int i = 0;
    int time = 0;
    char row[20];
    char col[20];
    char count[20];
    mazeInfoPtr maze;
    printf("请输入迷宫起始大小和样例数量：MAXROW MAXCOL number(4*4——80*80)\n");
    do
    {
        scanf("%s", row);
        scanf("%s", col);
        scanf("%s", count);
        MAXROW = atoi(row);
        MAXCOL = atoi(col);
        TestCount = atoi(count);
        if (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80) || TestCount < 0)
            printf("\n输入数据非法，请重新输入:  ");
    } while (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80) || TestCount < 0);
    MazeInit(&maze);
    for (i = 0; i < TestCount; i++)
    {
        printf("\t%d*%d\t\t探索次数 时间\n", MAXROW, MAXCOL);
        copyMazeMap(tempMaze, mazeRandMap);
        //非递归回溯法寻找单条路径(链栈实现)
        printf("\t非递归回溯法:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[0], MazePath);
        printf("%d\t", compare);
        printf("%3dms\n", time);

        copyMazeMap(tempMaze, mazeRandMap);

        //广度优先寻找一条最短路径(队列实现)
        printf("\t广度优先法:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[1], ShortestPath);
        printf("%d\t", compare);
        printf("%3dms\n", time);

        //递归回溯法寻找一条路径(链栈存储路径)
        printf("\t递归回溯法:\t");
        compare = 0;
        time = getSearchTime(maze, maze->route[2], MazeSimmplePath);
        printf("%d\t", compare);
        printf("%3dms\n\n", time);

        MAXROW += 10;
        MAXCOL += 10;
        if (MAXROW > MAX || MAXCOL > MAX)
        {
            printf("\n迷宫大小达到上限 %d*%d\n", MAX, MAX);
            TestMode = 0;
            return;
        }
        free(maze);
        MazeInit(&maze);
    }
    TestMode = 0; //测试完成后 退出测试模式
}

directionType operator++(directionType &di, int)
{
    switch (di)
    {
    case east:
        di = south;
        break;
    case south:
        di = west;
        break;
    case west:
        di = north;
        break;
    case north:
        di = error;
        break;
    }
    return di;
}
directionType Transfrom(int d)
{
    directionType di = east;
    switch (d)
    {
    case east:
        di = east;
        break;
    case south:
        di = south;
        break;
    case west:
        di = west;
        break;
    case north:
        di = north;
        break;
    }
    return di;
}
