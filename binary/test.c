#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *output = fopen("exefile","rb");
    char *outContent = (char *) malloc(100);

    //char outContent[] = "hello world!\304\343\272\303\243\241";
    //int iRet = fwrite(outContent, 1, 100, output);
    int iRet = fread(outContent, 1, 99, output);
    printf("output size:%d.\n", iRet); 
    printf("output txt:%s\n", outContent);
    int iEof = feof(output);
    if(0 != iEof)
    {
        printf("Till the file end.\n");
    }
    int iErr = ferror(output);
    if(0 != iErr)
    {
        printf("Find error!\n");
    }
    FILE *output2 = fopen("testfile2", "wb");
    iRet = fwrite(outContent, 1, 99, output2);
    FILE *output3 = fopen("testfile3", "w");
    iRet = fwrite(outContent, 1, 99, output3); 
    free(outContent); 
    fclose(output);
    fclose(output2);
    fclose(output3);
    return 0;
}
