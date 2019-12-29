/*
 * @Author: liguiyin
 * @Date: 2019-12-20 15:40:49
 * @LastEditTime : 2019-12-29 00:05:18
 * @Description: README
 * @FilePath: \Maze\include\stack.h
 */
#ifndef _statck_h
#define _statck_h
#include "config.h"
#include "maze.h"

//¶ÑÕ»Ïà¹Øº¯Êý
Status InitStack(linkStackPtr *S);
Status StackIsEmpty(linkStack S);
SElemType getStackTop(linkStack S);
Status destoryStack(linkStack *S);
Status clearStack(linkStack *S);
Status Push(linkStack *S, SElemType CB);
Status Pop(linkStack *S, SElemType *CB);
Status StackCopy(linkStack  r, RouteInfoPtr *route);
#endif
