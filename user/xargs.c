#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define ARGSZ 512
void clearArgs(char *xargv[MAXARG], int old) {
    for(int i=old; i<MAXARG; ++i) {
        xargv[i] = 0;
    }
}

void clearArg(char new_arg[ARGSZ]) {
    for(int i=0; i<ARGSZ; ++i) {
        new_arg[i] = 0;
    }
}

void xargs(int argc, char *argv[]) {
    char *xargv[MAXARG] = {0};
    for(int i = 1; i<argc; ++i) {
        xargv[i-1] = argv[i];
    }
    int count = argc - 1;
    char buf;
    char new_arg[ARGSZ] = {0};
    int arg = 0;
    int begin = arg;
    while(read(0, &buf, sizeof(char)) == sizeof(char)) {
        if(buf == ' ') {
            new_arg[++arg] = '\0';
            xargv[count++] = &new_arg[begin];
            begin = arg;
        } else if(buf == '\n') {
            new_arg[++arg] = '\0';
            xargv[count++] = &new_arg[begin];
            if(fork() == 0) {
                exec(argv[1], xargv);
            } else {
                wait(0);
                clearArg(new_arg);
                clearArgs(xargv, argc - 1);
                count = argc - 1;
                arg = 0;
                begin = arg;
            }
        } else {
            new_arg[arg++] = buf;
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    xargs(argc, argv);
    exit(0);
}
