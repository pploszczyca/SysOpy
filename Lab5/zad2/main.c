#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1000

void printEmailAdresses(char *typeOfPrint){
    char buffer[MAX_BUFFER_SIZE];
    strcpy(buffer, "");
    if(strcmp(typeOfPrint, "nadawca") == 0){
        FILE *mail = popen("mail -p | grep From: | grep -o [a-z,A-Z]*@[a-z,A-Z,.]* | sort", "r");
        fread(buffer, sizeof(char), MAX_BUFFER_SIZE, mail);
        printf("%s\n", buffer);
        pclose(mail);
    } else if (strcmp(typeOfPrint, "data") == 0) {
        
    }
}

void sendEmail(char *adresEmail, char *title, char *content){
    char buffer[MAX_BUFFER_SIZE];
    sprintf(buffer, "mail -s \"%s\" \"%s\"", title, adresEmail);
    FILE *mail = popen(buffer, "w");
    fputs(content, mail);
    pclose(mail);
}


int main(int argc, char *argv[]){
    if(!(argc == 2 || argc == 4)){
        printf("Bad ammount of arguments\n");
        exit(1);
    } else if(argc == 2){
        printEmailAdresses(argv[1]);
    } else {
        sendEmail(argv[1], argv[2], argv[3]);
    }

    return 0;
}