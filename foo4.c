#include<stdio.h>
void foo(void);
int x;
int main()
{
	x=1234;
	foo();
	printf("x=%d\n",x);
	return 0;
}

