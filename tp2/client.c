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


    char buffer[250];
    char buffer2[250];


    while (1) {
        if (access(pipeSrc, F_OK) == -1) {
            printf("Serveur fermer\n");
            exit(0);
        } else {
            printf("Connection serveur established\n");
        }

        fileDescriptor = open(pipeSrc, O_WRONLY);
        printf("Ecrire commande: \n");
        fgets(buffer, 250, stdin);

        write(fileDescriptor, buffer, strlen(buffer) + 1);
        close(fileDescriptor);

        if (strcmp(buffer, "fin\n") == 0) {
            printf("%s====================================\n", COLOR_YELLOW);
            printf("Le serveur a recu la commande de fermer\n");
            printf("====================================\n%s", COLOR_WHITE);
            break;
        } else {

            fileDescriptor = open(pipeSrc, O_RDONLY);
            read(fileDescriptor, buffer2, sizeof(buffer2));
            printf("%s====================================\n", COLOR_YELLOW);
            printf("%s", buffer2);
            printf("====================================\n%s", COLOR_WHITE);
            close(fileDescriptor);
        }
    }

    return 0;
}