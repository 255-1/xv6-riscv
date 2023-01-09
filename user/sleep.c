#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("请输入时间参数\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}
