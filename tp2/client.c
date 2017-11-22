#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_WHITE  "\x1B[37m"

char *concat(const char *, const char *);

char *itoa(int i, char b[]);

int main() {

    int fileDescriptor;

    char *serveurSrc = "serveurP";
    char *pipeSrc = "pipe";


    char buffer[250];
    char buffer2[250];

    int numeroRequete = 1;

    char *pipeName;
    int index = 0;
    char indexString[100];
    while (1) {
        itoa(index, indexString);
        pipeName = concat(pipeSrc, indexString);

        if (access(pipeName, F_OK) == -1) {
            printf("Pipe non existant\n");
            break;
        } else {
            printf("Pipe existant\n");
            index++;
        }
    }



    while (1) {
        if (access(serveurSrc, F_OK) == -1) {
            printf("Serveur fermer\n");
            exit(0);
        } else {
            printf("Connection serveur established\n");
        }

        mkfifo(pipeName, 0666);


        printf("Numero requete : %d\n",numeroRequete);
        printf("Ecrire commande: \n");
        fgets(buffer, 250, stdin);

        if (strcmp(buffer,"\n") == 0){
            remove(pipeName);
            exit(0);
        } else if (strcmp(buffer,"fin\n") == 0){
            printf("Vous avez envoye la commande de fin de serveur");
            remove(pipeName);
            exit(0);
        }

        if (access(serveurSrc, F_OK) == -1) {
            printf("Serveur fermer pendant votre input\n");
            exit(0);
        }

        fileDescriptor = open(serveurSrc, O_WRONLY);



        char *message = concat(pipeName, ",");


        char temp[250];
        strcpy(temp, message);

        char bufferToSend[250];
        char *temp2 = concat(temp, buffer);

        stpcpy(bufferToSend, temp2);

        write(fileDescriptor, bufferToSend, strlen(bufferToSend) + 1);
        close(fileDescriptor);
        sleep(1);
        numeroRequete++;


        while (1) {
            fileDescriptor = open(pipeName, O_RDONLY);
            read(fileDescriptor, buffer2, 250);

            printf("%s\n", buffer2);

            close(fileDescriptor);
            break;
        }
    }


    return 0;
}

char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
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
