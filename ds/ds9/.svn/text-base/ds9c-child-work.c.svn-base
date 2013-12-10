#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/*
 * you do not need to modify this file, though you may experiment with
 * it to be more confident with using pipes.
 */


void write_to_fd(int fd, const char *s)
{
	write(fd, s, strlen(s));
}

/* call this function with the write-end fd of the pipe */
void child1_does_work_and_exit(int fd)
{
	sleep(2);  /* Simulate work being done... */
	write_to_fd(fd, "World");

	sleep(6);  /* Simulate work being done... */
	write_to_fd(fd, "CS241");

    /* child close() the fd and then exit() after doing work */
    printf("  child1 closing write-end fd [%d] now, at second %ld\n",
           fd, time(NULL));
	close(fd);

    printf("  child1 sleeping 10 seconds...\n");
    sleep(10);

    printf("    child1 exiting now, at second %ld\n", time(NULL));
    exit(0);
}

/* call this function with the write-end fd of the pipe */
void child2_does_work_and_exit(int fd)
{
	/* No work needs to be done, send right away. */
	write_to_fd(fd, "Hello");

	sleep(3);  /* Simulate work being done... */
	write_to_fd(fd, "from ");

    /* child close() the fd and then exit() after doing work */
    printf("  child2 closing write-end fd [%d] now, at second %ld\n",
           fd, time(NULL));
	close(fd);

    printf("  child2 sleeping 10 seconds...\n");
    sleep(10);

    printf("    child2 exiting now, at second %ld\n", time(NULL));
    exit(0);
}
