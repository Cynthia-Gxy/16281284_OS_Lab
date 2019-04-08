#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>

int main()
{
	pid_t p2,p3,p4; //创建子进程2,3,4，P1无需创建，进入主函数后的进程即为p1进程
	sem_t *t1_23,*t24,*t34;//创建信号量

	t1_23=sem_open("t1_23",O_CREAT,0666,0);//表示关系进程1执行完进程2,3中的一个可以执行
	t24=sem_open("t24",O_CREAT,0666,0);//表示关系进程2执行完进程4才可执行
	t34=sem_open("t34",O_CREAT,0666,0);//表示关系进程3执行完进程4才可执行

	p2=fork();//创建进程p2
	if(p2==0)
	{
		sem_wait(t1_23);//实现互斥
		printf("I am the process p2\n");
		sem_post(t1_23);
		sem_post(t24);//实现前驱
	}
	if(p2<0)
	{
		perror("error!");
	}
	if(p2>0)
	{
		p3=fork();//创建进程p3
		if(p3==0)
		{
			sem_wait(t1_23);//实现互斥
			printf("I am the process p3\n");
			sem_post(t1_23);
			sem_post(t34);//实现前驱
		}
		if(p3<0)
		{
			perror("error!");
		}
		if(p3>0)
		{	
			p4=fork();//创建进程p4
			if(p4>0)
			{
				printf("I am the process p1\n");
				sem_post(t1_23);
			}
			if(p4==0)
			{
				sem_wait(t24);
				sem_wait(t34);
				printf("I am the process p4\n");
				sem_post(t24);
				sem_post(t34);
			}
			if(p4<0)
			{
				perror("error!");
			}
		}
		
	}
	sleep(1);
	sem_close(t1_23);
	sem_close(t24);
	sem_close(t34);
	sem_unlink("t1_23");
	sem_unlink("t24");
	sem_unlink("t34");
	return 0;
}
