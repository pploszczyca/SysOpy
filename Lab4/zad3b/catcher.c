#include "common.c"

void handleSignal(int sig_no, siginfo_t *sig_info, void *ucontext){
    countSignals++;

    if(countSignals != nSignalsToSend || HANDLE_SIGNALS != 2){
       kill((int) sig_info->si_pid, SIGUSR1); 
    } else {
        kill((int) sig_info->si_pid, SIGUSR2);
        HANDLE_SIGNALS = 0;
    }
}

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    printf("Number of received signals: %d\n", countSignals);
    nSignalsToSend = countSignals;
    countSignals = 0;
    HANDLE_SIGNALS = 2;
    kill((int) sig_info->si_pid, SIGUSR1);
}

int main(int argc, char *argv[]){
    sigset_t mask;

    printf("%d\n", (int) getpid());

    setMasksAndHandlers(SIGUSR1, SIGUSR2, &mask);

    while(HANDLE_SIGNALS != 0);
    
    return 0;
}