/** @file libds.c */
/* 
 * CS 241
 * The University of Illinois
 */


#include "libds.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "jansson.h"
#include "libhttp.h"
#include <string.h>
 #include <pthread.h>

/*
Questions:
*server.sin_addr.s_addr = inet_addr("127.0.0.1"); <- is that how we should connect?

datastore_put --- Content-Length: ###, number is it length of everything

*/

/**
 * Sets the server the data store should connect to for
 * NoSQL operations.  You may assume this call will be 
 * made before any calls to datastore_init().
 *
 * @param server
 *   The hostname of the server.
 * @param port
 *   The post on the server to connect to.
 */

int global_fd; 

void datastore_set_server(const char *server1, int port)
{





	global_fd = socket(AF_INET, SOCK_STREAM, 0);
/*
	struct sockaddr_in server; 
	server.sin_family = AF_INET; 
	server.sin_addr.s_addr = inet_addr(server1);
	server.sin_port = htons(port);

	int res = connect(global_fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

	if (res < 0){
		perror("connect");
		exit(1);
	}
	
	*/
	
	
	struct addrinfo * info;
	struct addrinfo hints;
	struct sockaddr_in *  server; 
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 */
	hints.ai_socktype = SOCK_STREAM; /* TCP stream */
	hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
	
	
	char * temp; 
	asprintf(&temp, "%d", port); 
	
	if(getaddrinfo(server1, temp, &hints, &info)) {
		perror("Unable to locate the server");
		//retval = 1;
		//goto out;
	}
	
	server = info->ai_addr;

	if(connect(global_fd, server, sizeof(struct sockaddr))) {
		perror("Unable to connect");
		//retval = 1;
		//goto out;
	}
}	


/**
 * Initializes the data store.
 *
 * @param ds
 *    An uninitialized data store.
 */
void datastore_init(datastore_t *ds)
{
	//ds  = malloc(sizeof(datastore_t));
	pthread_mutex_init(&ds->mutex, NULL);
}


/**
 * Adds the key-value pair (key, value) to the data store, if and only if the
 * key does not already exist in the data store.
 *
 * @param ds
 *   An initialized data store.
 * @param key
 *   The key to be added to the data store.
 * @param value
 *   The value to associated with the new key.
 *
 * @retval 0
 *   The key already exists in the data store.
 * @retval non-zero
 *   The revision number assigned to the specific value for the given key.
 */
unsigned long datastore_put(datastore_t *ds, const char *key, const char *value)
{
	//printf("PUT GOT CALLED\n");
	/*
	PUT /some_key_name HTTP/1.1
	Content-Type: application/json
	Content-Length: ###

	{"Value":"key_value"}

	*/
	char * message_to_send;
	char * body; 
	http_t temp;

	// IS THIS IN THE RIGHT ORDER??? 
	int length = asprintf(&body, "{\"%s\":\"%s\"}", "Value", value);

/*
PUT /some_key_name HTTP/1.1
Content-Type: application/json
Content-Length: ###

*/
	int len = asprintf (&message_to_send,"PUT /%s HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s", key, length, body);

	// MUTEX
	pthread_mutex_lock(&ds->mutex);
	send(global_fd, message_to_send, len, 0);

	//printf("%s\n", message_to_send);
	// UNLOCK MUTEX


	int size = http_read(&temp, global_fd);
	pthread_mutex_unlock(&ds->mutex);
	const char * get_line = http_get_status(&temp);
	

	//printf("This is the get_line %s\n", get_line);

	if (strstr(get_line, "201") != NULL){
		char *raw_json = http_get_body(&temp, NULL);
		json_t *json = json_loads(raw_json, JSON_DECODE_ANY, NULL);
		char *value = json_string_value(json_object_get(json, "rev"));
		long revision = atol(value);
		return revision; 
	}
	else{
		//printf("DID NOT ENTER\n");
		//printf("returned 0\n");
		return 0; 
	}
}


/**
 * Retrieves the current value, and its revision number, for a specific key.
 *
 * @param ds
 *   An initialized data store.
 * @param key
 *   The specific key to retrieve the value.
 * @param revision
 *   If non-NULL, the revision number of the returned value will be written
 *   to the address pointed to by <code>revision</code>.
 *
 * @return
 *   If the data store contains the key, a new string containing the value
 *   will be returned.  It is the responsibility of the user of the data
 *   store to free the value returned.  If the data store does not contain
 *   the key, NULL will be returned and <code>revision</code> will be unmodified.
 */
const char *datastore_get(datastore_t *ds, const char *key, unsigned long *revision)
{
	//GET /some_key_name HTTP/1.1
	char * message_to_send;
	asprintf (&message_to_send,"GET /%s HTTP/1.1\r\n\r\n", key);
	pthread_mutex_lock(&ds->mutex);
	send(global_fd, message_to_send, strlen(message_to_send), 0);

	http_t temp;
	int size = http_read(&temp, global_fd);
	pthread_mutex_unlock(&ds->mutex);
	const char * get_line = http_get_status(&temp);


	if ((strstr(get_line, "200") != NULL)){
		char *raw_json = http_get_body(&temp, NULL);
		json_t *json = json_loads(raw_json, JSON_DECODE_ANY, NULL);
		char *value = json_string_value(json_object_get(json, "Value"));
		//printf("%s\n", raw_json);
		char * temp = json_string_value(json_object_get(json, "_rev")); 
		if (revision != NULL) {
			*revision = atol(temp);
		}
		//if revision was pointing to null then do we malloc memory for it, pointing to the right revision number?
		return value; 

	}
	else{

		return NULL; 

	}	//printf("THIS IS FROM DELETE %s\n", get_line);
	//printf("before#f^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^innakill\n");

}


/**
 * Updates the specific key in the data store if and only if the
 * key exists in the data store and the key's revision in the data
 * store matches the knwon_revision specified.
 * 
 * @param ds
 *   An initialized data store.
 * @param key
 *   The specific key to update in the data store.
 * @param value
 *   The updated value to the key in the data store.
 * @param known_revision
 *   The revision number for the key specified that is expected to be found
 *   in the data store.  If the revision number specified in calling the
 *   function does not match the revision number in the data store, this
 *   function will not update the data store.
 *
 * @retval 0
 *    The revision number specified did not match the revision number in the
 *    data store or the key was not found in the data store.  If the key is
 *    in the data store, this indicates that the data has been modified since
 *    you last performed a datastore_get() operation.  You should get an
 *    updated value from the data store.
 * @retval non-zero
 *    The new revision number for the key, now associated with the new value.
 */
unsigned long datastore_update(datastore_t *ds, const char *key, const char *value, unsigned long known_revision)
{
	
/*
PUT /some_key_name HTTP/1.1
Content-Type: application/json
Content-Length: ###

{"Value":"key_value", "_rev":"42"}

*/
	char * message_to_send;
	char * body;  
	http_t temp;

	int length = asprintf(&body, "{\"Value\":\"%s\", \"_rev\":\"%lu\"}", value, known_revision);

	asprintf (&message_to_send,"PUT /%s HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n", key, length);

	// MUTEX
	pthread_mutex_lock(&ds->mutex);
	send(global_fd, message_to_send, strlen(message_to_send), 0);

	send(global_fd, body, strlen(body), 0);

	int size = http_read(&temp, global_fd);
	pthread_mutex_unlock(&ds->mutex);
	const char * get_line = http_get_status(&temp);

	//printf("THIS IS ANOTHER get_line %s\n", get_line);
	// do what with revision number?
	if (strstr(get_line, "201") != NULL){

		char *raw_json = http_get_body(&temp, NULL);
		json_t *json = json_loads(raw_json, JSON_DECODE_ANY, NULL);
		char *value = json_string_value(json_object_get(json, "rev"));

		long revision = atol(value);

		return revision; 
	}
	else{

		//printf("returned 0\n");
		return 0; 
	}

	return 0;
}


/**
 * Deletes a specific key from the data store.
 *
 * @param ds
 *   An initialized data store.
 * @param key
 *   The specific key to update in the data store.
 * @param known_revision
 *   The revision number for the key specified that is expected to be found
 *   in the data store.  If the revision number specified in calling the
 *   function does not match the revision number in the data store, this
 *   function will not update the data store.
 *
 * @retval 0
 *    The revision number specified did not match the revision number in the
 *    data store or the key was not found in the data store.  If the key is
 *    in the data store, this indicates that the data has been modified since
 *    you last performed a datastore_get() operation.  You should get an
 *    updated value from the data store.
 * @retval non-zero
 *    The key was deleted from the data store.
 */
unsigned long datastore_delete(datastore_t *ds, const char *key, unsigned long known_revision)
{

	/*DELETE /some_key_name HTTP/1.1\r\nIf-Match: "42"
	*/

	char * message_to_send; 
	asprintf (&message_to_send,"DELETE /%s HTTP/1.1\r\nIf-Match: \"%lu\"\r\n\r\n", key, known_revision);
	pthread_mutex_lock(&ds->mutex);
	send(global_fd, message_to_send, strlen(message_to_send), 0);

	http_t temp;
	int size = http_read(&temp, global_fd);
	pthread_mutex_unlock(&ds->mutex);
	const char * get_line = http_get_status(&temp);

	//printf("THIS IS FROM DELETE %s\n", get_line);
	//printf("before#f^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^innakill\n");

	if (strstr(get_line, "200") != NULL){
		char *raw_json = http_get_body(&temp, NULL);
		json_t *json = json_loads(raw_json, JSON_DECODE_ANY, NULL);
		char *value = json_string_value(json_object_get(json, "rev"));
		//printf("b3\n");
		long revision = atol(value);
		//printf("a3\n");
		return revision; 
	}
	else{
		return 0; 
	}

}

/**
 * Destroys the data store, freeing any memory that is in use by the
 * data store.
 *
 * @param ds
 *   An initialized data store.
 */
void datastore_destroy(datastore_t *ds)
{
	pthread_mutex_destroy(&ds->mutex);
}
