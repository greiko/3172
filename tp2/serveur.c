#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_WHITE  "\x1B[37m"


int main() {

    int fileDescriptor;

    char *pipeSrc = "pipe";

    mkfifo("pipe", 0666);

    char buffer[250];

    while (1) {
        fileDescriptor = open(pipeSrc, O_RDONLY);
        read(fileDescriptor, buffer, 250);

        if (strcmp(buffer, "fin\n") == 0) {
            printf("%s====================================\n", COLOR_YELLOW);
            printf("Fin du Serveur\n");
            printf("====================================\n%s", COLOR_WHITE);
            close(fileDescriptor);
            remove(pipeSrc);
            break;
        } else {
            close(fileDescriptor);
            fileDescriptor = open(pipeSrc, O_WRONLY);
            printf("La commande est : %s\n", buffer);
            char *message = "Jai recu la commande!\n";
            write(fileDescriptor, message, strlen(message) + 1);
            close(fileDescriptor);
        }
    }

    return 0;
}