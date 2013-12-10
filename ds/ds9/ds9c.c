#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define BUFSIZE (80)

extern void child1_does_work_and_exit(int fd);
extern void child2_does_work_and_exit(int fd);


int main()
{
    char buf[BUFSIZE];
    strcpy(buf, "123456789");
    int pipe1[2] = {-1, -1};
    int pipe2[2] = {-1, -1};
    int alive_num = 2;
    pipe(pipe1);
    pipe(pipe2);

    // in the child
    if (fork() == 0){
        close(pipe1[0]);
        child1_does_work_and_exit(pipe1[1]);
        return; 
    }
    if (fork() == 0){
        close(pipe2[0]);
        child2_does_work_and_exit(pipe1[1]);
        return; 
    }

    close(pipe1[1]);
    close(pipe2[1]);

    fd_set master_fdset, active_fdset; 

    FD_ZERO(&master_fdset);
    FD_ZERO(&active_fdset);

    FD_SET(pipe1[0], &master_fdset);
    FD_SET(pipe2[0], &master_fdset);

    while(alive_num> 0){
        active_fdset = master_fdset; 
        printf("waiting for data\n");
        select(FD_SETSIZE, &active_fdset, NULL, NULL, NULL);
        if (FD_ISSET(pipe1[0], &active_fdset)){
            int bytes = read(pipe1[0], buf, 5);
            if (bytes == 0){
                printf("Chlid 1 finished his work\n");
                close(pipe1[0]);
                FD_CLR(pipe1[0], &master_fdset);
                alive_num --;  
            }
            else{
                buf[bytes] = 0; 
                printf("Got bytes from child1: [%s]\n", buf); 
            }
        }


        if (FD_ISSET(pipe2[0], &active_fdset)){
            int bytes = read(pipe2[0], buf, 5);
            if (bytes == 0){
                printf("Chlid 2 finished his work\n");
                close(pipe2[0]);
                FD_CLR(pipe2[0], &master_fdset);
                alive_num --;  
            }
            else{
                buf[bytes] = 0; 
                printf("Got bytes from child2: [%s]\n", buf); 
            }
        }
    }
    /* fork 2 child processes and create a corresponding pipe to
     * communicate with each child process. give each child the
     * write-end of its corresponding pipe, and the child will write
     * to that pipe to send bytes back to the parent.
     *
     * - child1 should call child1_does_work() with the write-end of
     * pipe1.
     *
     * - child2 should call child2_does_work() with the write-end of
     * pipe2.
     *
     * after a child is done with its work, it will close the
     * write-end of its pipe and exit.
     */







    /* this is parent continuing on: read from the pipes...
     * use select() to multiplex.
     */






    printf("\n\n === parent exiting now, at second %ld ===\n\n", time(NULL));

    return 0;
}
