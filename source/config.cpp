/*
 * @Author: liguiyin
 * @Date: 2019-12-28 19:15:13
 * @LastEditTime : 2019-12-28 20:13:00
 * @Description: README
 * @FilePath: \Maze_V2.0\source\config.cpp
 */
#include "config.h"
#include "stdio.h"
/**
 * @brief   提示信息输出
 * @param[in]   提示信息
 * @retval  null
 **/
void printTip(char *str)
{
    printf((char *)"→_→\t%s\t←_←\n", str);
}
/**
 * @brief   错误信息输入
 * @param[in]   提示信息
 * @retval  null
 **/
void printError(char *str)
{
    printf((char *)"ERROR:%s\n",str);
}
