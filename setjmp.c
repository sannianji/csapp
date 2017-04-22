#include"csapp.c"
jmp_buf buf;
int error1=0;
int error2=1;

void foo(void),bar(void);
int main()
{
	switch(setjump(buf))
	{
		case 0:
			foo();
			break;
		case 1:
			printf("Detected an error1 condition in foo\n");
			break;
		case 2:
			printf("Detected an error2 condition in foo\n");
			break;
		default:
			printf("Unknown error condition in foo\n");
	}
	exit(0);
}
void foo()
{
	if(error1)
		longjmp(buf,1);
	bar();
}
void bar()
{
	if(error2)
		longjmp(buf,2);
}
