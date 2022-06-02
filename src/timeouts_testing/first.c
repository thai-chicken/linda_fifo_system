#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<stdlib.h>
#include <string.h>
#include <sys/wait.h>

static void handler(int sig, siginfo_t *si, void *uap)
{
  printf("\tsending process ID: si_pid=%d\n", si->si_pid);
  waitpid(si->si_pid, NULL, 0);
}

void sig_handler(int signum){
    printf("Timeouted!\n");
    pid_t ppid = getppid();
    kill(ppid, SIGUSR1);
}

void sig_usr_handler(int signum){
    printf("Received sigusr!\n");
}
 
int main(){
    struct sigaction sa;
    pid_t childpid = fork();         
    if (childpid < 0){
        printf("Error!\n");
        return -1;
    }
    else if (childpid == 0){
        // potomny
        signal(SIGALRM,sig_handler); // Register signal handler
        alarm(5);  // Scheduled alarm after 2 seconds
        pause();
        printf("Child1 finished!\n");
        exit(0);
    }
    else{
        pid_t childpid_2 = fork();
        if (childpid_2 < 0){
            printf("Error 2!\n");
            return -1;
        }
        else if (childpid_2 == 0){
            // drugi potomny
            signal(SIGALRM,sig_handler); // Register signal handler
            alarm(3);  // Scheduled alarm after 2 seconds
            pause();
            printf("Child2 finished!\n");
            exit(0);
        }
        else{
            // macierzysty
            signal(SIGUSR1,sig_usr_handler);
            int x= 0;
            int pid;
            memset(&sa, 0, sizeof(sa));
            sa.sa_sigaction = handler;
            sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDSTOP;
            sigaction(SIGCHLD, &sa, NULL);
            while(1);
            // {
            //     x+=1;
            //     if (x%150000000 == 0){
            //         printf("X: %d\n", x);
            //     }
            // }
        }
    }
    return 0;
}