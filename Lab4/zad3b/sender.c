#include "common.c"

void handleSignal(int sig_no, siginfo_t *sig_info, void *ucontext){
    countSignals++;
    
    if(countSignals != nSignalsToSend || HANDLE_SIGNALS != 1){
       kill((int) sig_info->si_pid, SIGUSR1); 
    } else {
        HANDLE_SIGNALS = 2;
        countSignals = 0;
        kill((int) sig_info->si_pid, SIGUSR2);
    }
}

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    HANDLE_SIGNALS = 0;
    printf("Number of received signals: %d\n", countSignals);
}

int main(int argc, char *argv[]){
    int catcherPID;
    sigset_t mask;
    
    if(argc != 4){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    catcherPID = atoi(argv[1]);
    nSignalsToSend = atoi(argv[2]);

    setMasksAndHandlers(SIGUSR1, SIGUSR2, &mask);

    kill(catcherPID, SIGUSR1);
    
    while(HANDLE_SIGNALS != 0);

    return 0;
}