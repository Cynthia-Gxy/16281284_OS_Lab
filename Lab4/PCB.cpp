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
    LNode* next;
};
struct Link
{
    int num;//链表上的结点数
    LNode* next;
};

int stay = 6;
int p;//工作集的起始位置
int table[32];//物理内存，每一个元素代表一个页面
int visit[32]; //访问序列
int memo[3] = { -1, -1, -1 };
int lost = 0;//没找到的页面数
int index = 0;//指示当前下标
LNode* nodes;//改进型Clock置换算法用到的数据结构
Link idle;
Link modified;

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

LNode* isinLinks (int n)//判断是否在队列中 
{
    LNode*p, *q;
    p = idle.next;
    q = NULL;
    
    while (p)
    {
        if (p->data == visit[n])//队列中页面同访问页面一致 
        {
            if (q != NULL)
            {
                q->next = p->next;
                p->next = NULL;
                idle.num--;
                break;
            }
            else
            {
                idle.next = NULL;
            }
        }
        q = p;
        p = p->next;
    } 
    if (p == NULL)
    {
        p = modified.next;
        
        while (p != NULL)
        {
            if (p->data == visit[n])
            {
                if (p == modified.next)
                { modified.next = p->next; }
                
                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    modified.num--;
                }
                
                if (modified.num == 0)
                { modified.next = NULL; }
                
                break;
            }
            
            q = p;
            p = p->next;
        }
    }
    
    return p;
}
void emptyIdle ()
{
    LNode* p;
    p = idle.next;
    
    while (p)
 {
        idle.next = p->next;
        free (p);
        p = idle.next;
    }
    
    idle.num = 0;
}
void emptyModi()
{
    LNode* p;
    p = modified.next;
    
    while (p)
    {
        modified.next = p->next;
        free (p);
        p = modified.next;
    }
    modified.num = 0;
}
void addToLink (int data, int type)//加入队列函数 
{
    LNode* p;
    LNode* q;
    q = (LNode*) malloc (sizeof (LNode));
    q->data = data;
    q->flag = 1;
    if (type == 1)
    {
        q->modify = 1;
        p = modified.next;
    }
    else
    {
        q->modify = 0;
        p = idle.next;
    }
    q->next = NULL;
    if (p == NULL)
    {
        if (type == 0)
        {
            idle.next = q;
        }
        
        else
        {
            modified.next = q;
        }
    }
    else
    {
        while (p)
        {
            if (p->next == NULL)
            {
                p->next = q;
                break;
            }
            
            else
            {
                p = p->next;
            }
        }
    }
    if (type == 0)
    {
        idle.num += 1;
        
        if (idle.num == 10)
        {
            emptyIdle();
        }
    }
    else
    {
        modified.num += 1;
        
        if (modified.num == 10)
        {
            emptyModi();
        }
    }
}

void PBA (int n)
{
    if (!isInNodes (n))//如果不可直接访问，即缺页 
    {
        if (index == stay)//判断当前下标是否到达窗口边缘 ，即是否装满窗口 
        {
            LNode *p;
            if ( (p = isinLinks (n)) != NULL)//若在二级队列中存在，则直接装入 
            {
                nodes = (LNode*) realloc (nodes, (stay + 1) * sizeof (LNode));
                nodes[stay] .data = p->data;
                nodes[stay].flag = p->flag;
                nodes[stay].modify = p->modify;
                nodes[stay].next = p->next;
                free (p);//释放当前结点，已拿走 
                stay++;
                index++;
            }
            else//若二级队列中没有，则为实际缺页 
            {
                lost++;//缺页数加一 
                if (nodes[n % 3].modify == 1)//当修改位为1 
                {
                    addToLink (nodes[n % 3].data, 1);//加入队列中修改位为1 
                }
                else
                {
                    addToLink (nodes[n % 3].data, 0);//否侧为0 
                }
                nodes[n % 3].data = visit[n];//装入页面 
                nodes[n % 3].flag = 1;//访问位置1 
                nodes[n % 3].next = NULL;
                if (rand() % 10 < 4)
                {
                    nodes[n % 3].modify = 0;
                }
                else
                {
                    nodes[n % 3].modify = 1;
                }
            }
        }
        else//若未装满则直接装入，此时不计算缺页次数 
        {
            nodes[index].data = visit[n];
            nodes[index].flag = 1;
            nodes[index].next = NULL;
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

int main ()
{
    int i = 0, j = 0;
    int k; 
	int len;
	printf("请输入您需要的随机序列长度：\n");
	scanf("%d",&len); 
    //produce(len);
    printf("产生的随机序列如下：\n");
     for (; k < len; k++)
    {
        printf ("%d, ", visit[k]);
    }
    printf("\n");
    
    printf ("页面缓冲置换算法(PBA)\n");
    idle.num = 0;
    idle.next = NULL;
    modified.num = 0;
    modified.next = NULL;
    nodes = (LNode*) malloc (stay * sizeof (LNode));
    
    for (i = 0; i < stay; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = 0;
        nodes[i].modify = 0;
        nodes[i].next = NULL;
    }
    
    for (i = 0; i < len; i++)
    {
        PBA (i);
        
        for (j = 0; j < stay; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        
        printf ("\n");
    }
    
    printf ("页面缓冲置换算法缺页率：%f   缺页次数为：%d\n", lost / (len*1.0), lost);
    getchar();
    getchar();
    return 0;
}

