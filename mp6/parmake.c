/** @file parmake.c */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "parser.h"
#include "queue.h"
#include "rule.h"
#include  <string.h>
#include <semaphore.h>

/**
 * Entry point to parmake.
 */
//********************** GLOBAL VARIABLES ***************************/// 
queue_t jobs;
queue_t ready_queue; 
char ** arr_of_targets; 
int num_of_targets=0, num_of_threads;
pthread_t *tids;
sem_t sem;  

//WHAT TOUCHES MEMORY???
// - remover, remover2, cleaner

/*
WHERE IS MEMORY BEING CREATED 
 - parsed_new_target -> rule, target, two queues
 - main -> arr_of_targets, tids, hash

*/

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

//******************** CALL-BACK FUNCTIONS *********************************************//
void parsed_new_target (char *target1){
    char * newtarget1 = strdup(target1);
    rule_t * curr = (rule_t *) malloc(sizeof(rule_t));
    curr -> target = newtarget1;
    curr->deps = (queue_t *) malloc(sizeof(queue_t));
    curr->commands = (queue_t *) malloc(sizeof(queue_t));

    queue_init(curr->deps);
    queue_init(curr->commands);
    queue_enqueue(&jobs, curr);
    num_of_targets++;
}
void parsed_new_dependency (char *target1, char *dependency){
    char * newdependency = strdup(dependency);
    unsigned int size =queue_size(&jobs);
    int i = 0; 
    rule_t * temp; 

    for (i = 0; i < size; i++){
        temp = queue_at(&jobs, i);
        if (strcmp(temp->target,target1) == 0){
            queue_enqueue(temp->deps, newdependency);
            return;
        }   
    }
}
void parsed_new_command (char *target1, char *command){
    char * newcommand = strdup(command);
    unsigned int size =queue_size(&jobs);
    int i = 0; 
    rule_t * temp; 

    for (i = 0; i < size; i++){
        temp = queue_at(&jobs, i);
        if (strcmp(temp->target,target1) == 0){
            queue_enqueue(temp->commands, newcommand);
            return;
        }   
    }
}
void printer (){
    printf("This is the size %d\n", queue_size(&jobs)); 
    rule_t * shit; 
    int z,x; 
    for (z = 0; z < queue_size(&jobs); z++){
        shit = queue_at(&jobs, z);
        printf("\n");
        printf("Target - %s\n", shit -> target);
        printf("These are the Dependencies \n");
        for (x = 0; x < queue_size(shit -> deps); x++){
            printf("Dep #%d - %s\n", x, (char *)queue_at(shit->deps,x));
        }
        printf("These are the Commands \n");
        for (x = 0; x < queue_size(shit -> commands); x++){
            printf("Command #%d - %s\n", x, (char *)queue_at(shit->commands,x));
        }
    }
    printf("\n");
}
//***************************** HELPER FUNCTIONS *************************************//
void remover(char * rem){
    //pthread_mutex_lock (&lock2);
    int i; 
    rule_t * curr = NULL; 
    for (i = 0; i < queue_size(&jobs); i++){
        curr = queue_at(&jobs, i); 
        if (strcmp(curr->target, rem) == 0){
            //free(curr -> target);
            queue_remove_at(&jobs, i); 
            //pthread_mutex_unlock (&lock2);
            return; 
        }
    }
    
}
void remover2(char * rem){
    //pthread_mutex_lock (&lock2);
    int i; 
    rule_t * curr = NULL; 
    for (i = 0; i < queue_size(&ready_queue); i++){
        curr = queue_at(&ready_queue, i); 
        printf("THIS IS THE VALUE OF curr %s\n", (char *) curr );
        if (strcmp(curr->target, rem) == 0){
            //free(curr -> target);
            queue_remove_at(&ready_queue, i);
            //pthread_mutex_unlock (&lock2); 
            return; 
        }
    }
    
}
void cleaner1 (queue_t * queue, char *gone){ 
    //pthread_mutex_lock (&lock2);
    int i,j,size_queue, size_deps; 
    size_queue = queue_size(queue);
    rule_t * temp; 

    for (i = 0; i < size_queue; i++){
        temp = queue_at(queue, i);
        size_deps = queue_size(temp -> deps);

        for (j = 0; j < size_deps; j++){
            if (strcmp(gone,queue_at(temp->deps,j)) == 0){
                //free(queue_at(temp->deps,j));
                    pthread_mutex_lock (&lock);
                    free(queue_remove_at(temp->deps,j));
                    pthread_mutex_unlock (&lock);
                break;
            }
        }

    }
    //pthread_mutex_unlock (&lock2);
}
int check_num_of_rules(char * target1, queue_t * deps){
    //pthread_mutex_lock (&lock2);
    int counter = 0,i,j;

    for (i = 0; i < queue_size(deps); i++){
        char * t1 = (char *)queue_at(deps,i);
        for (j = 0; j < num_of_targets; j++){
            char * t2 = arr_of_targets[j];
            if (strcmp(t1,t2) == 0){counter++;}
        }
    } 
    //pthread_mutex_unlock (&lock2);
    return counter;
}
rule_t * no_dependencies(){
    //pthread_mutex_lock (&lock2);
    int i; 
    rule_t * curr; 
    for(i = 0; i < queue_size(&jobs); i++){
        curr = queue_at(&jobs, i); 
        if (queue_size(curr -> deps) == 0){
            //pthread_mutex_unlock (&lock2);
            return curr; 
        }
    }
    //pthread_mutex_unlock (&lock2);
    return NULL; 
}
rule_t * find_all_files(){
    //pthread_mutex_lock (&lock2);
    int i, counter = -1; 
    rule_t * curr = NULL; 

    for(i = 0; i < queue_size(&jobs); i++){
        curr = queue_at(&jobs, i);
        counter = check_num_of_rules(curr->target, curr -> deps); 
        if (counter == 0){/*pthread_mutex_unlock (&lock2);*/return curr;}
    }
    //pthread_mutex_unlock (&lock2);
    return NULL; 
}
rule_t * master_finder(){
    
    pthread_mutex_lock (&lock2);

    rule_t * curr; 
    curr = no_dependencies(); 
    if (curr != NULL){
        //printf("found one with 0 deps\n");
        //remove it from the jobs queue
        //pthread_mutex_lock (&lock2);
        remover(curr->target);
        pthread_mutex_unlock (&lock2);
        return curr; 
    }
    curr = find_all_files(); 
    if (curr != NULL){
        //remove it from the jobs queue
        //pthread_mutex_lock (&lock2);
        remover(curr->target);
        pthread_mutex_unlock (&lock2);
        return curr;        
    }
    pthread_mutex_unlock (&lock2);
    return NULL; 
    //pthread_mutex_unlock (&lock2);
}
// queue is the queue of dependencies 
void run_target(queue_t * queue){
    if(queue == NULL){printf("getting some bs\n");return;}
    int j, system_return;
    for(j = 0; j < queue_size(queue); j++){
        system_return = system(queue_at(queue,j));   
        //free(queue_at(queue,j));                
        if (system_return != 0){exit(1);}
    } 
}
// PLAYED AROUND WITH

void master_executer(rule_t * run){
    if(run == NULL){printf("getting some bs\n");}
    rule_t * curr = (rule_t *) run; 

    pthread_mutex_lock (&lock2);
    int dependencies = queue_size(run->deps); 
    pthread_mutex_unlock (&lock2);
    if (dependencies == 0){

        run_target(curr -> commands); 
        pthread_mutex_lock (&lock2);

            cleaner1(&jobs, curr->target);       
            remover2(curr->target);
            //rule_destroy(curr);

        pthread_mutex_unlock (&lock2);
        //free(curr->target);
        //free(curr-> commands);
        
        //free(curr);
        return; 
    }
    pthread_mutex_lock (&lock2);
    int temp = check_num_of_rules(curr->target, curr->deps); 
    pthread_mutex_unlock (&lock2);
    if (temp == 0){

        if (access(curr->target,F_OK) != 0){
            run_target(curr->commands);
        }
        else{
            pthread_mutex_lock (&lock2);
            struct stat buffer,tempbuffer;
            int status,last_mod,c,tempstatus,flag=0;
            char * dep;     

            status = stat(curr->target, &buffer);
            last_mod = buffer.st_mtime; 

            for (c = 0; c < (queue_size(curr->deps)); c++){
                dep = queue_at(curr->deps, c);
                tempstatus = stat(dep,&tempbuffer);
                if (tempbuffer.st_mtime > last_mod){flag = 1;}
            } 
            if (flag==1){
                // CHANGING THIS 
                int system_return = system(curr->target);
                if (system_return != 0){pthread_mutex_unlock (&lock2);exit(1);}
            }
            
            pthread_mutex_unlock (&lock2);
        }
        

        // LOOK HERE

        pthread_mutex_lock (&lock2);
            cleaner1(&jobs, curr->target);
            remover2(curr->target);
            //rule_destroy(curr);
        pthread_mutex_unlock (&lock2);
        
        //free(curr->target);
    }
}
void free_queue(queue_t * q){
        pthread_mutex_lock (&lock2);
    while(queue_size(q) > 0) {
        free(queue_dequeue(q));
    }
        pthread_mutex_unlock (&lock2);
}
void final_freer(rule_t * curr){
    //pthread_mutex_lock (&lock2);
    free_queue(curr -> deps);
    free_queue(curr -> commands);


    free(curr -> target);
    free(curr -> deps);
    free(curr -> commands);
    //rule_destroy(curr);
    free(curr);
    //pthread_mutex_unlock (&lock2);
}

//****************************** LEG WORK ************************************
void * work(){
    rule_t * curr;
    int i,s,flag = 1; 

    pthread_mutex_lock (&lock);
    s = queue_size(&jobs);
    pthread_mutex_unlock (&lock);

    while(s != 0 || flag == 0){

        sem_wait(&sem);   

        pthread_mutex_lock (&lock);
        curr = master_finder();
        //printf("this is what i found %s\n", (char *)curr); 
        if (curr != NULL){queue_enqueue(&ready_queue, curr);}
        //queue_enqueue(&ready_queue, curr);
        pthread_mutex_unlock (&lock); 

        if (curr == NULL){return 0;}

        //should a lock be here?
        
        if (curr != NULL){
            pthread_mutex_lock (&lock);
            rule_t * off = queue_dequeue(&ready_queue);
            if (off == NULL){printf("getting some bs\n");}
        
            pthread_mutex_unlock (&lock);

            master_executer(off);

            pthread_mutex_lock (&lock);
            final_freer(off);
            pthread_mutex_unlock (&lock);
        }

        //if (curr != NULL){}
        //pthread_mutex_unlock (&lock);

        pthread_mutex_lock (&lock);

        int size = queue_size(&jobs); 

        if (size == 0){
            for (i = 0; i < num_of_threads; i++){
                sem_post(&sem);
            }
            flag =0; 
        }

        for(i = 0; i < queue_size(&jobs); i++){
            rule_t * now = queue_at(&jobs, i);
            int all_files = check_num_of_rules(now->target, now->deps);
            int dependencies = queue_size(now->deps); 

            if (all_files == 0 || dependencies == 0){
                sem_post(&sem);   
            }
        }

        pthread_mutex_unlock (&lock);

        pthread_mutex_lock (&lock);
        s = queue_size(&jobs);
        pthread_mutex_unlock (&lock);   
    }   
   return NULL;
}

void testcases_for_rules(){
    printf("This is a test case for the rules\n");
    printf("This is everything first ----\n");
    printer(); 

    printf("\n");

    rule_t * curr = queue_at(&jobs, 2); 

    int back = check_num_of_rules(curr->target, curr->deps); 

    printf("This is the value of back%d\n", back);
}

int main(int argc, char **argv){
    char * makefile = NULL; 
    int opt = 0, counter = 1,i;
//*********************************** PART 1 ******************************************//
    while ((opt = getopt(argc, argv, "f:j:")) != -1) {
        switch (opt) {
            case 'f':
                makefile = optarg; 
                counter+=2; 
                break;
            case 'j':
                num_of_threads = atoi(optarg); 
                counter+=2;
                break;
        }
    }
    if (num_of_threads == 0){num_of_threads = 1;}    

    if (makefile == NULL){
        if (access("makefile", F_OK) == 0){makefile = "makefile";}
        else if (access("Makefile",F_OK) == 0){makefile = "Makefile";}
        else {return -1;}
    }

//************************************* PART 2 ********************************************//
    queue_init(&jobs);
    queue_init(&ready_queue);

    parser_parse_makefile(makefile,argv+optind,parsed_new_target,parsed_new_dependency,parsed_new_command);


    arr_of_targets = (char **) malloc((sizeof(char*))*(num_of_targets));  
    tids = (pthread_t*) malloc (sizeof(pthread_t) * num_of_threads);

    //printer(); 
    for ( i = 0; i < num_of_targets; i++){
        arr_of_targets[i] = strdup(((rule_t *)queue_at(&jobs,i))->target);
    }

    rule_t * t1; 
    int adder = 0; 
    for (i = 0; i < queue_size(&jobs); i++){
        t1 = queue_at(&jobs, i); 
        int t = check_num_of_rules(t1->target, t1->deps);
        if (queue_size(t1->deps) == 0 || t==0 ){
            adder++; 
        }
    }
    //printf("value of adder %d\n", adder);
    sem_init(&sem, 0, adder); 

    //testcases_for_rules();

//************************************** PART 4 *******************************************//
    for (i = 0; i <num_of_threads; i++){
        pthread_create(&tids[i], NULL, work, NULL);
    }

    for(i = 0; i < num_of_threads; i++){
        pthread_join(tids[i],NULL);
    }

    for ( i = 0; i < num_of_targets; i++){
        free(arr_of_targets[i]);
    }    

    free(arr_of_targets);

    free(tids); 


    //free(arr_of_targets);
    queue_destroy(&jobs);
    queue_destroy(&ready_queue);

    return 0;
}









