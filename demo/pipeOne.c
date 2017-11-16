
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(){
    int pipefd[2];
    if (pipe(pipefd) == -1){
        exit(-1);
    }
    int size = fcntl
}