#include"csapp.h"
#include<unistd.h>
#define N 2

int main()
{
	int status ,i;
	pid_t pid;
	while(1){	
		for(i=0;i<N;i++)
			if((pid=Fork())==0)
				exit(100+i);
		while((pid=waitpid(-1,&status,0))>0)
			{
				if(WIFEXITED(status))
					printf("child %d terminated normally with exit status=%d\n",pid,WEXITSTATUS(status));
				else
					printf("child %d terminate abnormally\n",pid);
			}
		if(errno!=ECHILD)
			unix_error("waitpid error");
		sleep(1);
		}
	exit(0);
}
