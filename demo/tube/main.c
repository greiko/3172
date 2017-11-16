#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {

    int fileDescriptor;

    char * pipeSrc = "pipe";

    mkfifo("pipe",0666);

    char buffer[250];

    while(1){
        fileDescriptor = open(pipeSrc, O_RDONLY);
        read(fileDescriptor,buffer,250);

        if (strcmp(buffer, "FIN\n") == 0){
            printf("Fin du programme")
            break;
        }
    }



    return 0;
}