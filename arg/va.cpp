#include <stdio.h>
#include <stdlib.h>
void myprintf(char* fmt, ...) 
{
    char* pArg=NULL;
    char c;
    pArg = (char*) &fmt; 
    pArg += sizeof(fmt);
    do
    {
        c =*fmt;
        if (c != '%')
        {
            putchar(c);
        }
        else
        {
           switch(*++fmt)
           {
            case'd':
                printf("%d",*((int*)pArg)); 
                break;
            case'x':
                printf("%#x",*((int*)pArg));
                break;
            default:
                break;
            }
            pArg += sizeof(int); 
        }
        ++fmt;
    }while (*fmt != '\0');
    pArg = NULL;
    return;
}
int main(int argc, char* argv[])
{
    int i = 1234;
    int j = 5678;
    myprintf("the first test:i=%d\n",i,j);
    myprintf("the secend test:i=%d; %x;j=%d;\n",i,0xabcd,j);
    return 0;
}

