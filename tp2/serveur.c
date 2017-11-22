#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_WHITE  "\x1B[37m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"


int main() {


    int fdREad;
    int fdWrite;

    char *pipeSrc = "serveurP";

    mkfifo("serveurP", 0666);

    char buffer[BUFSIZ];
    int count = 1;
    printf("%s> Serveur up\n%s",COLOR_CYAN,COLOR_WHITE); int i = 0;

    while (1) {
        fdREad = open(pipeSrc, O_RDONLY);

        printf("%s> Connection established - Request number : %d\n%s",COLOR_CYAN, count,COLOR_WHITE);
        read(fdREad, buffer, BUFSIZ);
        close(fdREad);
        printf("%s\n", buffer);


        char nomPipe[BUFSIZ];
        char commande[BUFSIZ];
        int pipeNameDone = -1;
        int i, j;
        for (i = 0, j = 0; i < strlen(buffer); i++) {
            if (buffer[i] == ',') {
                nomPipe[j + 1] = '\0';
                j = 0;
                pipeNameDone = 0;
            } else {
                if (pipeNameDone == 0) {
                    commande[j] = buffer[i];

                } else {
                    nomPipe[j] = buffer[i];

                }
                j++;
            }
        }
        commande[j] = '\0';
        printf("%s> NomPipe = %s\n%s",COLOR_CYAN,nomPipe,COLOR_WHITE);
        printf("%s> Commande = %s\n%s",COLOR_CYAN,commande,COLOR_WHITE);
        if (strcmp(commande,"fin\n") == 0){
            printf("%s> Fermeture du serveur\n%s",COLOR_CYAN,COLOR_WHITE);
            sleep(1);
            close(fdREad);
            exit(0);
        } else {
            fdWrite = open(nomPipe, O_WRONLY);
            char * message = "Jai recu la commande";
            printf("Jai recu la commande et cest %s\n",commande);
            write(fdWrite,message,strlen(message) + 1);
        }
        count++;

    }

    return 0;
}

