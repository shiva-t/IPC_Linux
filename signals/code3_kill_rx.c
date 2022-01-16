/*
  AUTHOR: Shiva Tripathi
  DESCRIPTION: Demo code to show working of signals and 
               how an application can raise a signal on another
               using SIGUSR1. Receiver application receives this signal
               from sender, and handles it as per handler defined
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void signal_handler(int sig) {
    printf("received the signal\n");
    exit(0);
}

int main() {
    signal(SIGUSR1, signal_handler);

    while(1);
    return 0;
}
