#include <unistd.h>
#include <stdio.h>

int main(){
    pid_t p1,p2;// 记录生成的两个子进程的PID
    p1=fork();
    if(p1==0)
    {
 	p2=fork();
	if(p2>0)
	fork();
	printf("进程%d的爸爸是:进程%d\n",getpid(),getppid());
sleep(1);
    }
    if(p1>0)
    {
 	fork();
	printf("进程%d的爸爸是:进程%d\n",getpid(),getppid());
sleep(1);
    }
	return 0;
}
