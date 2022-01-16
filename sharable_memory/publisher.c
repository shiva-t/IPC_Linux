/*
 Author: Shiva Tripathi
 Description: Code to demonstrate Shared Memory method of IPC
              Publisher will write into the shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>

#define SHM_NAME "/shm_name"
#define BUFFER_SIZE 128

int main() {
    int fd = 0, rc = 0, size = 0;
    char buffer[BUFFER_SIZE];
    void *mem_addr = NULL;

    /*request for shared memory objects*/
    fd = shm_open(SHM_NAME, O_CREAT|O_TRUNC|O_RDWR, 0660);
    if(fd == -1) {
        printf("shm_open failed\n");
        exit(1);
    }
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter the string to publish..\n");
    scanf("%s", buffer);
    size = strlen(buffer);

    /*set the size of shared memory object*/
    rc = ftruncate(fd, size);
    if (rc == -1) {
        printf("ftruncate failed\n");
        exit(-1);
    }

    /*mapping shared memory into virtual address space*/
    //starting address is NULL - for OS to decide, mem is readable and writable and 
    // sharable with other applications
    mem_addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem_addr == MAP_FAILED) {
        printf("mmap failed\n");
        exit(-1);
    }

    memset(mem_addr, 0 , size);
    strncpy(mem_addr, buffer, size);
    munmap(mem_addr, size); 

    close(fd);
    return 0;
}
