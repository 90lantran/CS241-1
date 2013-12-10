/** @file shell.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.h"

/**
 * Starting point for shell.
 */

/*
Questions. 

 - New line for No Match
 - Not working on testlog
 - different implementation for fork()
 - test cases 
 - error on 145 of 


*/
int main()
{
	pid_t pid;
	char * cwd = malloc (2000);
	size_t size = 256;  
	log_t log;
	log_init(&log);
	char * input;

	while (1){
		pid = getpid(); 
		getcwd(cwd, 2000);
		printf("(pid=%d)%s$ ", pid, cwd);
		input = NULL;
		getline(&input, &size, stdin); 
		input[strlen(input)-1] = '\0';
		char * t; 


		if (strcmp(input,"exit") == 0){
			//free(cwd);
			log_destroy(&log);
			//free(input);
			//free(log);
			free(input);
			free(cwd);
			return 0;
			//free(t);
			//return 0;
		}
		else if (strcmp(input,"") == 0) {
			free(input);
		}
		else if (strncmp(input,"cd ",3) ==0) {
			t = malloc(size);
			strncpy(t,input,size);
			int ret = chdir(input + 3);
			if (ret != 0){
				printf("%s: No such file or directory\n", input + 3);
			}
			//else {
			log_append(&log, t);
			free(input);
			//}
		}
		else if (strncmp(input,"!#", 2) == 0){
			int i = 0;
			for (i = 0; i < (int)log_size(&log); i++){
				printf("%s\n",log_at(&log,i));
			}
			free(input);
		}
		else if (strncmp(input,"!",1) == 0){
			char * query = &input[1]; //getting the query portion
			char * temp = log_search(&log, query);
			//log_append(&log, temp);
			if (temp == NULL){printf("No Match\n");free(input);
			}
			else if (strncmp(temp,"cd ",3) ==0) {
					char * t = malloc(size);
					strncpy(t,temp,size);
					char *path = temp + 3;
					int ret = chdir(path);
					if (ret != 0){
						printf("%s: No such file or directory\n", path);
					}
					else {
						log_append(&log, t);
					}
					printf("%s matches %s\n", query, temp);
					free(input);
			}
			else {
					char * t = malloc(size);
					strncpy(t,temp,size);
					log_append(&log, t);
					//free(input);
					pid = fork(); 

					if (pid == 0){
						char *arr[1000];

						char* temp_str = strtok(temp, " ");
						int i=0;
						while(temp_str!=NULL){
							arr[i] = temp_str;
							i++;
							temp_str = strtok(NULL, " ");
						}
						arr[i] = NULL;

						execvp(arr[0],arr);
						//free(input);
						printf("%s: not found\n",arr[0]);

					}
					else if (pid > 0){
						int temp; 
						wait(&temp);
					}
					else{
					}
					printf("%s matches %s\n", query, t);
					free(input);
		}					
	} 
	else {
			log_append(&log, input);
			pid = fork(); 
			if (pid == 0){
				char *arr[1000];
				//int count =0; 
				char* temp_str = strtok(input, " ");
				int i=0;
				while(temp_str!=NULL){
					arr[i] = temp_str;
					i++;
					temp_str = strtok(NULL, " ");
				}
				arr[i] = NULL;
				execvp(arr[0],arr);
				printf("%s: not found\n",arr[0]);

			}
			else if (pid > 0){
				int temp; 
				wait(&temp);
			}
			else{
			}
		}			
		printf("Command executed by pid=%d\n", pid); 	
	}
	free(cwd);
	free(input);
    return 0;
}
