/*
 Author: Shiva Tripathi
 Description: Code to demonstrate Shared Memory method of IPC
              Subscriber will read fom the shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define SHM_NAME "/shm_name"
#define BUFFER_SIZE 128

int main() {
    int fd = 0 , rc = 0;
    char buffer[BUFFER_SIZE];
    void *mem_addr = NULL;

    /*open shared memory*/
    fd = shm_open(SHM_NAME, O_CREAT|O_RDONLY, 0660);
    if(fd == -1) {
        printf("shm_open failed\n");
        exit(-1);
    }

    mem_addr = mmap(NULL, BUFFER_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (mem_addr == MAP_FAILED) {
        printf("mmap failed\n");
        exit(-1);
    }

    memset(buffer, 0, BUFFER_SIZE);    
    memcpy(buffer, mem_addr, BUFFER_SIZE);
    printf("Read from mempry: %s\n", buffer);
    rc = munmap(mem_addr, BUFFER_SIZE);
    close(fd);
    shm_unlink(SHM_NAME);
    return 0;
}
