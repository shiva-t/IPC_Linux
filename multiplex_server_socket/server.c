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
#define MAX_CLIENT_SUPP 16  //maximum clients that can connect to server

/*array of File descriptors corresponding to the connected clients
  and also the master file descriptor of server*/
int monitored_fd_set[MAX_CLIENT_SUPP];

int client_result[MAX_CLIENT_SUPP] = {0}; //result arrays for every client

void initialise_fd_arr(void) {
    int i = 0;
    for(i = 0; i< MAX_CLIENT_SUPP; i++) {
        monitored_fd_set[i] = -1;
    }
}

void add_fd_monitored_set(int in_fd) {
    int i = 0;
    for (i= 0; i < MAX_CLIENT_SUPP; i++) {
        if(monitored_fd_set[i] == -1) {
            monitored_fd_set[i] = in_fd;
            break;
         }
    }
}

void remove_from_monitored_fd(int skt_fd) {
    int i = 0;
    for (i =0 ; i< MAX_CLIENT_SUPP; i++) {
        if(monitored_fd_set[i] == skt_fd) {
            monitored_fd_set[i] == -1;
            break;
        }
    }
}

void refresh_fd_set(fd_set *fd_set_ptr) {
    FD_ZERO(fd_set_ptr);
    int i =0;
    for (i = 0; i< MAX_CLIENT_SUPP; i++) {
        if(monitored_fd_set[i] != -1) {
           FD_SET(monitored_fd_set[i], fd_set_ptr);
        } 
    }
}

int get_max_fd(void) {
    int i = 0, max = -1;
    for (i = 0; i < MAX_CLIENT_SUPP; i++) {
       if(monitored_fd_set[i] != -1 && max<monitored_fd_set[i]) {
           max = monitored_fd_set[i];
       }
    }
    return max;
}

int main(int argc, char *argv[])  {
    struct sockaddr_un s_name;
    int rc = 0;
    int connection_socket; //master socket file descriptor
    int data_socket; //handler for each client
    char buffer[BUFFER_SIZE];
    int data = 0;
    int comm_socket_fd, i;
    fd_set readfds; //inbuilt structure

    /*Initialise the file descriptor array*/
    initialise_fd_arr();
    //add_fd_monitored_set(0); //TODO:

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

    /* add master fd to fd_array*/
    add_fd_monitored_set(connection_socket); 
    
    /*Infinite loop for the server to be ready to take requests*/
    while(1) {
        // -- refresh fd_set ; readfds set must have all monitored fd 
        refresh_fd_set(&readfds);

        printf("Waiting on select system call\n");

        select(get_max_fd()+1, &readfds, NULL, NULL, NULL);

        /*Code part hit if some client sent connection req or data*/
        if(FD_ISSET(connection_socket, &readfds)) {
            //new connection iniation request
            printf("New connection received \n");
            data_socket = accept(connection_socket, NULL,NULL);
            if(data_socket == -1) {
                printf("accpet failed\n");
                exit(EXIT_FAILURE);
            }
            add_fd_monitored_set(data_socket);
            printf("connection accpeted\n");
        } else if (FD_ISSET(0, &readfds)) {
            //data sent by client
            memset(buffer, 0, BUFFER_SIZE);
            rc = read(0, buffer, BUFFER_SIZE);
            printf("data read :%s\n", buffer);
        } else {
           //data sent by one of the clients
           //find the client which has sent the data
           comm_socket_fd = -1;
           printf("data to be received\n");
           for(i =0; i< MAX_CLIENT_SUPP; i++) {
               if(FD_ISSET(monitored_fd_set[i], &readfds)) {
                   comm_socket_fd = monitored_fd_set[i];
                   memset(buffer, 0, BUFFER_SIZE);
                   printf("waiting to read \n");
                   rc = read(comm_socket_fd, buffer, BUFFER_SIZE);
                   if(rc == -1) {
                       printf("read failed\n");
                       exit(EXIT_FAILURE);
                   }
                   memcpy(&data, buffer, sizeof(int));
                   printf("data read: %d\n", data);
                   if(data == 0) {
                       //sum to be returned
                       memset(buffer, 0, BUFFER_SIZE);
                       sprintf(buffer, "result = %d", client_result[i]);
                       rc = write(comm_socket_fd, buffer, BUFFER_SIZE);
                       if(rc == -1) {
                           printf("write failed\n");
                           exit(EXIT_FAILURE);
                       }
                       close(comm_socket_fd);
                       client_result[i] = 0;
                       remove_from_monitored_fd(comm_socket_fd);
                       continue;
                   }
                   client_result[i] = client_result[i]+data;
               }
           }

        }
    }

    /*close connection socket - master file descriptor*/
    close(connection_socket);
    unlink(SOCKET_NAME);

    return 0;
}
