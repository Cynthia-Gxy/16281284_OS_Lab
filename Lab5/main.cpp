#include <iostream>
#include "function.h"

int main()
{
	Init();
	
	char func[20];
	int index,count,pos;
	Init_block(func,20);
	while(gets(func))
	{
		int fun;
		char filename[FILE_NAME_LENGTH];
		//初始化
		fun = -1;
		Init_block(filename,FILE_NAME_LENGTH);
		if(strncmp("dictionary",func,10) == 0)			//浏览目录dir(non)
		{
			printf("---------------------文件情况如下-------------------------\n");
			directory();	
			printf("----------------------------------------------\n\n");
		}
		if(strncmp("create",func,6) == 0)			//创建文件命令create(filename)
		{
			strcat(filename,func+7);
			if(create(filename) == 1)
				printf("已创建文件%s\n\n",filename);
			directory();
		}
		if(strncmp("delete",func,6) == 0)			//删除文件命令delete(filename)
		{
			strcat(filename,func+7);
			if(destroy(filename) == 1)
				printf("已删除文件%s\n\n",filename);
			directory();
		}
		if(strncmp("open",func,4) == 0)			//打开文件命令open(filename)
		{
			strcat(filename,func+5);
			if(open(filename) == 1)
				printf("已打开文件%s，请操作：\n",filename);
		}
		if(strncmp("close",func,5) == 0)			//关闭文件命令close(index)
		{
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n\n");
				break;
			}
			printf("请输入要关闭文件的索引号：\n");
			scanf("%d",&index);
			if(close(index) == 1)
				printf("已关闭%d\n\n",index);
			directory();
		}
		if(strncmp("read",func,4) == 0)			//读文件命令read(index)
		{
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n\n");
				break;
			}
			printf("请输入要读取文件的索引号：\n");
			scanf("%d",&index);
			printf("请输入读取文件长度：\n");
			scanf("%d",&count);
			if(read(index,0,count) == 1)
				printf("读文件成功\n\n");
		}
		if(strncmp("write",func,5) == 0)			//写文件命令write(index)
		{
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n\n");
				break;
			}
			printf("请输入要写入文件的索引号\n");
			scanf("%d",&index);
			printf("请输入写入长度：\n");
			scanf("%d",&count);
			if(write(index,0,count) == 1)
				printf("写入操作成功\n\n");
		}
		if(strncmp("lseek",func,5) == 0)			//指针命令lseek(index,pos)
		{
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n\n");
				break;
			}
			printf("请输入要写入文件的索引号：\n");
			scanf("%d",&index);
			printf("请输入文件相对位置\n");
			scanf("%d",&pos);
			lseek(index,pos);
			printf("\n"); 
		}
		if(strncmp("openlist",func,6) == 0)			// 查看打开文件表
		{
			if(show_openlist()==0)
			{
				printf("没有文件被打开\n\n");
				break;
			}
			show_openlist();
		}
		if(strncmp("lblock",func,6) == 0)			//查看磁盘 
		{
			printf("磁盘情况如下：\n");
			show_lblock();
		}

		if(strncmp("exit",func,4) == 0)			// 退出命令exit
		{
			exit(0); 
		}
		fflush(stdin);
		Init_block(func,20);
	}
return 0;
}

