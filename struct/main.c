#include <stdio.h>
struct CBase
{
/*    CBase()
    {
        printf("CBase().");
    };*/
/*    virtual ~CBase()
    {
        printf(~CBase().);
    };*/
int a;
};
/*struct CDerived : public CBase
{
    CDerived()
    {
        printf("CDerived().");
    };
    ~CDerived()
    {
        printf("~CDerived().");
    };
};*/
int main(int argc, char *argv[])
{
    /*CBase * aObj = new CDerived();*/
    struct CBase * aObj = malloc(sizeof(struct CBase)); 
    free(aObj);
    return 0;
}


