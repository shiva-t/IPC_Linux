/*
 Author : Shiva Tripathi
 Description : Demo Code to application to dequque messages from a message queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>

#define Q_NAME "/mq_path"
#define BUFFER_SIZE 128

int main() {
    int msgq_fd = 0, rc = 0;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    
    /*To set msgQ attributes*/
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 118;
    attr.mq_curmsgs = 0;

    /*open message queue*/
    msgq_fd = mq_open(Q_NAME, O_RDONLY | O_CREAT, 0660 ,&attr);
    if (msgq_fd == -1) {
        printf("mq_open failed\n");
        exit(1);
    }

    while(1) {
        FD_ZERO(&readfds);
        FD_SET( msgq_fd, &readfds);
        printf("waiting on select \n");
        select(msgq_fd +1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(msgq_fd, &readfds)) {
            printf("message received\n");
            /*receive message*/
            memset(buffer, 0, BUFFER_SIZE);
            rc = mq_receive(msgq_fd, buffer, BUFFER_SIZE, NULL);
            if(rc == -1) {
                printf("message receive failed\n");
                exit(1);
            }
            printf("message received : %s\n", buffer);
        }
    }
    mq_unlink(Q_NAME);
    return 0;
}
