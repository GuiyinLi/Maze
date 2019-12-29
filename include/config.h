/*
 * @Author: liguiyin
 * @Date: 2019-12-20 15:36:18
 * @LastEditTime : 2019-12-28 20:38:42
 * @Description: README
 * @FilePath: \Maze\include\config.h
 */
#ifndef _config_h
#define _config_h

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASLIBLE -1 //不可行的
#define OVERFLOW -2    //溢出
//Status 函数返回值类型
typedef int Status;

void printTip(char *str);   //提示信息打印
void printError(char *str); //错误信息打印

#endif
