#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
sem_t* Sem = NULL;

int ticketCount=1000;

void *back()
{
	int temp;
	int i;
	for(i=0;i<30;i++)
	{
		sem_wait(Sem);
		int temp = ticketCount;
		printf("退票后的票数为：%d \n",ticketCount);
		//放弃CPU，强制切换到另外一个进程
		pthread_yield();
		temp=temp+1;
		pthread_yield();
		ticketCount=temp;
		sem_post(Sem);
	}
}

void *sell()
{
	int temp;
	int i;
	for(i=0;i<80;i++)
	{
		sem_wait(Sem);
		int temp = ticketCount;
		printf("售票后的票数为：%d \n",ticketCount);
		//放弃CPU，强制切换到另外一个进程
		pthread_yield();
		temp=temp-1;
		pthread_yield();
		ticketCount=temp;
		sem_post(Sem);
	}

}

int main(int argc,char *argv[]){
	pthread_t p1,p2;
	printf("开始的票数为：%d \n",ticketCount);
Sem = sem_open("sem", O_CREAT, 0666, 1);
    	pthread_create(&p1,NULL,sell,NULL);
    	pthread_create(&p2,NULL,back,NULL);
    	pthread_join(p1,NULL);
    	pthread_join(p2,NULL);
sem_close(Sem);
sem_unlink("sem");
    	printf("最终的票数为： %d \n",ticketCount);
    	return 0;
}
