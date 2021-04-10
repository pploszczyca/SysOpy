#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

void printInfoFromSigInfo_t(siginfo_t *sig_info){
    printf("Signal number: %d\nUser ID: %d\nSignal code: %d\nSending process PID: %d\nExit value: %d\n", sig_info->si_signo, (int)sig_info->si_uid ,sig_info->si_code, (int)sig_info->si_pid, sig_info->si_status);
}

void SIGUSR1Handler(int sig_no, siginfo_t *sig_info, void *ucontext){
    printInfoFromSigInfo_t(sig_info);

    sigset_t newMask, oldMask;
    sigemptyset(&newMask);
    sigemptyset(&oldMask);

    sigprocmask(SIG_UNBLOCK, &newMask, &oldMask);
    if(sigismember(&oldMask,SIGUSR1)){
        printf("SIGUSR1 is masked by default.\n");
    } else {
        printf("SIGUSR1 is NOT masked by default.\n");
    }

    printf("\n");
}

void SIGCHLDHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    printInfoFromSigInfo_t(sig_info);

    printf("\n");
}

int main(int argc, char *argv[]){
    printf("PID: %d\n", (int) getpid());

    struct sigaction actionSIGUSR1;
    actionSIGUSR1.sa_sigaction = SIGUSR1Handler;
    sigemptyset(&actionSIGUSR1.sa_mask);
    actionSIGUSR1.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NODEFER;
    sigaction(SIGUSR1, &actionSIGUSR1, NULL);

    printf("SA_NODEFER ON\n");
    kill((int) getpid(), SIGUSR1);

    actionSIGUSR1.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
    sigaction(SIGUSR1, &actionSIGUSR1, NULL);
    
    printf("SA_NODEFER OFF\n");
    kill((int) getpid(), SIGUSR1);


    struct sigaction actionSIGCHLD;
    actionSIGCHLD.sa_sigaction = SIGCHLDHandler;
    sigemptyset(&actionSIGCHLD.sa_mask);
    actionSIGCHLD.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NODEFER;
    sigaction(SIGCHLD, &actionSIGCHLD, NULL);

    printf("SA_NOCLDSTOP ON\n");
    pid_t child_pid = fork();

    if(child_pid == 0){
        kill((int) getpid(), SIGSTOP);
    }

    sleep(1);
    actionSIGCHLD.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIGCHLD, &actionSIGCHLD, NULL);

    printf("SA_NOCLDSTOP OFF\n");
    child_pid = fork();

    if(child_pid == 0){
        kill((int) getpid(), SIGSTOP);
    }

    sleep(1);

    return 0;
}