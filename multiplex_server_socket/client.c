/*
 Description : Implementation of Client for Unix socket IPC
               The Client send set of numbers to the servers to 
               perform addition of data and resturn result on sending 0
 Execute     : gcc -g client.c -o client
               ./client
 Author      : Shiva Tripathi
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
 
#define BUFFER_SIZE 128
#define SOCKET_NAME "/tmp/Add_demo_Socket"

int main() {
    int data_socket;
    int rc = 0, in = 0;
    struct sockaddr_un s_name;
    char buffer[BUFFER_SIZE];

    /*Step1: create data socket, no master socket in client */
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(data_socket == -1) {
        printf("data connection failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Data socket created\n");

    /*set socket credentials*/
    memset(&s_name, 0, sizeof(struct sockaddr_un));
    s_name.sun_family = AF_UNIX;
    strncpy(s_name.sun_path, SOCKET_NAME, sizeof(s_name.sun_path)-1);

    /*Step2: Establish connection with server:
             Connection initiation request*/
    rc = connect(data_socket, (const struct sockaddr *)&s_name,  sizeof(struct sockaddr_un));
    if(rc == -1) {
        printf("connect failed\n");
        exit(EXIT_FAILURE);
    }
    printf("connection created\n");

    while(1) {
        printf("Enter the data to add:\n");
        scanf("%d", &in);
        rc = write(data_socket, &in, sizeof(int));
        if (rc == -1) {
            printf("write failed\n");
            exit(EXIT_FAILURE);
        }
        printf("data sent: %d\n", in);

        if(in == 0) {
            break;
        }
    }

    memset(buffer, 0, BUFFER_SIZE);
    rc = read(data_socket, buffer, BUFFER_SIZE);
    if (rc == -1) {
        printf("read failed\n");
        exit(EXIT_FAILURE);
    }

    buffer[BUFFER_SIZE-1] = 0;

    printf("data received: %s\n", buffer);
    close(data_socket);

    return 0;
}
