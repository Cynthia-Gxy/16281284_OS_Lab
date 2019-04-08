#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int fd[2];
    char buf[20];
    int real_read;
    pid_t pid;

    memset((void*)buf, 0, sizeof(buf));
    if(-1 == pipe(fd))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

     pid=fork();
     if (pid == 0) /* 创建一个子进程 */
     {
  /* 子进程关闭写描述符，并通过使子进程暂停1s等待父进程已关闭相应的读描述符 */
            close(fd[1]);
            /* 子进程读取管道内容 */
	while(1)
	{
		//系统调用
            if ((real_read = read(fd[0], buf, sizeof(buf))) > 0)
            {
                printf("%d bytes read from the pipe is '%s'\n", real_read, buf);
            }
	    else
	    	printf("no data\n");sleep(1);
	    if(strcmp(buf,"end")==0)
			break;
	    memset(buf,0,sizeof(buf));
	}
     }
     if (pid > 0)
     {
  /* 父进程关闭读描述符，并通过使父进程暂停1s等待子进程已关闭相应的写描述符 */
            close(fd[0]);
        while(1)
	{
            printf("write into the pipe:\n");
	    scanf("%s",buf);
	    write(fd[1],buf,strlen(buf));//系统调用
	    if(strcmp(buf,"end")==0)
		break;
         }
     }
    return 0;
}
