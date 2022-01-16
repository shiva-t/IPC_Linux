/*
 Author : Shiva Tripathi
 Description : Demo Code to application to enquque messages on a message queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Q_NAME "/mq_path"
#define BUFFER_SIZE 128

int main(int argc, int **argv) {
    int msgq_fd = 0, rc = 0;
    char buffer[BUFFER_SIZE];

   /*open the message queue*/
    msgq_fd = mq_open(Q_NAME, O_WRONLY | O_CREAT, 0 ,0);
    if (msgq_fd == -1) {
        printf("mq_open failed\n");
        exit(1);
    }

    /*enqueue the message*/
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter the message to send...\n");
    scanf("%s", buffer);
    rc = mq_send(msgq_fd, buffer, strlen (buffer) + 1, 0);
    if (rc == -1) {
        printf("mq_send failed \n");
        exit(1);
    }

    mq_close(msgq_fd);
    return 0;
}
