#include"csapp.h"
int main()
{
	int x=1;
	pid_t pid=Fork();
	if(pid==0)
	{
		printf("x=%d,pid=%d\n",++x,pid);
	}
	printf("x=%d,pid=%d\n",--x,pid);
	exit(0);
}
