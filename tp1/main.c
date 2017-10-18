#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <ctype.h>


const char *ERR_OPTION_NO_VALID = "Votre choix n'est pas valide!\n"; //-1
const char *ERR_AMOUNT_NO_VALID = "Votre montant n'est pas valide!\n"; //-1
const char *ERR_ACCOUNT_NO_VALID = "Votre numero de compte n'est pas valide!\n"; //-1
const char *ERR_RECORD_LOCKED = "L'enregistrement que vous voulez acceder est verrouille.\n"; //-1
const char *ERR_AMOUNT_LOCKED = "Le solde que vous voulez acceder est verrouille.\n"; //-1
const char *ERR_FILE_LOCKED = "Le fichier est verrouille, veuillez essayer plus tard.\n"; //-1
char *WHICH_TO_DELETE = "Quel compte voulez vous supprimer"; //-1
char *WHICH_ACCOUNT = "Veuillez ecrire le numero du compte"; //-1
char *HOW_MUCH_WITHDRAW = "Combien voulez vous retirer?"; //-1
char *HOW_MUCH_DEPOSIT = "Combien voulez vous deposer?"; //-1
char *WE_DEPOSIT = "On va deposer"; //-1
char *WE_WITHDRAW = "On va retirer"; //-1
char *ATTRIBUTS = "CODE\t\t|NOM\t\t|PRENOM\t\t|SOLDE\n"; //-1

const char *BD_FILE_NAME = "bd.txt";


int verifyOption(char *option);

int verifyOptionNumeroCompte(char *option);

int verifyAmount(char *option, int lowRange, int highRange, char *message);

char *inputUser(char *question);

char *itoa(int i, char b[]);

int showMenu();

int optionChoice(int option);


int resetWord(char *);


int consultAccount();

int depositAccount();

int withdrawAccount();

int deleteAccount();

int attributsOption();
int showAccountAttribut();

int main() {
    int validation = -1;
    int done = 0;


    while (done == 0) {
        validation = -1;
        showMenu();
        char *option;
        while (validation <= 0) {
            option = inputUser("Choix");
            validation = verifyOption(option);
        }
        done = optionChoice(validation);
    }

    return 0;
}


int consultAccount() {
    int validation = -1;
    char *option = NULL;

    while (validation <= 0 || validation > 100) {
        option = inputUser(WHICH_ACCOUNT);
        validation = verifyOptionNumeroCompte(option);
    }
    int fileD = open(BD_FILE_NAME, O_RDWR);


    long int byteDebutLigne = 0;
    long int byteFinLigne = 0;
    long int byteDebutSolde = 0;


    FILE *file = fdopen(fileD, "w+");

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;


    byteDebutLigne = ftell(file);

    char c = (char) fgetc(file);
    char mot[50];
    char code[5];
    char nom[100];
    char prenom[100];
    char balance[100000];
    int n = 0;
    int virguleCount = 0;
    while (c != EOF) {

        if (virguleCount == 4) {
            break;
        }
        if (c == ',') {
            byteFinLigne = ftell(file);
            switch (virguleCount) {
                case 0  :
                    strcpy(code, mot);
                    resetWord(mot);
                    n = 0;
                    break;
                case 1  :
                    if (strcmp(code, option) == 0) {
                        lock.l_start = byteDebutLigne;
                        lock.l_len = byteFinLigne - byteDebutLigne;
                        if (fcntl(fileD, F_SETLK, &lock)) {
                            printf("%s", ERR_RECORD_LOCKED);
                            return 0;
                        }
                        strcpy(nom, mot);
                        resetWord(mot);
                        n = 0;
                        c = (char) fgetc(file);
                        virguleCount++;
                        while (c != EOF) {
                            if (c == ',') {
                                if (virguleCount != 3) {
                                    strcpy(prenom, mot);
                                }
                                n = 0;
                                virguleCount++;
                            }
                            else if (c == '\n') {
                                byteFinLigne = ftell(file);
                                mot[n] = '\0';
                                strcpy(balance, mot);
                                lock.l_len = byteFinLigne - byteDebutLigne;
                                break;
                            }
                            else {
                                mot[n] = c;
                                n++;
                            }
                            c = (char) fgetc(file);
                        }

                    }
                    else {
                        break;
                    }
                case 2  :
                    if (virguleCount != 3) {
                        strcpy(prenom, mot);
                    }
                    break;
                case 3 :
                    strcpy(balance, mot);
                    break;
            }
            virguleCount++;


        }
        else if (c == '\n') {
            mot[n] = '\0';
            n = 0;
            virguleCount = 0;
            resetWord(mot);
            byteDebutLigne = ftell(file);
        }
        else {
            mot[n] = c;
            n++;
        }

        c = (char) fgetc(file);
    }

    if (fcntl(fileD, F_SETLK, &lock)) {
        printf("%s", ERR_AMOUNT_LOCKED);
        return 0;
    }

    printf("%s", ATTRIBUTS);
    printf("%s\t\t|%s\t\t|%s\t\t|%s\t\t\n", code, nom, prenom, balance);


    fclose(file);
    close(fileD);

    return 0;


}

int resetWord(char *mot) {
    int i = 0;
    for (; i < 50; i++) {
        mot[i] = '\0';
    }
    return 0;
}

int withdrawAccount() {
    int validation = -1;
    char *option = NULL;
    char codeInput[1000];
    while (validation <= 0 || validation > 100) {
        option = inputUser(WHICH_ACCOUNT);
        validation = verifyOptionNumeroCompte(option);
    }
    int fileD = open(BD_FILE_NAME, O_RDWR);
    strcpy(codeInput,option);


    long int byteDebutLigne = 0;
    long int byteFinLigne = 0;
    long int byteDebutSolde = 0;


    FILE *file = fdopen(fileD, "w+");

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;


    byteDebutLigne = ftell(file);

    char c = (char) fgetc(file);
    char mot[50];
    char code[5];
    char nom[100];
    char prenom[100];
    char balance[100000];
    int n = 0;
    int virguleCount = 0;
    while (c != EOF) {

        if (virguleCount == 4) {
            break;
        }
        if (c == ',') {
            byteFinLigne = ftell(file);
            switch (virguleCount) {
                case 0  :
                    strcpy(code, mot);
                    resetWord(mot);
                    n = 0;
                    break;
                case 1  :
                    if (strcmp(code, option) == 0) {
                        lock.l_start = byteDebutLigne;
                        lock.l_len = byteFinLigne - byteDebutLigne;
                        if (fcntl(fileD, F_SETLK, &lock)) {
                            printf("%s", ERR_RECORD_LOCKED);
                            return 0;
                        }
                        strcpy(nom, mot);
                        resetWord(mot);
                        n = 0;
                        c = (char) fgetc(file);
                        virguleCount++;
                        while (c != EOF) {
                            if (c == ',') {
                                if (virguleCount != 3) {
                                    strcpy(prenom, mot);
                                }
                                n = 0;
                                virguleCount++;
                                byteDebutSolde = ftell(file);


                            }
                            else if (c == '\n') {
                                byteFinLigne = ftell(file);
                                mot[n] = '\0';
                                strcpy(balance, mot);
                                lock.l_len = byteFinLigne - byteDebutLigne;
                                printf("Solde Courant : %s\n", balance);
                                break;
                            }
                            else {
                                mot[n] = c;
                                n++;
                            }
                            c = (char) fgetc(file);
                        }

                    }
                    else {
                        break;
                    }
                case 2  :
                    if (virguleCount != 3) {
                        strcpy(prenom, mot);
                    }
                    break;
                case 3 :
                    strcpy(balance, mot);
                    break;
            }
            virguleCount++;


        }
        else if (c == '\n') {
            mot[n] = '\0';
            n = 0;
            virguleCount = 0;
            resetWord(mot);
            byteDebutLigne = ftell(file);

        }
        else {
            mot[n] = c;
            n++;
        }

        c = (char) fgetc(file);
    }

    if (fcntl(fileD, F_SETLK, &lock)) {
        printf("%s", ERR_AMOUNT_LOCKED);
        return 0;
    }

    int balanceInt = atoi(balance);
    validation = -1;
    while (validation == -1) {
        option = inputUser(HOW_MUCH_WITHDRAW);
        validation = verifyAmount(option, 0, balanceInt, WE_WITHDRAW);
        if (validation != -1){
            if ((balanceInt - validation) < 0){
                printf("Ne peut pas etre au dessous de 0 apres le changement\n");
                validation = -1;
            }
        }
    }

    char newAmount[10];
    itoa((balanceInt - validation), newAmount);
    fclose(file);
    file = fopen(BD_FILE_NAME,"r");

    FILE * newFile = fopen("temp.txt", "w+");
    c = fgetc(file);
    virguleCount = 0;
    resetWord(mot);
    n = 0;
    int done = -1;
    if (c != EOF) {
        while (done != 0) {
            if (c == EOF) {
                if (strcmp(code, codeInput) == 0) {
                    char buf[256];
                    snprintf(buf, sizeof buf, "%s,%s,%s,%s\n", code, nom, prenom, newAmount);
                    fputs(buf,newFile);
                }
                else {
                    fputs(mot, newFile);

                }
                break;

            }
            else if (c == ',' && virguleCount == 0) {
                strcpy(code, mot);
                mot[n] = c;
                n++;
                virguleCount++;
            }
            else if (c == '\n') {
                mot[n] = '\n';
                if (strcmp(code, codeInput) == 0) {
                    char buf[256];
                    snprintf(buf, sizeof buf, "%s,%s,%s,%s\n", code, nom, prenom, newAmount);
                    fputs(buf,newFile);
                }
                else {
                    fputs(mot, newFile);
                }
                virguleCount = 0;
                n = 0;
                resetWord(mot);
            }
            else {
                mot[n] = c;
                n++;
            }

            c = (char) fgetc(file);

        }
    }

    fclose(newFile);
    fclose(file);


    printf("%s", ATTRIBUTS);
    printf("%s\t\t|%s\t\t|%s\t\t|%s\t\t\n", code, nom, prenom, newAmount);

    rename("temp.txt", BD_FILE_NAME);
    remove("temp.txt");

    close(fileD);

    return 0;


}

int depositAccount() {
    int validation = -1;
    char *option = NULL;
    char codeInput[1000];


    while (validation <= 0 || validation > 100) {
        option = inputUser(WHICH_ACCOUNT);
        validation = verifyOptionNumeroCompte(option);
    }
    int fileD = open(BD_FILE_NAME, O_RDWR);
    strcpy(codeInput,option);

    long int byteDebutLigne = 0;
    long int byteDebutSolde = 0;


    FILE *file = fdopen(fileD, "w+");
    FILE *newFile = fopen("temp.txt", "w+");

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;


    byteDebutLigne = ftell(file);
    lock.l_start = byteDebutLigne;
    char c = (char) fgetc(file);
    char mot[50];
    char code[5];
    char nom[100];
    char prenom[100];
    char balance[100000];
    int n = 0;
    int virguleCount = 0;
    while (c != EOF) {

        if (virguleCount == 4) {
            break;
        }
        if (c == ',') {
            switch (virguleCount) {
                case 0  :
                    strcpy(code, mot);
                    resetWord(mot);
                    n = 0;
                    break;
                case 1  :
                    if (strcmp(code, option) == 0) {
                        strcpy(nom, mot);
                        resetWord(mot);
                        n = 0;
                        c = (char) fgetc(file);
                        virguleCount++;
                        while (c != EOF) {
                            if (c == ',') {
                                if (virguleCount != 3) {
                                    strcpy(prenom, mot);
                                }
                                n = 0;

                                virguleCount++;
                                byteDebutSolde = ftell(file);
                                lock.l_start = byteDebutSolde;

                            }
                            else if (c == '\n') {
                                mot[n] = '\0';
                                strcpy(balance, mot);
                                lock.l_len = n;
                                break;
                            }
                            else {
                                mot[n] = c;
                                n++;
                            }
                            c = (char) fgetc(file);
                        }

                    }
                    else {
                        break;
                    }
                case 2  :
                    if (virguleCount != 3) {
                        strcpy(prenom, mot);
                    }
                    break;
                case 3 :
                    strcpy(balance, mot);
                    break;
            }
            virguleCount++;


        }
        else if (c == '\n') {
            mot[n] = '\0';
            n = 0;
            virguleCount = 0;
            resetWord(mot);
            byteDebutLigne = ftell(file);
            lock.l_start = byteDebutLigne;

        }
        else {
            mot[n] = c;
            n++;
        }

        c = (char) fgetc(file);
    }

    if (fcntl(fileD, F_SETLK, &lock)) {
        printf("%s", ERR_AMOUNT_LOCKED);
        return 0;
    }

    printf("Solde Courant : %s\n", balance);

    int balanceInt = atoi(balance);
    validation = -1;
    while (validation == -1) {
        option = inputUser(HOW_MUCH_DEPOSIT);
        validation = verifyAmount(option, 0, 10000, WE_DEPOSIT);
        if (validation != -1){
            if ((balanceInt + validation) > 10000){
                printf("Ne peut pas acceder 10000$ apres le changement\n");
                validation = -1;
            }
        }
    }


    char newAmount[10];
    itoa((balanceInt + validation), newAmount);

    fclose(file);
    file = fopen(BD_FILE_NAME,"r");

    c = fgetc(file);
    virguleCount = 0;
    resetWord(mot);
    n = 0;
    int done = -1;
    if (c != EOF) {
        while (done != 0) {
            if (c == EOF) {
                if (strcmp(code, codeInput) == 0) {
                    char buf[256];
                    snprintf(buf, sizeof buf, "%s,%s,%s,%s\n", code, nom, prenom, newAmount);
                    fputs(buf,newFile);
                }
                else {
                    fputs(mot, newFile);

                }
                break;

            }
            else if (c == ',' && virguleCount == 0) {
                strcpy(code, mot);
                mot[n] = c;
                n++;
                virguleCount++;
            }
            else if (c == '\n') {
                mot[n] = '\n';
                if (strcmp(code, codeInput) == 0) {
                    char buf[256];
                    snprintf(buf, sizeof buf, "%s,%s,%s,%s\n", code, nom, prenom, newAmount);
                    fputs(buf,newFile);
                }
                else {
                    fputs(mot, newFile);
                }
                virguleCount = 0;
                n = 0;
                resetWord(mot);
            }
            else {
                mot[n] = c;
                n++;
            }

            c = (char) fgetc(file);

        }
    }

    fclose(newFile);
    fclose(file);


    printf("%s", ATTRIBUTS);
    printf("%s\t\t|%s\t\t|%s\t\t|%s\t\t\n", code, nom, prenom, newAmount);

    rename("temp.txt", BD_FILE_NAME);
    remove("temp.txt");

    close(fileD);

    return 0;


}

int deleteAccount() {
    int validation = -1;
    char *option = NULL;

    while (validation <= 0 || validation > 100) {
        option = inputUser(WHICH_TO_DELETE);
        validation = verifyOptionNumeroCompte(option);
    }
    int fileD = open(BD_FILE_NAME, O_RDWR);


    FILE *file = fdopen(fileD, "r");
    FILE *newFile = fopen("temp.txt", "w+");

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = SEEK_SET;
    lock.l_len = 0;

    if (fcntl(fileD, F_SETLK, &lock)) {
        printf("%s", ERR_FILE_LOCKED);
        return 0;
    }

    char c = (char) fgetc(file);
    char mot[50];
    char codeDetruit[5];
    char code[5];
    int n = 0;
    int virguleCount = 0;
    int done = -1;
    if (c != EOF) {
        while (done != 0) {
            if (c == EOF) {
                if (strcmp(code, option) == 0) {
                    strcpy(codeDetruit, code);

                }
                else {
                    fputs(mot, newFile);

                }
                break;

            }
            else if (c == ',' && virguleCount == 0) {
                strcpy(code, mot);
                mot[n] = c;
                n++;
                virguleCount++;
            }
            else if (c == '\n') {
                mot[n] = '\n';
                if (strcmp(code, option) == 0) {
                    strcpy(codeDetruit, code);
                }
                else {
                    fputs(mot, newFile);
                }
                virguleCount = 0;
                n = 0;
                resetWord(mot);
            }
            else {
                mot[n] = c;
                n++;
            }

            c = (char) fgetc(file);

        }
    }


    printf("Le compte %s est detruit\n", codeDetruit);


    fclose(file);
    fclose(newFile);

    rename("temp.txt", BD_FILE_NAME);
    remove("temp.txt");
    close(fileD);

    return 0;


}

int attributsOption() {
    int validation = -1;
    char *option = NULL;

    while (validation <= 0 || validation > 100) {
        option = inputUser(WHICH_ACCOUNT);
        validation = verifyOptionNumeroCompte(option);
    }


    int fileD = open(BD_FILE_NAME, O_RDWR);


    long int byteDebutLigne = 0;
    long int byteFinLigne = 0;
    long int byteDebutSolde = 0;


    FILE *file = fdopen(fileD, "w+");

    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;


    byteDebutLigne = ftell(file);

    char c = (char) fgetc(file);
    char mot[50];
    char code[5];
    char nom[100];
    char prenom[100];
    char balance[100000];
    int n = 0;
    int virguleCount = 0;
    while (c != EOF) {

        if (virguleCount == 4) {
            break;
        }
        if (c == ',') {
            byteFinLigne = ftell(file);
            switch (virguleCount) {
                case 0  :
                    strcpy(code, mot);
                    resetWord(mot);
                    n = 0;
                    break;
                case 1  :
                    if (strcmp(code, option) == 0) {
                        lock.l_start = byteDebutLigne;
                        lock.l_len = byteFinLigne - byteDebutLigne;
                        if (fcntl(fileD, F_SETLK, &lock)) {
                            printf("%s", ERR_RECORD_LOCKED);
                            return 0;
                        }
                        strcpy(nom, mot);
                        resetWord(mot);
                        n = 0;
                        c = (char) fgetc(file);
                        virguleCount++;
                        while (c != EOF) {
                            if (c == ',') {
                                if (virguleCount != 3) {
                                    strcpy(prenom, mot);
                                }
                                n = 0;
                                virguleCount++;
                            }
                            else if (c == '\n') {
                                byteFinLigne = ftell(file);
                                mot[n] = '\0';
                                strcpy(balance, mot);
                                lock.l_len = byteFinLigne - byteDebutLigne;
                                break;
                            }
                            else {
                                mot[n] = c;
                                n++;
                            }
                            c = (char) fgetc(file);
                        }

                    }
                    else {
                        break;
                    }
                case 2  :
                    if (virguleCount != 3) {
                        strcpy(prenom, mot);
                    }
                    break;
                case 3 :
                    strcpy(balance, mot);
                    break;
            }
            virguleCount++;


        }
        else if (c == '\n') {
            mot[n] = '\0';
            n = 0;
            virguleCount = 0;
            resetWord(mot);
            byteDebutLigne = ftell(file);
        }
        else {
            mot[n] = c;
            n++;
        }

        c = (char) fgetc(file);
    }

    if (fcntl(fileD, F_SETLK, &lock)) {
        printf("%s", ERR_AMOUNT_LOCKED);
        return 0;
    }

    showAccountAttribut();
    while (strcmp(option, "N") != 0 &&
           strcmp(option, "P") != 0 &&
           strcmp(option, "S") != 0) {
        option = inputUser(WHICH_ACCOUNT);
    }

    if (strcmp(option, "N") == 0) {
        printf("%s", "|\t\tNOM\t\t|\n");
        printf("|\t\t%s\t\t|\n", nom);
    }
    else if (strcmp(option, "P") == 0) {
        printf("%s", "|\t\tPRENOM\t\t|\n");
        printf("|\t\t%s\t\t|\n", prenom);
    }
    else if (strcmp(option, "S") == 0) {
        printf("%s", "|\t\tSOLDE\t\t|\n");
        printf("|\t\t%s\t\t|\n", balance);
    }


    fclose(file);
    close(fileD);

    return 0;
}


int showMenu() {

    printf("===========================================================================\n");
    printf("MonGestionnaireCompteBancaire\n");
    printf("1- Consulter compte\n");
    printf("2- Retirer Compte\n");
    printf("3- Deposer Compte\n");
    printf("4- Detruire Compte\n");
    printf("5- Afficher attribut\n");
    printf("6- FIN\n");
    printf("===========================================================================\n");

    return 0;
}

int showAccountAttribut() {
    printf("N- Nom\n");
    printf("P- Prenom\n");
    printf("S- Solde\n");
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

int optionChoice(int option) {
    switch (option) {
        case 1 :
            return consultAccount();
        case 2 :
            return withdrawAccount();
        case 3 :
            return depositAccount();
        case 4 :
            return deleteAccount();
        case 5 :
            return attributsOption();
        case 6 :
            return 1;
        default:
            exit(0);

    }
}


char *inputUser(char *question) {

    char *input = (char *) malloc(sizeof(char) * 50);
    printf("%s : ", question);
    scanf("%s", input);
    return input;
}

int verifyOption(char *option) {
    if (strlen(option) != 1) {
        printf("%s", ERR_OPTION_NO_VALID);
        return -1;
    }
    if (isdigit(option[0])) {
        int choixInt = atoi(option);
        return choixInt;
    }
    else {
        printf("%s", ERR_OPTION_NO_VALID);
        return -1;
    }
}

int verifyOptionNumeroCompte(char *option) {
    if (isdigit(option[0])) {
        int choixInt = atoi(option);
        return choixInt;
    }
    else {
        printf("%s", ERR_ACCOUNT_NO_VALID);
        return -1;
    }
}

int verifyAmount(char *option, int lowRange, int highRange, char *message) {
    if (isdigit(option[0])) {
        int choixInt = atoi(option);
        if (choixInt > lowRange && choixInt <= highRange) {
            printf("%s", message);
            printf(" %d$ de votre compte\n", choixInt);
            return choixInt;
        }
        else {
            printf("%s", ERR_AMOUNT_NO_VALID);
            return -1;
        }
    }

}
