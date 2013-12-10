/** @file libmapreduce.c */
/* 
 * CS 241
 * The University of Illinois
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>

#include "libmapreduce.h"
#include "libds/libds.h"

static const int BUFFER_SIZE = 2048;  /**< Size of the buffer used by read_from_fd(). */
pthread_t  worker_thread;

/**
 * Adds the key-value pair to the mapreduce data structure.  This may
 * require a reduce() operation.
 *
 * @param key
 *    The key of the key-value pair.  The key has been malloc()'d by
 *    read_from_fd() and must be free()'d by you at some point.
 * @param value
 *    The value of the key-value pair.  The value has been malloc()'d
 *    by read_from_fd() and must be free()'d by you at some point.
 * @param mr
 *    The pass-through mapreduce data structure (from read_from_fd()).
 */
static void process_key_value(const char *key, const char *value, mapreduce_t *mr)
{
	//printf("DO YOU EVEN COME HERE?\n");
	datastore_t * curr = mr -> dictionary; 
	char * old; 
	char * add; 
	unsigned long * rev = malloc(sizeof(unsigned long)); 

	//printf("This is the pair (%s,%s)\n", key, value);

	if (datastore_put(curr, key, value)){
		//printf("***************************************************\n");
		free( (char *)key);
		free((char *) value);
		free (rev);
		return; 
	}
	else {
		old = (char*)datastore_get(curr, key, (unsigned long *)rev);
		//printf("this is old %s\n", old);
		add = (char*) mr->myreduce_function(old, value);
		//printf("this is the one we will add %s\n", add);
		free(old); 

		//printf("This is what is being sent in..... curr -> %s, key -> %s, add -> %s, rev -> %lu\n", curr, key, add, *rev);
		//printf("this is the key %s\n", key);

		datastore_update(curr, key, add, *rev);
		free(add);
	}
	free((char *) value);
	free((char *) key);
	free((char *) rev);
}


/**
 * Helper function.  Reads up to BUFFER_SIZE from a file descriptor into a
 * buffer and calls process_key_value() when for each and every key-value
 * pair that is read from the file descriptor.
 *
 * Each key-value must be in a "Key: Value" format, identical to MP1, and
 * each pair must be terminated by a newline ('\n').
 *sorter
 * Each unique file descriptor must have a unisorterque buffer and the buffer
 * must be of size (BUFFER_SIZE + 1).  Therefore, if you have two
 * unique file descriptors, you must have two buffers that each have
 * been malloc()'d to size (BUFFER_SIZE + 1).
 *
 * Note that read_from_fd() makes a read() call and will block if the
 * fd does not have data ready to be read.  This function is complete
 * and does not need to be modified as part of this MP.
 *
 * @param fd
 *    File descriptor to read from.sorter
 * @param buffer
 *    A unique buffer associated with the fd.  This buffer may have
 *    a partial key-value pair between calls to read_from_fd() and
 *    must not be modified outside the context of read_from_fd().
 * @param mr
 *    Pass-through mapreduce_t structure (to process_key_value()).
 *
 * @retval 1
 *    Data was available and was read successfully.
 * @retval 0
 *    The file descriptor fd has been closed, no more data to read.
 * @retval -1
 *    The call to read() produced an error.
 */
static int read_from_fd(int fd, char *buffer, mapreduce_t *mr)
{
	/* Find the end of the string. */
	int offset = strlen(buffer);

	/* Read bytes from the underlying stream. */
	int bytes_read = read(fd, buffer + offset, BUFFER_SIZE - offset);
	if (bytes_read == 0)
		return 0;
	else if(bytes_read < 0)
	{
		fprintf(stderr, "error in read.\n");
		return -1;
	}

	buffer[offset + bytes_read] = '\0';

	/* Loop through each "key: value\n" line from the fd. */
	char *line;


	while ((line = strstr(buffer, "\n")) != NULL)
	{
		*line = '\0';

		/* Find the key/value split. */
		char *split = strstr(buffer, ": ");
		if (split == NULL)
			continue;

		/* Allocate and assign memory */
		char *key = malloc((split - buffer + 1) * sizeof(char));
		char *value = malloc((strlen(split) - 2 + 1) * sizeof(char));

		strncpy(key, buffer, split - buffer);
		key[split - buffer] = '\0';

		strcpy(value, split + 2);

		/* Process the key/value. */
		process_key_value(key, value, mr);

		/* Shift the contents of the buffer to remove the space used by the processed line. */
		memmove(buffer, line + 1, BUFFER_SIZE - ((line + 1) - buffer));
		buffer[BUFFER_SIZE - ((line + 1) - buffer)] = '\0';
	}

	return 1;
}

/**
 * Initialize the mapreduce data structure, given a map and a reduce
 * function pointer.
 */
void mapreduce_init(mapreduce_t *mr, 
                    void (*mymap)(int, const char *), 
                    const char *(*myreduce)(const char *, const char *))
{
	mr -> mymap_function = mymap; 
	mr -> myreduce_function = myreduce; 
	datastore_t * dictionary = malloc(sizeof(datastore_t)); 
	datastore_init(dictionary);

	mr -> dictionary = dictionary; 

	mr -> done = 0; 
}

int size_of_array(const char ** arr){
	int i = 0; 
	while (arr[i] != NULL){i++;}
	return i; 
}

void * worker_function (void * in){
	mapreduce_t * mr = (mapreduce_t *) in; 
	fd_set master_fdset, active_fdset; 

    FD_ZERO(&master_fdset);
    FD_ZERO(&active_fdset);

    int i, j; 
    int alive_num = mr -> size; 

    char ** buffer_arr = malloc(sizeof(char *) * mr->size);
    mr -> buffers = buffer_arr;

    for (i = 0; i < (mr-> size); i++){
    	FD_SET(mr->pipes[i][0], &master_fdset);
    	buffer_arr[i] = malloc(BUFFER_SIZE + 1);
    	buffer_arr[i][0] = '\0';
    }

    while(alive_num> 0){
        active_fdset = master_fdset; 
        select(FD_SETSIZE, &active_fdset, NULL, NULL, NULL);
        for (j = 0; j < mr -> size; j++){
        	if (FD_ISSET(mr->pipes[j][0], &active_fdset)){
        		if (!read_from_fd(mr->pipes[j][0], buffer_arr[j], mr)){
        			close(mr->pipes[j][0]);
        			FD_CLR(mr->pipes[j][0], &master_fdset);
                	alive_num --;  
        		}
        	}
        }
    }

    
    //for (i = 0; i < mr -> size; i++){
    //	free(buffer_arr[i]);
    //	free(mr -> pipes[i]);
    //}

    //free(buffer_arr);
    //free(mr -> pipes);
	
    mr -> done = 1; 

    return 0; 
}

/**
 * Starts the map() processes for each value in the values array.
 * (See the MP description for full details.)
 */

void mapreduce_map_all(mapreduce_t *mr, const char **values)
{
	if (values == NULL){return;}
// ******************************** VARIABLES DECLARATION *********************************
	int size = size_of_array(values); 

	int ** pipe_arr = malloc(sizeof(int *) * size);

	int i = 0, curr = 0; 
// ******************************** INIT PIPES ********************************************
	for (i = 0; i < size; i++){
		pipe_arr[i] = malloc(sizeof(int) * 2); 
		pipe_arr[i][0] = -1;
		pipe_arr[i][1] = -1; 
		pipe(pipe_arr[i]); 
	}

	for(curr = 0; curr < size; curr++){

		if (fork() == 0){
			close(pipe_arr[curr][0]);
			//printf("This is what I am sending %s\n", values[curr]);
			mr->mymap_function(pipe_arr[curr][1], values[curr]);
  			exit(0);
		}
		close(pipe_arr[curr][1]);

	}
	mr -> pipes = pipe_arr;
	mr -> size = size; 

	pthread_create(&worker_thread, NULL, worker_function, (void *) mr);

}


/**
 * Blocks until all the reduce() operations have been completed.
 * (See the MP description for full details.)
 */
void mapreduce_reduce_all(mapreduce_t *mr)
{
	mr -> done = 1; 
	pthread_join(worker_thread, NULL);
}


/**
 * Gets the current value for a key.
 * (See the MP description for full details.)
 */
const char *mapreduce_get_value(mapreduce_t *mr, const char *result_key)
{

	const char * back = datastore_get(mr -> dictionary, result_key, NULL); 

	return back;
}

/**
 * Destroys the mapreduce data structure.
 */
void mapreduce_destroy(mapreduce_t *mr)
{
	// delete
	
	int i; 

	for (i = 0; i < mr->size; i++){
		//printf("here bitch\n");
		free(mr->buffers[i]);
		free(mr->pipes[i]);
	} 

	free(mr ->buffers);
	free(mr ->pipes);
	
	datastore_destroy(mr -> dictionary);
	free(mr -> dictionary);

}
