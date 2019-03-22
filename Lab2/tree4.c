#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    pid_t p1,p2,pp;// 记录生成的两个子进程的PID
    p1=fork();
    if(p1==0)
    {
 	p2=fork();
	if(p2>0)
	pp=fork();
while(1){
	printf("进程%d的爸爸是:进程%d\n",getpid(),getppid());
	sleep(1);
	if(p2>0 && pp>0)
	{
		int *p=NULL;
		*p=0;
	}
}
    }
    if(p1>0)
    {
 	fork();
while(1){
	printf("进程%d的爸爸是:进程%d\n",getpid(),getppid());
	sleep(1);
}
    }
printf("\n");
	return 0;
}
