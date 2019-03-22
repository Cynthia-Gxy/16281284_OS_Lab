#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int pid = fork();
	if(pid>0){
		while(1);
	}
	else if(pid == 0){
		int r;
		r = execlp("vi","",NULL);
		if(r==-1){
			printf("error\n");
			exit(-1);
		}
	}
	else{
		printf("fork error\n");
		exit(-1);	
	}
	return 0;
} 
