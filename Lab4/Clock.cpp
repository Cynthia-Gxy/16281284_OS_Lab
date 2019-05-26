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


bool isInNodes (int n)
{
    int i;
    
    for (i = 0; i < stay; i++)
    {
        if (nodes[i].data == visit[n])
        {
            return true;
        }
    }
    
    return false;
}

void updated_Clock (int n)
{
    if (!isInNodes (n))//如果不可直接访问，即缺页
    {
        if (index == stay)//窗口空间已装满 
        {
            lost++;//缺页数增加 
            int i = 0, flag = -1;
            while (true)
            {
                if ( (i / stay) % 2 == 0)
                {
                    if (nodes[i % stay].flag == 0 && nodes[i % stay].modify == 0)//选择flag=0且modify=0的第一个页面淘汰
                    {
                        flag = i % stay;
                        break;
                    }
                }
                if ( (i / stay) % 2 == 1)
                {
                    if (nodes[i % stay].flag == 0 && nodes[i % stay].modify == 1)//第二轮扫描，选择flag=0且modify=1的第一个页面淘汰
                    {
                        flag = i % stay;
                        break;
                    }
                    else
                    {
                        nodes[i % stay].flag = 0;//访问位置0 
                    }
                }
                i++;
            }
            nodes[flag].data = visit[n];//替换页面 
            nodes[flag].flag = 1;//访问位置1 
            if (rand() % 10 < 4)
            {
                nodes[flag].modify = 1;
            }
            
            else
            {
                nodes[flag].modify = 0;
            }
        }
        else//未满则直接装入 
        {
            nodes[index].data = visit[n];
            nodes[index].flag = 1;//访问位置1 
            if (rand() % 10 < 4)
            {
                nodes[index].modify = 1;
            }
            else
            {
                nodes[index].modify = 0;
            }
            index++;
        }
    } 
}
 
void Clock(int len)
{
    int i = 0, j = 0;
    printf ("改进型Clock置换算法\n");
    nodes = (LNode*) malloc (stay * sizeof (LNode));
    
    for (i = 0; i < stay; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = -1;
        nodes[i].modify = -1;
    }
    
    for (i = 0; i < len; i++)
    {
        updated_Clock (i);
        for (j = 0; j < stay; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        printf ("\n");
    }
    printf ("改进型Clock置换算法缺页率： %2f 缺页次数为：%d \n", lost / (len*1.0), lost);
    lost = 0;
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

    Clock(len);
	printf("\n");
	
    getchar();
    getchar();
    return 0;
}
