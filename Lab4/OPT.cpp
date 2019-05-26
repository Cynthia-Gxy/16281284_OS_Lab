#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#define P 32    //物理内存块数
#define V 64   //虚拟内存块数

struct LNode
{
    int data;
    int flag;//访问位
    int modify;//修改位
};

int stay = 6;//暂留集数 
int visit[32]; //访问序列
int* work;
int lost = 0;//没找到的页面数
int index = 0;//指示当前下标
LNode* nodes;//改进型Clock置换算法用到的数据结构

void produce(int len)
{
    srand ( (unsigned) time (NULL)); 
    int p = rand() % 64;
    int m = 8, e = 8;
    int i, j;
    double t;
    t = rand() % 10 / 10.0;
    
    for (i = 0; i < 4; i++)
    {
        for (j = i * m; j < (i + 1) *m; j++)
        {
        	if (j > len)
			break;
            visit[j] = (p + rand() % e) % 64;
        }
        
        double r = (rand() % 10) / 10.0;
        
        if (r < t)
        {
            p = rand() % 64;
        }
        
        else
        {
            p = (p + 1) % 64;
        }
    }
}

void initwork()//初始化窗口 
{
    work = (int*) malloc (stay * sizeof (int));
    int i = 0;
    
    for (; i < stay; i++)
    {
        work[i] = -1;//还未调入页面是设定为-1 
    }
    
    return;
}

bool  isInwork (int n)//判断是否可直接访问 
{
    int i = 0;
    
    for (; i < stay; i++)
    {
        if (visit[n] == work[i])
        {
            return true;
        }
    }
    
    return false;
}

void optimal (int n)
{
    int i = 0, j = 0;
    
  	if (!isInwork (n))//如果不可直接访问，即缺页 
	{
        if (index == stay) //判断当前下标是否到达窗口边缘 ，即是否装满窗口 
        {
            lost++;		//缺页数增加 
            int max = 0, pos, flag;
            
            for (i = 0; i < stay; i++)//窗口内的页面 
            {
                flag = -1;
                
                for (j = n + 1; j < 32; j++)//当前页到序列最大值 
                {
                    if (visit[j] == work[i]) //第一个在驻留集内的访问页面 
                    {
                        flag = j;//标记该页面 
                        break;
                    }
                }
            
                if (flag == -1)//未被标记的页面 
                {
                    max = 32;
                    pos = i;
                    break;
                }
                else
                {
                    if (max < flag)//标记的页面 
                    {
                        max = flag;
                        pos = i;//记录替换页的位置 
                    }
                }
            }
            work[pos] = visit[n];//驻留集窗口中中该位置页面替换成要访问页面 
        }
        else//若未装满则直接装入，此时不计算缺页次数 
        {
            work[index] = visit[n];
            index++;
        }
    }
}

void run_Optimal(int len)
{
    initwork();
    int i = 0;
    printf ("最佳置换算法：\n");

    for (; i < len; i++)
    {
        optimal (i);
        printf ("[ %d %d %d ]\n", work[0], work[1], work[2]);
    }
    
    printf ("最佳置换算法缺页率： %2f   缺页次数为：%d\n", lost / (len*1.0), lost);
    lost = 0;
    free (work);
    index = 0;
}

int main ()
{
	int i = 0;
	int len;
	
	printf("请输入您需要的随机序列长度：\n");
	scanf("%d",&len); 
    //produce(len);
    printf("产生的随机序列如下：\n");
     for (; i < len; i++)
    {
        printf ("%d, ", visit[i]);
    }
    printf("\n");
    
    run_Optimal(len);
    printf("\n");
	
    getchar();
    getchar();
    return 0;
}
