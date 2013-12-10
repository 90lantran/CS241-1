#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BUFSIZE (80)

int main()
{
    char buf[BUFSIZE];
    strcpy(buf, "123456789");
    int fds[2] = {-1, -1};

    /* create the pipe here... */
    if (0 != pipe(fds)) {
        perror("cannot create pipe...");
        exit(1);
    }


    /* write "world" to the pipe */
    write(fds[1], "world", 6);


    /* read 1 char from the pipe here... */
    printf("\n----------------\n");
    printf("reading 1 byte from pipe...\n");

    read(fds[0], buf, 1);



    printf("the read byte is [%c]\n", buf[0]);
    printf("the buf is [%s]\n", buf);


    /* read 2 chars from the pipe here... */
    printf("\n----------------\n");
    printf("reading 2 bytes from pipe...\n");

    read(fds[0], buf+1, 2);



    printf("the buf is [%s]\n", buf);


    /* read 3 chars from the pipe here... */
    printf("\n----------------\n");
    printf("reading 3 bytes from pipe...\n");

    read(fds[0], buf+3, 3);



    printf("the buf is [%s]\n", buf);

    return 0;
}
