#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int maein(int argc, char ** argv){
//    printf("%s", argv[0]);


    int location = 0;
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i],"|") == 0){
            location = i;
            break;
        }
    }

    char firstPart[100];

    for (int i = 1; i != location; i++){
//        strcpy(firstPart[i-1].argv[i]);
    }
    char* secondPart = argv[location + 1];

    printf("%s",firstPart);
    printf("%s",secondPart);

    return 0;





}