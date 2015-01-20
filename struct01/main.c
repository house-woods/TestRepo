#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct oper 
{
	int (*get)(int para01, int para02);
};
static int getValue(int p0, int p2)
{
	p0 = 10;
	return p0;
};
int main(int argc, char* argv[])
{
	struct oper aobj = 
	{
		get:	getValue,
	};
	return 0;
}
