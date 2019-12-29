/*
 * @Author: liguiyin
 * @Date: 2019-12-23 09:21:02
 * @LastEditTime : 2019-12-23 09:40:06
 * @Description: README
 * @FilePath: \Maze\source\sort->c
 */
#include "sort.h"
/**
 * @brief:        快速排序子算法1
 * @param[in]:    排序表 low high
 * @return:       枢轴所在位置
 * @note:         一趟快速排序
 */
int Partition(SeaList S[], int low, int high, sortOrder sortorder)
{
    KeyType pivotkey = S[low]->step;
    S[0] = S[low]; //暂存枢轴记录
    while (low < high)     //从表的两端交替向中间扫描
    {
        while (low < high && (sortorder == ascending ? S[high]->step >= pivotkey : S[high]->step <= pivotkey))
            high--;
        S[low] = S[high]; //将比枢轴记录小的记录移到低端
        while (low < high && (sortorder == ascending ? S[low]->step <= pivotkey : S[low]->step >= pivotkey))
            low++;
        S[high] = S[low]; //将比枢轴记录大的记录移到高端
    }
    S[low] = S[0];
    return low;
}
/**
 * @brief:        快速排序子算法2
 * @param[in]:    排序表
 * @return:       null
 * @note:         时间性能：最好:O(nlogn) 最坏:O(n^2)  空间性能：O(logn)
 *                稳定性能：不稳定
 */
void QSort(SeaList S[], int low, int high, sortOrder sortorder)
{
    int pivotc;
    if (low < high)
    {
        pivotc = Partition(S, low, high, sortorder); //将递归子表分成两部分
        QSort(S, low, pivotc - 1, sortorder);        //先左
        QSort(S, pivotc + 1, high, sortorder);       //后右
    }
}
