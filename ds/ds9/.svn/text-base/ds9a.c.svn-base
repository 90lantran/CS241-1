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
    char readc = '\0';
    int fds[2] = {-1, -1};

    /* create the pipe here... */

    if (!pipe(fds)) {
    	fprintf(stderr, "Unable to create pipes\n");
    }

    write(fds[1],buf,1);

    /* read 1 char from the pipe here... */
    printf("reading 1 byte from pipe...\n");

    read(fds[0], &readc, 1);





    printf("the read byte is [%c]\n", buf[0]);

    return 0;
}
