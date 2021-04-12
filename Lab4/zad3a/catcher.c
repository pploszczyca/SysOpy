#include "common.c"

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    HANDLE_SIGNALS = 0;

    chooseSignalMethod((int)sig_info->si_pid, countSignals);
}

int main(int argc, char *argv[]){
    sigset_t mask;
    signalSenderType = argv[1];
    int startSignal, endSignal;

    setStartEndSignal(&startSignal, &endSignal);

    printf("%d\n", (int) getpid());

    signal(startSignal, handleSignal);

    setMasksAndHandlers(startSignal, endSignal, &mask);

    while(HANDLE_SIGNALS == 1){
        sigsuspend(&mask);
    }

    printf("Number of received signals: %d\n", countSignals);
    
    return 0;
}