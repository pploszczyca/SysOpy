#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char fifo_path[4] = "fif";
    mkfifo(fifo_path,S_IRUSR | S_IWUSR);
    char *producentArgv[] = {"./producent", fifo_path, "1", "p1.txt", "10", NULL};
    char *producent2Argv[] = {"./producent", fifo_path, "2", "p2.txt", "5", NULL};
    char *konsumentArgv[] = {"./konsument", fifo_path, "k1.txt", "20", NULL};

    if(fork() == 0) execvp("./konsument", konsumentArgv);
    if(fork() == 0) execvp("./producent", producentArgv);
    if(fork() == 0) execvp("./producent", producent2Argv);

    while(wait(NULL) > 0);
    return 0;
}