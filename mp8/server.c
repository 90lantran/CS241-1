/** @file server.c */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <queue.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include "queue.h"
#include "libhttp.h"
#include "libdictionary.h"

const char *HTTP_404_CONTENT = "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1>The requested resource could not be found but may be available again in the future.<div style=\"color: #eeeeee; font-size: 8pt;\">Actually, it probably won't ever be available unless this is showing up because of a bug in your program. :(</div></html>";
const char *HTTP_501_CONTENT = "<html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1>The server either does not recognise the request method, or it lacks the ability to fulfill the request.</body></html>";

const char *HTTP_200_STRING = "OK";
const char *HTTP_404_STRING = "Not Found";
const char *HTTP_501_STRING = "Not Implemented";

/*
QUESTIONS:
-> CTRL + C
--> pass in server socket, close()
--> how do we take care of client socket

->Is this alright
-->free(request_line);
-->request_line = "/index.html";

*/

/**
 * Processes the request line of the HTTP header.
 * 
 * @param request The request line of the HTTP header.  This should be
 *                the first line of an HTTP request header and must
 *                NOT include the HTTP line terminator ("\r\n").
 *
 * @return The filename of the requested document or NULL if the
 *         request is not supported by the server.  If a filename
 *         is returned, the string must be free'd by a call to free().
 */
int server_socket_global;
int * arr[1000];
char * reqs[1000];
int i = 0; 
int j = 0; 
struct addrinfo *res;

int malls = 0; 
int frees = 0; 

queue_t threads; 

void handler (int signum){
	close(server_socket_global);

	printf("you called signum: %d", signum);
	int s = 0; 
	for (s = 0; s <= i; s++){
		//printf("im here\n");
		if (arr[s] != NULL){
		close(*arr[s]);}

		free(arr[s]);
		//free(&arr[s]);
	}

	int size = queue_size(&threads);
	printf("this is size %d\n", size);
	for (s = 0; s < size; s++){
		pthread_t * temp = (pthread_t *) queue_dequeue(&threads);
		pthread_join(*temp,NULL);
		free(temp);
		printf("\n******************************************free******************************\n");
		frees++;
		//printf("\n******************************************FREE********************************\n");
	}

	printf("This is the number of malls: %d\n numbers of frees %d\n", malls, frees);
	queue_destroy(&threads);
	freeaddrinfo(res);
	exit(0);
}

char* process_http_header_request(const char *request)
{
	// Ensure our request type is correct...
	if (strncmp(request, "GET ", 4) != 0)
		return NULL;

	// Ensure the function was called properly...
	assert( strstr(request, "\r") == NULL );
	assert( strstr(request, "\n") == NULL );

	// Find the length, minus "GET "(4) and " HTTP/1.1"(9)...
	int len = strlen(request) - 4 - 9;

	// Copy the filename portion to our new string...
	char *filename = malloc(len + 1);
	//printf("\n******************************************MALLOCD - given********************************\n");
	strncpy(filename, request + 4, len);
	filename[len] = '\0';

	// Prevent a directory attack...
	//  (You don't want someone to go to http://server:1234/../server.c to view your source code.)
	if (strstr(filename, ".."))
	{
		free(filename);
		return NULL;
	}

	return filename;
}

void * worker_thread(void * fd){
	int * client_socks = (int *)fd;
	//arr[i] = client_socks;
	int client_sock = *client_socks;
	//i++;
	while(1){
		FILE *fp = NULL; 
		http_t temp; 
		//printf("THIS IS THE CLIENT SOCK %d\n", client_sock);
		int size = http_read(&temp, client_sock);
		int response_code;
		if (size == -1){http_free(&temp);break;}
		char * status = (char * )http_get_status(&temp);
		char * request_line = process_http_header_request(status); 
		reqs[j] = request_line; 
		j++;
		char * response, * content, * content_type, * connection; 
		char * contains = (char *) http_get_header(&temp, "Contains"); 
		char * index = "/index.html";
		size_t content_length;
		struct stat sb;
		char * work = NULL; 
		int work_char_flag = 0;


		printf("This is the request_line: %s\n", request_line);
		if (contains == NULL){connection = "close";}
		else{
			if (strcasecmp(contains, "Keep-Alive") == 0){connection = "Keep-Alive";}
			else{connection = "close";}
		}
		printf("This is the type of connection: %s\n", contains);

		if (request_line == NULL){ 
			free(request_line);
			//printf("\n******************************************FREE - NULL********************************\n");
			response = (char * ) HTTP_501_STRING;
			content = (char * ) HTTP_501_CONTENT;
			response_code = 501;
			content_length = strlen(content);
			content_type = "text/html";
		}
		else {
			if (strcmp(request_line, "/") == 0){
				free(request_line);
				//printf("\n******************************************FREE********************************\n");
				request_line = index;
				work = malloc(1+strlen(request_line));
				//printf("\n******************************************MALLOCD - /********************************\n");
				work_char_flag = 1;
				strcpy(work, request_line);
				//request_line = "/index.html";
			} 
			else{
				work = malloc(1+strlen(request_line));
				//printf("\n******************************************MALLOCD********************************\n");
				work_char_flag = 1;
				strcpy(work, request_line);
				free(request_line);
				//printf("\n******************************************FREE********************************\n");
			}
			   
			char pass [1000] = {0};
			strcpy(pass, "web");
			strcat(pass, work);
			printf("This is the value of pass: %s\n", pass);	
			fp = fopen(pass,"rb");
			printf("This is the memory address of fp: %p\n", fp);
			if (fp == NULL){			
				response = (char * ) HTTP_404_STRING;
				content = (char * ) HTTP_404_CONTENT;
				response_code = 404; 
				content_length = strlen(content);
				content_type = "text/html";
			}
			else {
				response = (char * ) HTTP_200_STRING;
				response_code = 200;
				if (strstr(work, ".html") != NULL){ 
					content_type = "text/html";
				}
				else if (strstr(work, ".css") != NULL){
					content_type = "text/css";
				}
				else if (strstr(work, ".jpg") != NULL){
					content_type = "image/jpeg";
				}
				else if (strstr(work, ".png") != NULL){
					content_type = "image/png";
				}
				else {
					content_type = "text/plain";
				}

				if (stat(pass, &sb) == -1){
					return 0;
				}
				else {
					content_length = sb.st_size;
					printf("This is the length: %d\n", (int)content_length);
				}
			}
      	}
		//char message [content_length];
		printf("value of content_length %d\n", (int) content_length);
		void * message = (void  *) malloc((int)content_length+1);
		printf("problem\n");
		printf("This is what I am fread'ing--\nThis is message: %p\nThis is content_length: %d \nThis is fp: %p\n", message, (int)content_length, fp);
		if (fp != NULL){
			fread(message, 1, content_length, fp);
		}
		else {
			printf("Maybe im an image??\n");
			memcpy(message, content, content_length);
		}

		char final [content_length + 1000];

		size_t test = sprintf(final,"HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: %s\r\n\r\n",response_code, response, content_type , (int)content_length, connection);
		send(client_sock,final,test,0);
		send(client_sock, message, content_length, 0);

		if (fp != NULL){fclose(fp);}

		free(message);
		if (work_char_flag){
			free(work);
			//printf("\n******************************************FREE********************************\n");
		}
		
		//free(request_line);
		http_free(&temp);

		if (strcmp(connection, "Keep-Alive") !=0){
			break;
		}	
	}

	close(client_sock);
	//free(fd);
	//printf("\n******************************************free******************************\n");
	//frees++;
	
	//pthread_exit(0);
	return 0;  
}


int main(int argc, char **argv)
{
	queue_init(&threads);
	typedef void (*signalhandler_t)(int);
	int server_sock, *client_sock; 
	struct addrinfo hints; //*res; 
	int mallocd = 0; 

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [port number]\n", argv[0]);
		return 1;
	}

	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE; 
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_protocol = 0; 

	getaddrinfo(NULL, argv[1], &hints ,&res);
	bind(server_sock, res->ai_addr, res -> ai_addrlen);
	listen(server_sock, 10);

	server_socket_global = server_sock;

	//printf("#finnakill\n");
	signal(SIGINT, handler);
	while(1){
		//client_sock = malloc(sizeof(int));

		
		int temp_var = accept(server_sock, NULL, NULL);
		mallocd =1;
		printf("\n******************************************mallocd %d******************************\n", malls);
		malls++;
		client_sock = malloc(sizeof(int));
		*client_sock =temp_var;
		arr[i] = client_sock;
		i++;
		pthread_t * tids = malloc(sizeof(pthread_t));
		//printf("\n******************************************mallocd******************************\n");
		printf("IM ADDING\n");
		queue_enqueue(&threads, tids);
		pthread_create(tids, NULL, worker_thread, (void *) (client_sock));
		//pthread_detach(tids);
	}
	close(server_sock);
	freeaddrinfo(res);
	//free(argv[0]);
	//free(res);
	//pthread_exit(0);
	return 0;
}
