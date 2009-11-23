#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void){
	int cpid=0;
	char *arg[] = {NULL};
	cpid = fork();
	printf("%d\n",cpid);
	if(cpid==0){
		while( 1 ){
			puts ("a");
		}
//		execve("/bin/pwd",arg,arg);
	}else{
		usleep(2*1000*1000);
		kill(cpid,SIGKILL);
		printf("ok\n");
	}
	return 0;
}
