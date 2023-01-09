#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buf[64];
    //子程序通过p1接受ping, 通过p2发送pong
    if(fork() == 0) {
        close(p1[1]);
        read(p1[0], &buf, sizeof(buf));
        close(p1[0]);
        printf("%d received: %s\n",getpid(),buf);
        write(p2[1], "pong\n", 5);
        close(p2[0]);
        close(p2[1]);

    } else {
        //父程序通过p1发送ping, 通过p2接受pong
        close(p2[1]);
        //父进程必须先发送一个ping, 不然父子进程都会read阻塞
        write(p1[1], "ping\n", 5);
        close(p1[1]);
        read(p2[0], &buf, sizeof(buf));
        close(p2[0]);
        printf("%d received: %s\n", getpid(), buf);
        write(p1[1], "ping\n", 5);
        close(p1[1]);
    }
    exit(0);
}
