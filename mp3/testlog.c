/** @file testlog.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

/**
 * Game plan:
 * 1) Test log_init() 
 * 2) Test log_append() and log_at() and log_size()
 * 3) Test log_pop()
 * 4) Test log_append() after log_pop()
 * 5) Test log_search()
 * 6) Test log_destroy()
 * 7) Profit!!
*/

int main()
{
        log_t l;
        log_init(&l);
        
        printf("\nTesting log_append():\n");
        log_append(&l, "ab  1");
        log_append(&l, "a   2");
        log_append(&l, "abc 3");
        log_append(&l, "ab  4");
        log_append(&l, "a   5");
        printf("0: '%s'\n",log_at(&l, 0));
        printf("1: '%s'\n",log_at(&l, 1));
        printf("2: '%s'\n",log_at(&l, 2));
        printf("3: '%s'\n",log_at(&l, 3));
        printf("4: '%s'\n",log_at(&l, 4));
        printf("log size: %d\n", log_size(&l));
        printf("\n");

        // Are you copying the contents of the string into your log or are you
        // just storing a pointer to the string?
        printf("Testing method of appending entries:\n");
        char * temp = malloc(11), * temp2 = malloc(10);

        // "cba 6"
        temp[0] = 'c'; temp[1] = 'b'; temp[2] = 'a'; temp[3] = ' ';
        temp[4] = '6'; temp[5]= '\0';
        // Copy of "cba 6"
        temp2[0] = temp[0]; temp2[1] = temp[1]; temp2[2] = temp[2];
        temp2[3] = temp[3]; temp2[4] = temp[4]; temp2[5] = temp[5];

        log_append(&l, temp);

        // "abc 6"
        temp[0] = 'a'; temp[1] = 'b'; temp[2] = 'c';

        if (strncmp(temp, temp2, 6) == 0)
            printf("FAIL:\tNew log entries are copied instead of pointed to.\n\tExpected: '%s'\n\tActual: '%s'\n\n",temp2,log_pop(&l));
        else
            printf("SUCC:\tExpected: '%s'\n\tActual: '%s'\n\n",temp,log_pop(&l));

        
        printf("Testing log_pop():\n");
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("Popped: '%s'. ", log_pop(&l));
        printf("Current size: %d\n", log_size(&l));
        printf("\n");
        
        printf("Re-populating log entries:\n");
        log_append(&l, "ab  1");
        log_append(&l, "a   2");
        log_append(&l, "abc 3");
        log_append(&l, "ab  4");
        log_append(&l, "a   5");
        printf("0: '%s'\n",log_at(&l, 0));
        printf("1: '%s'\n",log_at(&l, 1));
        printf("2: '%s'\n",log_at(&l, 2));
        printf("3: '%s'\n",log_at(&l, 3));
        printf("4: '%s'\n",log_at(&l, 4));
        printf("log size: %d\n", log_size(&l));
        printf("\n");
        
        printf("Testing log_search():\n");
        printf("searching 'ab': '%s'\n",log_search(&l, "ab"));
        printf("searching 'a': '%s'\n",log_search(&l, "a"));
        printf("searching 'abc': '%s'\n",log_search(&l, "abc"));
        printf("searching 'd': '%s'\n",log_search(&l, "d"));
        printf("\nDone.\n\n");
        
        log_destroy(&l);
        free(temp);
        free(temp2);
        return 0;
}

