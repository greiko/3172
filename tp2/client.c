#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define COLOR_YELLOW                    "\x1B[33m"
#define COLOR_GREEN                     "\x1B[32m"
#define COLOR_RED                       "\x1b[31m"

#define CLEAR                           "\e[H\e[2J"

#define COLOR_WHITE                     "\x1B[37m"

#define CLOSING_COMMAND_SERVER          "> VOUS AVEZ ENVOYE LA COMMANDE DE FERMETURE DE SERVEUR"
#define MAXIMUM_REQUEST_REACHED         "> MAXIMUM DE REQUETES ATTEINTE"
#define SERVER_CLOSED                   "> SERVEUR FERMER"
#define CONNECTION_ESTABLISHED          "> CONNECTION ETABLI AVEC LE SERVEUR"
#define REQUEST_NUMBER                  "> NUMERO DE REQUETE : "
#define WRITE_COMMAND                   "> ECRIRE VOTRE COMMANDE : "
#define SERVER_CLOSES_WHILE_INPUT       "> SERVEUR FERMER PENDANT VOTRE L'ATTENTE DE SAISI DE COMMANDE"

#define PIPE_SERVER_SRC                 "serveurP"
#define PIPE_CLIENT_SRC                 "pipe"


char *concat(const char *, const char *);

char *itoa(int i, char b[]);

int main() {

    int fileDescriptor;

    char buffer[BUFSIZ];
    char buffer2[BUFSIZ];

    int requestNumber = 1;

    char *pipeName;
    int index = 0;
    char indexString[100];
    while (1) {
        itoa(index, indexString);
        pipeName = concat(PIPE_CLIENT_SRC, indexString);

        if (access(pipeName, F_OK) == -1) {
            break;
        } else {
            index++;
        }
    }

    printf("\\033[41m%s",CLEAR);


    while (1) {
        if (requestNumber >= 100){
            printf("%s%s\n",COLOR_GREEN,MAXIMUM_REQUEST_REACHED);
            exit(0);
        }
        if (access(PIPE_SERVER_SRC, F_OK) == -1) {
            printf("%s%s\n%s",COLOR_RED,SERVER_CLOSED,COLOR_WHITE);
            exit(0);
        } else {
            printf("%s%s\n%s",COLOR_GREEN,CONNECTION_ESTABLISHED,COLOR_WHITE);
        }

        mkfifo(pipeName, 0666);


        printf("%s%s%d\n%s",COLOR_GREEN,REQUEST_NUMBER,requestNumber,COLOR_WHITE);
        printf("%s",WRITE_COMMAND);
        fgets(buffer, BUFSIZ, stdin);
        printf("\n");

        if (strcmp(buffer,"\n") == 0) {
            remove(pipeName);
            exit(0);
        }

        if (access(PIPE_SERVER_SRC, F_OK) == -1) {
            printf("%s%s%s\n",COLOR_RED,SERVER_CLOSES_WHILE_INPUT,COLOR_WHITE);
            remove(pipeName);
            exit(0);
        }


        fileDescriptor = open(PIPE_SERVER_SRC, O_WRONLY);



        char *message = concat(pipeName, ",");


        char temp[BUFSIZ];
        strcpy(temp, message);

        char bufferToSend[BUFSIZ];
        char *temp2 = concat(temp, buffer);

        stpcpy(bufferToSend, temp2);

        write(fileDescriptor, bufferToSend, strlen(bufferToSend) + 1);
        close(fileDescriptor);
        requestNumber++;

        if (strcmp(buffer,"fin\n") == 0 || strcmp(buffer,"FIN\n") == 0){
            printf("%s%s%s\n",COLOR_GREEN,CLOSING_COMMAND_SERVER,COLOR_WHITE);
            remove(pipeName);
            exit(0);
        }


        while (1) {
            fileDescriptor = open(pipeName, O_RDONLY);
            read(fileDescriptor, buffer2, BUFSIZ);

            printf("%s====================================================\n", COLOR_YELLOW);
            printf("\tRESULTAT\n");
            printf("%s====================================================%s\n", COLOR_YELLOW, COLOR_GREEN);
            printf("%s\n" ,buffer2);
            printf("%s====================================================\n", COLOR_YELLOW);
            printf("\tFIN RESULTAT\n");
            printf("%s====================================================\n%s", COLOR_YELLOW, COLOR_WHITE);

            close(fileDescriptor);
            break;
        }
    }


    return 0;
}

char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *itoa(int i, char b[]) {
    char const digit[] = "0123456789";
    char *p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do {
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do {
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}
