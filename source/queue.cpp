/*
 * @Author: liguiyin
 * @Date: 2019-12-23 17:52:05
 * @LastEditTime : 2019-12-24 20:01:54
 * @Description: README
 * @FilePath: \Maze\source\queue.c
 */
#include "queue.h"
/**
 * @brief   初始化循环队列
 * @param[in]   循环队列指针
 * @retval  Status
 **/
Status InitQueue(Queue *Q)
{
    Q->base=(QElemType *)malloc(sizeof(QElemType)*MAXLEN);
    if(!(Q->base))
    {
        printError((char *)"内存溢出");
        return OVERFLOW;
    }
    Q->size = MAXLEN;
    Q->front = 0;
    Q->rear = 0;
    Q->step = 0;
    return OK;
}
/**
 * @brief   求队列长度
 * @param[in]   循环队列指针
 * @retval  队列长度
 **/
int getLength(Queue Q)
{
    return (Q.rear - Q.front + Q.size) % Q.size;;
}
/**
 * @brief   判队满
 * @param[in]   循环队列指针
 * @retval  队满返回TRUE FALSE
 **/
Status  QueueuIsFull(Queue Q)
{
     if(((Q.rear+1)%Q.size) == Q.front)
        return TRUE;
     else
        return FALSE;
}
/**
 * @brief   判队空
 * @param[in]   循环队列指针
 * @retval  队空返回TRUE 否则返回FALSE
 **/
Status  QueueuIsEmpty(Queue Q)
{
    if(Q.front==Q.rear)
        return TRUE;
     else
        return FALSE;
}
/**
 * @brief   入队
 * @param[in]   循环队列指针 二叉树
 * @retval  Status
 **/
Status EnQueue(Queue *Q, QElemType E)
{
    if(QueueuIsFull(*Q))
    {
        printError((char *)"队列已满");
        Q->size += MAXADD;
        if(!(Q->base=(QElemType *)realloc(Q->base,sizeof(QElemType)*(Q->size))))//连续分配内存
        {
            printError((char *)"内存溢出");
            return OVERFLOW;
        }
    }
    Q->rear = (Q->rear+1)%Q->size;
    Q->base[Q->rear] = E;
    Q->step++;
    return OK;
}
/**
 * @brief   出队
 * @param[in]   循环队列指针 出队元素
 * @retval  Status
 **/
Status DeQueue(Queue *Q, QElemType *E)
{
    if(QueueuIsEmpty(*Q))
    {
        printError((char *)"队列为空");
        return ERROR;
    }
    else
    {
        Q->front = (Q->front + 1) % Q->size;
        *E = Q->base[Q->front];
        Q->step--;
        return OK;
    }
}
