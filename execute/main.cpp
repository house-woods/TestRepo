#include <iostream>
#include <ospace/file.h>
#include <ospace/unix.h>

int main()
{
    osi_init_unix_toolkit();
    osi_init_file_toolkit();
    osi_file_toolkit init_file;
    osi_unix_toolkit init_unix;
    osi_file file("exec4.out", O_CREAT | O_WRONLY);
    osi_this_process::execute(osi_nil_env, osi_nil_desc, file, file, "./test.sh");
    return 0;
}


