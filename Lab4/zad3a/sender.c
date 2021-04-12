#include "common.c"

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    HANDLE_SIGNALS = 0;
    printf("Number of received signals: %d\n", countSignals);

    if(strcmp(signalSenderType, "SIGQUEUE") == 0){
        printf("Number of signals that catcher received: %d\n", sig_info->si_value.sival_int);
    }
}

int main(int argc, char *argv[]){
    int startSignal, endSignal, nOfSignals, catcherPID;
    sigset_t mask;
    
    if(argc != 4){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    catcherPID = atoi(argv[1]);
    nOfSignals = atoi(argv[2]);
    signalSenderType = argv[3];

    setStartEndSignal(&startSignal, &endSignal);

    setMasksAndHandlers(startSignal, endSignal, &mask);
    
    chooseSignalMethod(catcherPID, nOfSignals);

    while(HANDLE_SIGNALS == 1);

    return 0;
}