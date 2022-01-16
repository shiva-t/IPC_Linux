/*
  AUTHOR: Shiva Tripathi
  DESCRIPTION: Demo code to show working of signals and 
               how user defined handler can be used for 
               SIGINT and SIGABRT signals
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void signal_int_handler(int sig) {
   printf("received Ctrl+C signal, terminating\n");
   exit(0);
}

static void signal_abrt_handler(int sig) {
   printf("Abort received\n");
   exit(0);
}

int main() {
    char ch;
    int rc = 0;

    /*signal syscall is used to register handler for SIGNALS received*/
    rc = signal(SIGINT, signal_int_handler);
    if (rc == SIG_ERR) {
       printf("singal handler registration failed: SIGINT\n");
       exit(0);
    }
    rc = signal(SIGABRT, signal_abrt_handler);
    if (rc == SIG_ERR) {
       printf("singal handler registration failed: SIGABRT\n");
       exit(0);
    }

    printf("Is abort needed:..(y/n)\n");
    scanf("%c", &ch);
    if(ch == 'y')
        abort(); //raises SIGABRT
    return 0;
}
