/*
  AUTHOR: Shiva Tripathi
  DESCRIPTION: Demo code to show working of signals and 
               how application can raise a linux signal 
               on itself using raise()
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void signal_int_handler(int sig) {
    printf("received Ctrl+C interrupt, terminating\n ");
    exit(0);
}

int main() {
    char ch;
    int rc = 0;

    /*register handler for SIGINT*/
    rc = signal(SIGINT, signal_int_handler);
    if (rc == SIG_ERR) {
       printf("singal handler registration failed\n");
       exit(0);
    }

    printf("rasie interrrup?.. (y/n)\n");
    scanf("%c",&ch);
    if(ch == 'y') {
        rc = raise(SIGINT);
        if (rc != 0) {
            printf("raise signal failed\n");
            exit(0);
         }
    }

    return 0;
}
