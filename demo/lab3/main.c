#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <wait.h>


int piddd();

int execccc();

int main() {
//    piddd();
    execccc();

}

int piddd(){
    int i = 0;
    int pids[10];

    while (i < 10){
        int forkId = fork();
        pids[i] = forkId;
        if (forkId == 0){
            printf("Je suis le numero %d et mon pID est %d, et mon parent est %d\n",i,getpid(),getppid());
            exit(0);
        }
        i++;
    }
    int etat;

    for (int j = 0;j < 10; j++){
        waitpid(pids[j],&etat,NULL);
        printf("Le processus %d est fini et son pid est %d\n",j,pids[j]);
    }

    return 0;
}

int execccc(){
    int pids[10];

    for (int i = 0; i < 4;i++){
        int pid = fork();
        pids[i] = pid;
        if (pid == 0){
            execl("/bin/ls","ls","-l",NULL);
            exit(0);
        }
    }

    int etat;
    for (int i = 0; i < 4;i++){
        waitpid(pids[i],&etat,NULL);
        printf("The process is finished\n");
    }




    return 0;
}