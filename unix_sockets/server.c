/*
 Description : Implementation of Server for Unix socket IPC
               The server when receives service request from client, 
               performs addition of data and resturns result on receiving 0
 Execute     : gcc -g server.c -o server
               ./server
 Author      : Shiva Tripathi
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define SOCKET_NAME "/tmp/Add_demo_Socket"
#define BUFFER_SIZE 128

int main(int argc, char *argv[])  {
    struct sockaddr_un s_name;
    int rc = 0;
    int connection_socket; //master socket file descriptor
    int data_socket; //handler for each client
    char buffer[BUFFER_SIZE];
    int result = 0, data = 0;

    /*Step 1: Remove any exisiting socket with same name*/
    unlink(SOCKET_NAME);

    /*Step 2: Server should create the master socket/file descriptor*
              All connection initiation req of client are handled by this
              Communication can be Stream or Datagram*/
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connection_socket == -1) {
        printf("socket connection failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Master socket created\n");

    //Setting socket creadentials
    memset(&s_name, 0, sizeof(struct sockaddr_un));
    s_name.sun_family= AF_UNIX;
    strncpy(s_name.sun_path, SOCKET_NAME, sizeof(s_name.sun_path) -1);

    /*Step 3: bind socket to the name : SOCKET_NAME
              bind tells the OS, whenever some clients reaches out to SOCKET_NAME path
              this particular server needs to be reached out to*/
    rc = bind(connection_socket, (const struct sockaddr *)&s_name, sizeof(struct sockaddr_un));
    if (rc == -1) {
        printf("bind failed\n");
        exit(EXIT_FAILURE);
    }

    printf("bind success\n");

    /*Step 4: listen is invoked to make server ready to accept connections
              second argument tells the size of waiting queue for this server*/
    rc = listen(connection_socket, 15);
    if (rc == -1) {
        printf("listen failed\n");
        exit(EXIT_FAILURE);
    }
    
    /*Infinite loop for the server to be ready to take requests*/
    while(1) {
        printf("Waiting on accept system call\n");

        /*Step 5: accept system call, it's blocking and will wait for client request
                  it return the data socket or the handler for that client*/
        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1) {
            printf("accept error\n");
            exit(EXIT_FAILURE);
        }
        printf("Connection with client done\n");

        result = 0;
        //communication with client begins
        while(1) {
            memset(buffer, 0, BUFFER_SIZE);
            printf("waiting for read ffrom client\n");
            rc = read(data_socket, buffer, BUFFER_SIZE);
            if (rc == -1) {
                printf("read failed\n");
                exit(EXIT_FAILURE);
            }

            //proces data - functionality of server
            memcpy(&data, buffer, sizeof(int));
            if (data == 0) break;
            result = result + data;
        }

        //send result
        memset(buffer, 0 , BUFFER_SIZE);
        sprintf(buffer, "Result = %d", result);

        printf("sending result to client\n");
        rc = write(data_socket, buffer, BUFFER_SIZE);
        if (rc == -1) {
            printf("write failed\n");
            exit(EXIT_FAILURE);
        }

        /*close data socket - handler for client*/
        close(data_socket);
    }

    /*close connection socket - master file descriptor*/
    close(connection_socket);
    unlink(SOCKET_NAME);

    return 0;
}
