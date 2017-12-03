#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_WHITE  "\x1B[37m"
#define COLOR_GREEN  "\x1B[32m"

#define CLEAR "\e[H\e[2J"

#define COLOR_RED     "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"

#define SERVEUR_UP "> SERVEUR UP"
#define COMMANDE_NON_VALIDE "\x1b[31mCOMMANDE NON VALIDE"
#define CONNECTION_ETABLI "> CONNECTION ETABLI - NUMERO DE REQUETE : "
#define NOM_PIPE "> NOM DE PIPE : "
#define COMMANDE_RECU "> COMMANDE RECU : "
#define FERMETURE_SERVEUR "> FERMETURE DU SERVEUR"


int main() {


    int fdREad;
    int fdWrite;

    char *pipeSrc = "serveurP";
    char *logSrc = "log.txt";

    FILE *logFile = fopen(logSrc, "w");

    mkfifo("serveurP", 0666);

    printf("\\033[41m%s", CLEAR);


    char buffer[BUFSIZ];
    int count = 1;


    fprintf(logFile, "%s%s\n%s", COLOR_GREEN, SERVEUR_UP, COLOR_WHITE);
    printf("%s%s\n%s", COLOR_GREEN, SERVEUR_UP, COLOR_WHITE);

    while (1) {
        fdREad = open(pipeSrc, O_RDONLY);


        printf("%s%s%d\n%s", COLOR_GREEN, CONNECTION_ETABLI, count, COLOR_WHITE);

        fprintf(logFile,"%s==========================================================================================================\n", COLOR_YELLOW);
        fprintf(logFile,"%s%s%d\n%s", COLOR_GREEN, CONNECTION_ETABLI, count, COLOR_WHITE);
        fprintf(logFile,"%s==========================================================================================================\n", COLOR_YELLOW, COLOR_GREEN);

        read(fdREad, buffer, BUFSIZ);
        close(fdREad);

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
//        printf("%s%s%s\n%s", COLOR_GREEN, NOM_PIPE, nomPipe, COLOR_WHITE);
//        printf("%s%s%s\n%s", COLOR_GREEN, COMMANDE_RECU,commande, COLOR_WHITE);


        fprintf(logFile, "%s%s%s\n%s", COLOR_GREEN, NOM_PIPE, nomPipe, COLOR_WHITE);
        fprintf(logFile, "%s%s%s\n%s", COLOR_GREEN, COMMANDE_RECU, commande, COLOR_WHITE);

        if (strcmp(commande, "fin\n") == 0) {
            printf("%s%s\n\n\n\n%s", COLOR_GREEN, FERMETURE_SERVEUR, COLOR_WHITE);
            fprintf(logFile, "%s%s\n%s", COLOR_GREEN, FERMETURE_SERVEUR, COLOR_WHITE);
            remove(pipeSrc);
            sleep(1);
            close(fdREad);
            fclose(logFile);

            int c;
            FILE *file;
            file = fopen(logSrc, "r");
            if (file) {
                while ((c = getc(file)) != EOF)
                    putchar(c);
                fclose(file);
            }
            exit(0);
        }

        char buffer[BUFSIZ];
        for (i = 0; i < BUFSIZ; i++) {
            buffer[i] = '\0';
        }
        FILE *commandeFile = popen(commande, "r");
        fread(buffer, BUFSIZ, sizeof(commandeFile), commandeFile);

        int pcloseStatus = pclose(commandeFile);


        fdWrite = open(nomPipe, O_WRONLY);
        if (pcloseStatus == 32512) {
            strcpy(buffer, COMMANDE_NON_VALIDE);
        } else {
//                printf("%s====================================================\n", COLOR_YELLOW);
//                printf("\tRESULTAT\n", COLOR_YELLOW);
//                printf("%s====================================================%s\n", COLOR_YELLOW, COLOR_GREEN);
//                printf("%s\n" ,buffer);
//                printf("%s====================================================\n", COLOR_YELLOW);
//                printf("\tFIN RESULTAT\n", COLOR_YELLOW);
//                printf("%s====================================================\n%s", COLOR_YELLOW, COLOR_WHITE);

            fprintf(logFile, "%s====================================================\n", COLOR_YELLOW);
            fprintf(logFile, "\tRESULTAT\n");
            fprintf(logFile, "%s====================================================%s\n", COLOR_YELLOW, COLOR_GREEN);
            fprintf(logFile, "%s\n", buffer);
        }
        write(fdWrite, buffer, strlen(buffer) + 1);

        count++;



        fprintf(logFile,"%s===========================================================================\n", COLOR_YELLOW);
        fprintf(logFile,"\tFIN DE REQUETE\n", COLOR_YELLOW);
        fprintf(logFile,"%s===========================================================================\n", COLOR_YELLOW, COLOR_GREEN);

    }

    return 0;
}

