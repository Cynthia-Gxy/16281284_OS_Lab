#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#define C		10			//柱面号 
#define H		10			//磁头号 
#define B1		10			//扇区号 
#define L		500	    //存储块总数
#define B 		10			//存储块长度
#define K		100			//保留区大小
#define FILE_BLOCK_LENGTH		7				//文件分配磁盘块号数组长度
#define FILE_NAME_LENGTH		9				//最长文件名长度
#define FILE_SIGN_AREA			((L-1-K)/B+1)		//保留区中文件标识符起始块号(位图之后)
#define FILE_NUM				FILE_BLOCK_LENGTH	//目录内最多文件数目
#define BUFFER_LENGTH			25					//打开文件表目中的缓冲区长度

struct BLock
{
    int Block_num; //存储块号
    int c; // 柱面号
    int h; //磁头号
    int b; //扇区号
}Block; 

struct filesign{							//文件描述符
	int file_length;						//文件长度
	int filesign_flag;						//占用标识位
	int file_block;							//文件分配磁盘块号数组实际长度
	int file_block_ary[FILE_BLOCK_LENGTH];	//文件分配磁盘块号数组
};

struct con{							//目录项
	char filename[FILE_NAME_LENGTH];		//文件名
	int	 filesignnum;						//文件描述符序号
};

struct openfilelist{						//打开文件表表目
	char buffer[BUFFER_LENGTH];				//读写缓冲区
	int pointer[2];							//读写指针
	int filesignnum;						//文件描述符
	int flag;								//占用符
};

BLock ldisk[C][H][B1];						//模拟磁盘
char lblock[L][B]; 

openfilelist open_list[FILE_NUM];		//打开文件表

int Disk_Block(int c,int h,int b);
void InitDisk();
int show_openlist();				
void directory();	
void read_block(int,char *);	
void write_block(int,char *);		
void Init();						
int create(char *);					
int destroy(char *);				
int open(char *);					
int close(int);					
int read(int,int,int);			
int write(int,int,int);			
int write_buffer(int,int);		
int lseek(int,int);				
void Init_block(char,int);		

int Disk_Block(int c,int h,int b)
{
    return b+b*h+b*h*c;
}

void InitDisk()
{
    for(int i=0;i<C;i++)
        for(int j=0;j<H;j++)
            for(int k=0;k<B1;k++)
            {
                ldisk[i][j][k].c=i;
                ldisk[i][j][k].h=j;
                ldisk[i][j][k].b=k;
                ldisk[i][j][k].Block_num=Disk_Block(i,j,k);//计算对应的逻辑块号
            }
}

void read_block(int i,char *p)
{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		*temp = lblock[i][a];
		a++;
		temp++;
	}
}

void write_block(int i,char *p)
{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		lblock[i][a] = *temp;
		a++;
		temp++;
	}
}

void Init_block(char *temp,int length) 
{
	int i;
	for(i = 0; i < length; i++)
	{
		temp[i] = '\0';
	}
}

int write_buffer(int index,int list)
{

	int i;
	int j;
	int freed;
	char temp[B];

	int buffer_length = BUFFER_LENGTH;
	for(i = 0; i < BUFFER_LENGTH ; i ++)
	{
		if(open_list[list].buffer[i] == '\0')
		{
			buffer_length = i;				
			break;
		}
	}
	
	int x = open_list[list].pointer[0];
	int y = open_list[list].pointer[1];
	int z = B-y;									

	if( buffer_length < z )				
	{	
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,buffer_length);	
		write_block(x,temp);
		
		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;	
		write_block(index+FILE_SIGN_AREA,temp);
		
		open_list[list].pointer[0] = x;
		open_list[list].pointer[1] = y + buffer_length ;				
	}
	else								
	{
		read_block(index+FILE_SIGN_AREA,temp);
		if(temp[2]+(buffer_length - z)/B+1 > FILE_BLOCK_LENGTH)
		{
			printf("文件分配数组不够分配\n");
			return 0;
		}
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,z);			
		write_block(x,temp);
		for(i = 0; i < (buffer_length - z)/B ; i ++ )
		{
			for(j = K + FILE_NUM; j < L ; j++)
			{
				read_block((j-K)/B,temp);
				if(temp[(j-K)%B] == 0)
				{
					freed = j;
					break;
				}
			}
			if(j == L)
			{
				printf("磁盘已满,分配失败\n");
				return 0;
			}
			
			Init_block(temp,B);						
			strncpy(temp,(open_list[list].buffer+z+(i*B)),B);
			write_block(freed,temp);				

			read_block((freed-K)/B,temp);			
			temp[(freed-K)%B] = 1;
			write_block((freed-K)/B,temp);			

			read_block(index+FILE_SIGN_AREA,temp);
			temp[2] ++;								
			temp[2+temp[2]] = freed;
			write_block(index+FILE_SIGN_AREA,temp);	
		}
		for(j = K + FILE_NUM; j < L ; j++)
		{
			read_block((j-K)/B,temp);
			if(temp[(j-K)%B] == 0)
			{
				freed = j;
				break;
			}
		}
		if(j == L)
		{
			printf("磁盘已满,分配失败\n");
			return 0;
		}
		Init_block(temp,B);
		strncpy(temp,(open_list[list].buffer+z+(i*B)),(buffer_length - z)%B);
		write_block(freed,temp);			

		read_block((freed-K)/B,temp);		
		temp[(freed-K)%B] = 1;
		write_block((freed-K)/B,temp);			

		read_block(index+FILE_SIGN_AREA,temp);
		temp[2] ++;							
		temp[2+temp[2]] = freed;
		write_block(index+FILE_SIGN_AREA,temp);


		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;			
		write_block(index+FILE_SIGN_AREA,temp);

		open_list[list].pointer[0] = freed;
		open_list[list].pointer[1] = (buffer_length - z)%B ;
	}	
}
int lseek(int index,int pos)
{
	int i;
	int list = -1;
	char temp[B];
	int block_pos = pos / B; 	
	int block_in = pos % B;			
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)//在打开目录中找到对应文件 
		{
			list = i;
			break;
		}
	}
	read_block(open_list[list].filesignnum + FILE_SIGN_AREA,temp);//确定具体内容 
	if(block_pos > temp[2]-1)										
	{
		printf("越界\n");
		return 0;
	}
	open_list[list].pointer[0] = temp[3+block_pos];		
	open_list[list].pointer[1] = block_in; //找到定位位置 
	return 1;

}


void Init()
{
	
	int i;
	char temp[B];
	

	for(i = 0; i < L; i++)					
	{
		Init_block(temp,B);
		write_block(i,temp);			
	}

	for(i = K; i < L; i ++)					
	{
		read_block((i-K)/B,temp);
		temp[(i-K)%B] = 0;
		write_block((i-K)%B,temp);
	}


	filesign temp_cnt_sign;
	temp_cnt_sign.filesign_flag =1;
	temp_cnt_sign.file_length = 0;							
	temp_cnt_sign.file_block = FILE_BLOCK_LENGTH;
	 
	Init_block(temp,B);
	temp[0] = temp_cnt_sign.filesign_flag;
	temp[1] = temp_cnt_sign.file_length;
	temp[2] = temp_cnt_sign.file_block;

	for(i = 0; i < FILE_BLOCK_LENGTH ; i++ )
	{
		temp[i+3] = K+i;								
	}
	write_block(FILE_SIGN_AREA,temp);


	read_block(0,temp);
	for(i = 0; i < FILE_NUM ; i++ )
	{
		temp[i] = 0;					
	}
	write_block(0,temp);
}


int create(char filename[])
{
	int i;
	int sign_local;						
	int	content_local;						
	int content_locall;								
	char temps[B];
	char tempc[B];
	char temp[B];	

	for(i = FILE_SIGN_AREA ; i < K; i ++)
	{	
		read_block(i,temp);
		if(temp[0] == 0)
		{
			sign_local = i;			//找到保留区中空闲的文件描述符	
			break;
		}
	}
	for(i = K ; i < K+FILE_NUM ; i++)
	{	
		read_block((i-K)/B,temp);						
		if(temp[(i-K)%B] == 0)
		{
			content_local = i;     //找到数据区中指定的空闲块 
			break;
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("文件已达上限\n");
		return 0;
	}

	for(i = K + FILE_NUM; i < L ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 0)
		{
			content_locall = i;   //找到文件区的空闲块 
			break;
		}
	}
	//临时变量 
	filesign temp_filesign;				
	contents temp_contents;				
	temp_filesign.filesign_flag = 1;
	temp_filesign.file_length = 0;
	temp_filesign.file_block = 1;
	
	Init_block(temps,B);
	temps[0] = temp_filesign.filesign_flag;
	temps[1] = temp_filesign.file_length;
	temps[2] = temp_filesign.file_block;
	temps[3] = content_locall;
	for(i = 4 ; i < FILE_BLOCK_LENGTH ; i++)
	{
		temps[i] = '\0';
	}
	write_block(sign_local,temps);		//将该量文件描述符写入磁盘	
	temp_contents.filesignnum = sign_local - FILE_SIGN_AREA;
	strncpy(temp_contents.filename,filename,FILE_NAME_LENGTH);
	
	Init_block(tempc,B);
	tempc[0] = temp_contents.filesignnum;   
	tempc[1] = '\0';
	strcat(tempc,temp_contents.filename);
	write_block(content_local,tempc);	//写入数据区部分		

	read_block((content_local-K)/B,temp);     //修改位图 
	temp[(content_local-K)%B] = 1;
	write_block((content_local-K)/B,temp);		

	read_block((content_locall-K)/B,temp);		
	temp[(content_locall-K)%B] = 1;
	write_block((content_locall-K)/B,temp);		

	read_block(FILE_SIGN_AREA,temp); //目录长度加1 
	temp[1]++;
	write_block(FILE_SIGN_AREA,temp);		
	return 1;	
}

int destroy(char * filename)
{
	int i;
	int sign_dlocal;										
	int content_dlocal;									
	int use_block;								
	int index;
	char temp[B];
	char tempd[B];
	
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 1)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)//找对应文件名
			{
				content_dlocal = i;			//找到目录项位置 
				sign_dlocal = temp[0] + FILE_SIGN_AREA;	 //找到文件描述符位置 
				index = temp[0];
				break;
			}
		}
	}
	int list = -1;
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)//确定文件是否打开 ，打开无法删除 
		{
			list = i;
			break;
		}
	}
	if(open_list[list].flag == 1 && list != -1)
	{
		printf("该文件未关闭\n");
		return 0;
	}
	
	read_block(sign_dlocal,temp);
	use_block = temp[2];
	for(i = 0 ; i < use_block ; i++)
	{
		read_block((temp[i+3]-K)/B,tempd);				
		tempd[(temp[i+3]-K)%B] = 0;
		write_block((temp[i+3]-K)/B,tempd);
	}
	//删除文件 
	Init_block(temp,B);//删除目录项 
	write_block(sign_dlocal,temp);
	Init_block(temp,B);//删除文件描述符 
	write_block(content_dlocal,temp);
	//更新位图 
	read_block((content_dlocal-K)/B,temp);
	temp[(content_dlocal-K)%B] = 0;
	write_block((content_dlocal-K)/B,temp);
	read_block(FILE_SIGN_AREA,temp);
	temp[1]--;
	write_block(FILE_SIGN_AREA,temp);
	return 1;
}
int open(char * filename)
{
	int i;
	int oppos;
	int opcontent;
	int list;
	char temp[B];
	int index;       
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == 1)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)//找到对应文件名 
			{
				oppos = i;				//找到文件目录项位置
				opcontent = temp[0] ;	//找到文件描述符 
				break;
			}
		}
	}
	for(i = 0 ; i < FILE_NUM ; i++)  //找到空表目 
	{
		if(open_list[i].flag != 1)
		{	
			list = i;
			break;
		}
	}
	open_list[list].filesignnum = opcontent;   //写入文件描述符		
	open_list[list].flag = 1;							
	index = open_list[list].filesignnum;	//产生索引号			
	lseek(index,0);			//指针设定为在初始位置		
	Init_block(open_list[list].buffer,BUFFER_LENGTH);			
	read_block(open_list[list].pointer[0],temp);				
	strncpy(open_list[list].buffer,temp,BUFFER_LENGTH);	  //写入缓冲区 
	return 1;
}

int close(int index) 
{
	int i;
	int list = -1;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)   
	{
		if(open_list[i].filesignnum == index)    //找到目录 
		{
			list = i;
			break;
		}
	}
	write_buffer(index,list);//将打开文件表的缓冲写入 
	Init_block(open_list[list].buffer,BUFFER_LENGTH);//清空				
	open_list[list].filesignnum = 0;								
	open_list[list].flag = 0;									
	open_list[list].pointer[0] = NULL;							
	open_list[list].pointer[1] = NULL;
	return 1;
}

int read(int index, int mem_area, int count) 
{
	int i;
	int list = -1;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)//找到对应项 
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}

	char temp_output[100];//确定文件内容 
	Init_block(temp_output,100);
	char output[100];
	Init_block(output,100);
	read_block(FILE_SIGN_AREA+index,temp);
	int file_length = temp[1];				
	int file_block = temp[2];				
	int file_area;
	
	for(i = 0; i < file_block - 1 ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		read_block(temp[3+i],temp);
		strncpy(temp_output+i*B,temp,B);			
	}
	read_block(FILE_SIGN_AREA+index,temp);
	read_block(temp[3+i],temp);
	strncpy(temp_output+i*B,temp,B);
	int x = open_list[list].pointer[0];   //定位文件 
	int y = open_list[list].pointer[1];
	for(i = 0 ; i < file_block ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		if(temp[3+i] == x)
		{
			break;
		}
	}
	file_area = i * B + y;		//位置转换						
	for(i = 0 ;  i < count ; i++)//找到定位的地方进行输出 
	{
		output[i+mem_area] = temp_output[i+file_area]; 
	}
	printf("%s\n",output+mem_area);
	return 1;
}
 
int write(int index,int mem_area,int count)
{
	int i;
	int list = -1;
	int input_length;
	char temp[B];
	for(i = 0; i < FILE_NUM ; i++)//找到文件打开表 
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	char input[100];
	Init_block(input,100);
	i = 0;
	fflush(stdin);
	while(scanf("%c",&input[i]))   //输入结束 
	{
		if(input[i] == '\n')											
		{
			input[i] == '\0';			
			break;
		}
		i++;
	}
	input_length = i;		
	if(count <= BUFFER_LENGTH )		//缓存区够可以写入			
	{
		strncat(open_list[list].buffer,input+mem_area,count);		
	}
	else												
	{
		int rest;					
		for(i = 0; i < BUFFER_LENGTH ; i++)
		{
			if(open_list[list].buffer[i] == 0)
			{
				rest = BUFFER_LENGTH - i;    //得出空闲容量 
				break;
			}
		}		
		strncat(open_list[list].buffer+BUFFER_LENGTH-rest,input + mem_area,rest);   //有空闲的写入文件并清空 
		write_buffer(index,list);
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		for(i = 0; i < (count/BUFFER_LENGTH)-1 ; i++)    //循环写入 
		{
			strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,BUFFER_LENGTH);

			write_buffer(index,list);
			Init_block(open_list[list].buffer,BUFFER_LENGTH);
		}
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,count%BUFFER_LENGTH);//剩余的写入缓冲区 
		int buffer_start;
	}
	return 1;
}

void directory()
{
	int i;
	int filenum;
	int filelength;
	char filename[FILE_NAME_LENGTH];
	char temp[B];
	char tempd[B];
	char temps[B];
	read_block(FILE_SIGN_AREA,temp);
	filenum = temp[1];						
	printf("\n");
	if(filenum == 0 )//文件数为零时的情况 
	{
		printf("该目录下没有文件\n");
	}
	
	for(i = 0; i < FILE_NUM; i++)//显示目录中文件信息 
	{
		read_block(temp[3+i],tempd);					
		if(tempd[0] != 0)
		{
			read_block(tempd[0]+FILE_SIGN_AREA,temps);		
			if(temps[0] == 1 && tempd[0] != 0)
			{	
				filelength = temps[1];
				strcpy(filename,tempd+1);
				printf("%s\t\t该文件的大小为%d字节\n",filename,filelength);
			}
		}
	}
	if(filenum != 0 )
	{
		printf("\t\t\t\t当前共有%d个文件\n",filenum);//输出统计信息 
	}
}

int show_openlist()
{

	int i,j;
	int openfile = 0;
	char temp[B];
	int index;
	printf("\n索引号\t\t该文件的大小\t\t该文件名\n");
	for(i = 0 ; i < FILE_NUM ; i ++) 
	{
		if(open_list[i].flag == 1)//在文件打开列表中寻找 
		{
			index = open_list[i].filesignnum;//找到对应索引号 
			printf("  %d",index);
			openfile++;
			read_block(FILE_SIGN_AREA+index,temp);//找到对应文件大小 
			printf("\t\t %d",temp[1]);	
			for(j = K; j < K+FILE_NUM ; j++)
			{
				read_block(j,temp);
				if(temp[0] == index)//找到对应文件名 
				{
					printf("\t\t%s\n",temp+1);
				}
			}	
		}
	}
	return openfile;

}

