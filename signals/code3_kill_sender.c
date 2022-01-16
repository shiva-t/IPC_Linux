/*
  AUTHOR: Shiva Tripathi
  DESCRIPTION: Demo code to show working of signals and 
               how an application can raise a signal on another
               using SIGUSR1. Sender application raises this signal
               for receiver application
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    int pid;
    printf("enter the PID of receiver application to kill: ps -ef | grep ./code3_kill_rx.exe\n");
    scanf("%d", &pid);
    kill(pid , SIGUSR1);

    return 0;
}
