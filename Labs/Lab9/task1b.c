
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 27
void die(char* str){
    perror(str); exit(1);
}

int main(void){
    int shmid; key_t key = 1122;
    int *shm, *s;

    if((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0) die("shmget");

    if((shm = shmat(shmid, NULL, 0)) == (int*)-1) die("shmat");

    s = shm; *s = 1;
    while(*shm != '*') sleep(1);
    exit(0);
}

