#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

void primes(int *p) {
    close(p[WR]);
    int data;
    if(read(p[RD], &data, INT_LEN) == 0) exit(0);
    printf("prime %d\n", data);
    int p2[2];
    pipe(p2);
    int data2;
    while(read(p[RD], &data2, INT_LEN) == INT_LEN) {
        if(data2 % data) {
            write(p2[WR], &data2, INT_LEN);
            // printf("put %d into next pipe\n", data2);
        }
    }
    if(fork() == 0) {
        primes(p2);
    } else {
        close(p[RD]);
        close(p2[WR]);
        close(p2[RD]);
        wait(0);
    }
    exit(0);
}


int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    for(int i=2; i<=35; ++i) {
        write(p[WR], &i, INT_LEN);
    }

    if(fork() == 0) {
        primes(p);
    } else {
        close(p[RD]);
        close(p[WR]);
        wait(0);
    }

    exit(0);
}
