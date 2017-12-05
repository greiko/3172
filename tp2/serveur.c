#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define COLOR_YELLOW            "\x1B[33m"
#define COLOR_WHITE             "\x1B[37m"
#define COLOR_GREEN             "\x1B[32m"
#define COLOR_RED               "\x1b[31m"

#define CLEAR                   "\e[H\e[2J"

#define SERVER_UP               "> SERVEUR UP"
#define COMMAND_NO_VALID        "\x1b[31mCOMMANDE NON VALIDE"
#define CONNECTION_ESTABLISHED  "> CONNECTION ETABLI - NUMERO DE REQUETE : "
#define PIPE_NAME               "> NOM DE PIPE : "
#define COMMAND_RECEIVED        "> COMMANDE RECU : "
#define SERVER_CLOSING          "> FERMETURE DU SERVEUR"

#define PIPE_SERVER_SRC                "serveurP"
#define LOG_SRC                 "log.txt"

int checkCD(char *commande);

void printLogFile();


int main() {


    int fdServerRead;
    int fdServerWrite;
    char buffer[BUFSIZ];
    int count = 1;
    int pcloseStatus = 0;


    FILE *logFile = fopen(LOG_SRC, "w");
    mkfifo("serveurP", 0666);
    printf("\\033[41m%s", CLEAR); // Clear ton terminal pour un beau visuel

    fprintf(logFile, "%s%s\n%s", COLOR_GREEN, SERVER_UP, COLOR_WHITE);
    printf("%s%s\n%s", COLOR_GREEN, SERVER_UP, COLOR_WHITE);

    while (1) {
        fdServerRead = open(PIPE_SERVER_SRC, O_RDONLY);


        printf("%s%s%d\n%s", COLOR_GREEN, CONNECTION_ESTABLISHED, count, COLOR_WHITE);

        fprintf(logFile,
                "%s==========================================================================================================\n",
                COLOR_YELLOW);
        fprintf(logFile, "%s%s%d\n%s", COLOR_GREEN, CONNECTION_ESTABLISHED, count, COLOR_WHITE);
        fprintf(logFile,
                "%s==========================================================================================================\n",
                COLOR_YELLOW);

        read(fdServerRead, buffer, BUFSIZ);
        close(fdServerRead);

        char pipeNameClient[BUFSIZ];
        char command[BUFSIZ];
        int pipeNameDone = -1;
        int i, j;
        for (i = 0, j = 0; i < strlen(buffer); i++) {
            if (buffer[i] == ',') {
                pipeNameClient[j + 1] = '\0';
                j = 0;
                pipeNameDone = 0;
            } else {
                if (pipeNameDone == 0) {
                    command[j] = buffer[i];

                } else {
                    pipeNameClient[j] = buffer[i];

                }
                j++;
            }
        }
        command[j] = '\0';

        fprintf(logFile, "%s%s%s\n%s", COLOR_GREEN, PIPE_NAME, pipeNameClient, COLOR_WHITE);
        fprintf(logFile, "%s%s%s\n%s", COLOR_GREEN, COMMAND_RECEIVED, command, COLOR_WHITE);

        if (strcmp(command, "fin\n") == 0 || strcmp(command, "FIN\n") == 0) {
            printf("%s%s\n\n\n\n%s", COLOR_GREEN, SERVER_CLOSING, COLOR_WHITE);
            fprintf(logFile, "%s%s\n%s", COLOR_GREEN, SERVER_CLOSING, COLOR_WHITE);
            remove(PIPE_SERVER_SRC);
            sleep(1);
            close(fdServerRead);
            fclose(logFile);

            printLogFile();

            exit(0);
        } else if (checkCD(command) == 0) {
            system("gnome-terminal -e vi");
        } else {
            for (i = 0; i < BUFSIZ; i++) {
                buffer[i] = '\0';
            }
            FILE *commandeFile = popen(command, "r");
            fread(buffer, BUFSIZ, sizeof(commandeFile), commandeFile);
            pcloseStatus = pclose(commandeFile);


        }


        fdServerWrite = open(pipeNameClient, O_WRONLY);
        if (pcloseStatus == 32512) {
            strcpy(buffer, COMMAND_NO_VALID);
        } else {
            fprintf(logFile, "%s====================================================\n", COLOR_YELLOW);
            fprintf(logFile, "\tRESULTAT\n");
            fprintf(logFile, "%s====================================================%s\n", COLOR_YELLOW, COLOR_GREEN);
            fprintf(logFile, "%s\n", buffer);
        }
        write(fdServerWrite, buffer, strlen(buffer) + 1);
        count++;

    }

    return 0;
}

void printLogFile() {
    int c;
    FILE *file;
    file = fopen(LOG_SRC, "r");
    if (file) {
        while ((c = getc(file)) != EOF)
            putchar(c);
        fclose(file);
    }
}

int checkCD(char *commande) {
    if (commande[0] == 'v' && commande[1] == 'i') {
        return 0;
    }
    return -1;
}
