#include<cstdlib>
#include<cstdio>
int main()
{
	int num = 10;
	char str[100];
	itoa(num, str, 2);
	printf("%s\n", str);
	return 0;
}
