#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int madsdin(){
    int pipefd[2];
    if (pipe(pipefd) == -1){
        exit(-1);
    }
    int size = fcntl(pipefd[0],F_GETPIPE_SZ);
    printf("SIZE %d", size);

    close(pipefd[0]);
    close(pipefd[1]);

}