#include"csapp.h"

int main()
{
	pid_t pid;
	
	if((pid=Fork())==0)
	{
		Pause();
		printf("control should never reach here!\n");
		exit(0);
	}
	printf("just prove the programming is still working and the control reach here\n");
	Kill(pid,SIGKILL);
	exit(0);
}
