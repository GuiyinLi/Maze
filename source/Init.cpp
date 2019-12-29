/*
 * @Author: liguiyin
 * @Date: 2019-12-27 09:12:05
 * @LastEditTime : 2019-12-29 00:02:30
 * @Description: README
 * @FilePath: \Maze\source\Init.c
 */
#include <Windows.h>
#include "Init.h"
int GETRANDMAZE;    //0->规定迷宫 1->随机迷宫
int MAXROW ;        //迷宫宽度 80*80
int MAXCOL ;        //迷宫长度
void PrintInit()
{
    printf("\t*************************************************************\n");
    printf("\t*                                                           *\n");
    printf("\t*                   欢迎进入迷宫求解系统                    *\n");
    printf("\t*                                                           *\n");
    printf("\t*                                设计者：李圭印、李竹林     *\n");
    printf("\t*************************************************************\n");
    printf("\t*                 ☆随机生成迷宫   请按：1                  *\n");
    printf("\t*                 ☆使用固定迷宫   请按：2                  *\n");
    printf("\t*                 ☆时间复杂度测试 请按：3                  *\n");
    printf("\t*                 ☆退出           请按：4                  *\n");
    printf("\t*************************************************************\n");
}
Status GetInput(void)
{
	int flag;
	char k[20];
	char row[20];
	char col[20];
	printf("请输入：");
	do{
		scanf("%s",k);
		if(strlen(k)>1)
			k[0] = 0;
		flag=0 ;
		switch(k[0])
        {
            case '1':
                Sleep(400);
                system("cls");
                PrintInit();
                GETRANDMAZE = 1;
	    		printf("请按格式输入：MAXROW MAXCOL(4*4——80*80)\n");
	    		do{
	    			scanf("%s",row);
	    			scanf("%s",col);
	    			MAXROW = atoi(row);
	    			MAXCOL = atoi(col);
	    			if(!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80))
	    				printf("\n输入数据非法，请重新输入:  ");
	    		} while (!(MAXROW >= 4 && MAXCOL >= 4 && MAXROW <= 80 && MAXCOL <= 80));
	    		Sleep(400);
                system("cls");
	    		flag = 1;
				TestMode = 0;
				break;
            case '2':
                flag = 1;
                Sleep(300);
                system("cls");
                GETRANDMAZE = 0;
				MAXROW = 10;
				MAXCOL = 10;
				TestMode = 0;
				break;
			case '3':
				flag = 1;
				GETRANDMAZE = 1;
				TestMode = 1;	//进入时间复杂度比较模式
				Sleep(300);
				break;
            case '4':
			    return FALSE;
            default:
                printf("\n输入数据非法，请重新输入:  ");
	    		break;
	    }
	}while(!flag);
	return TRUE;
}
/**
 * @brief:        询问是否还需要进行操作
 * @param[in]:    null
 * @return:       Status
 * @note:         null
 */
Status conOperate(void)
{
	char c;
	printf("您是否继续执行此系统(Y/N)?  ");
	do{
		scanf("%c", &c);
		if(!(c=='Y'||c=='N'||c=='\n'))
        	printf("\n输入数据非法，请重新输入:  ");
	}while(!(c=='Y'||c=='N'));
	getchar();
	if(c=='Y')
	{
		Sleep(300);
		system("cls");
		return TRUE;
	}
	else
		return FALSE;
}
