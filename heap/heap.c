#include <stdlib.h>
char *ptr1, ptr2[4096];
int main()
{
ptr1 = (char*)malloc(2 * 4096);
ptr1[4095] = '\0';
free(ptr1);
}
