/** @file msort.c */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Questions: 
1. Would the Data[] cause everything ...
*/

typedef struct _ptest_t
{
	void * start;
	void * end;
	int nums;
} ptest_t;

typedef struct _sender_t
{
	int * arr1;
	int len1;
	int * arr2;
	int len2;
} sender_t;

int compare (const void * a, const void * b){return ( *(int*)a - *(int*)b );}

void *sorter(void *ptr)
{
	ptest_t *curr = (ptest_t *)ptr;
	qsort (curr->start, (size_t)curr->nums,(size_t) sizeof(int),compare);
	printf("Sorted %d elements\n", curr -> nums);
	return 0;
}

void *combine (void *ptr){
	
	sender_t *curr = (sender_t *)ptr;

	int * storage = (int *) malloc((sizeof(int) * (curr ->len1 + curr ->len2)));

	int i = 0;
	int arr1_curr = 0; 
	int arr2_curr = 0;
	int dups = 0;  

	while (i != (curr ->len1+curr ->len2)){
		if(arr1_curr== curr->len1){
			while (arr2_curr<curr ->len2){
				storage[i] = curr ->arr2[arr2_curr];
				i++;
				arr2_curr++;
			}
			break;
		}
		else if(arr2_curr==curr ->len2){
			while (arr1_curr<curr ->len1){
				storage[i] = curr ->arr1[arr1_curr];
				i++;
				arr1_curr++;
			}
			break;
		}

		if(curr->arr1[arr1_curr] == curr->arr2[arr2_curr]){
			dups++;
		}
			
		if (curr ->arr1[arr1_curr] < curr ->arr2[arr2_curr]){
			storage[i] = curr ->arr1[arr1_curr];
			arr1_curr++;
			i++;
		}
		else if (curr ->arr1[arr1_curr] >= curr ->arr2[arr2_curr]){
			storage[i] = curr ->arr2[arr2_curr];
			arr2_curr++;
			i++;
		}
	}

	for (i = 0; i < curr ->len1; i++){
		curr ->arr1[i] = storage[i];
	}
	for (i = 0; i < curr ->len2; i++){
		curr ->arr2[i] = storage[i+curr ->len1];
	}

	free(storage);

	printf("Merged %d and %d elements with %d duplicates.\n", curr ->len1, curr ->len2,dups);

	return 0;
}

int main(int argc, char **argv)
{
/*******************************INIT VARIABLES***********************************************/	
	int wtf = argc;
	int j =0, i =-1;
	wtf = j + i;
	int size = 0; 
	int * data = (int *) malloc((sizeof(int) * 16));	
	int threads;
	int unit;
	int num_per;
	int num_per_last;
	int pos;
	int cap = 16;
	int ** positions; 
	int * lengths;
	threads = atoi(argv[1]); /* The file we are reading from*/
	ptest_t *tests;
	pthread_t *tids;
	tests = (ptest_t *)malloc(sizeof(ptest_t) * threads); 
	tids = (pthread_t*) malloc (sizeof(pthread_t) * threads);
	positions = (int **) malloc(sizeof(int *) * threads);
	lengths = (int *) malloc(sizeof(int) * threads);

/********************CREATE AN ARRAY TO STORE EVERYTHING**********************************/
	while(!feof(stdin)) {	
		++i;
		if (cap == i) {
			data = (int*) realloc (data, (cap*2) * sizeof(int));
			//i = i *2; 
			cap = cap *2;
		}
		fscanf(stdin, "%i", &j);
		if(i >= 0){ 
			data[i] = j;
		}
		size++;
	}
	size--;
	if (size % threads == 0){
		num_per = size/threads;
		num_per_last = size/threads;
		unit = size/threads;
	}
	else {
		num_per = (size/threads) + 1; 
		num_per_last = size -(((size/threads)+1) * (threads-1));
		unit = (size/threads) +1;
	}

/***********************CREATING THREADS********************************************************/
	pos = 0;
	for (i = 0; i < threads; i++){
		tests[i].start = &data[pos];

		positions[i] = (int *) &data[pos];

		if ( (((i + 1) == threads) && !(size % threads == 0)) || threads == 1 ) {
			tests[i].end = &data[size];
			tests[i].nums = num_per_last;
			lengths[i] = num_per_last;
		}
		else{
			tests[i].end = &data[pos+num_per-1];
			tests[i].nums = num_per;
			lengths[i] = num_per;
		}
		pos += unit;
		pthread_create(&tids[i], NULL, sorter, (void*) &tests[i]);
	}

/*********************JOINING THE THREADS********************************************************/
	for(i = 0; i < threads; i++){
		pthread_join(tids[i],NULL);
	}

/********************START COMBINING STUFF*******************************************************/
	int segments = threads;
	sender_t * thrds;
	pthread_t *tids2;

	while (segments != 1) {
		thrds = (sender_t*) malloc(sizeof(sender_t) * segments/2);
		tids2 = (pthread_t*) malloc (sizeof(pthread_t) * segments/2);
			
		for (i = 0; i < segments/2; i++){
			thrds[i].arr1 = positions[(2*i)];
	
			thrds[i].len1 = lengths[(2*i)];

			if ((2*i)+1 < segments){
				thrds[i].arr2 = positions[(2*i)+1];
				thrds[i].len2 = lengths[(2*i)+1];
				pthread_create(&tids2[i], NULL, combine, (void *) &thrds[i]);
			}

		}

		for(i = 0; i < segments/2; i++){
			pthread_join(tids2[i],NULL);
		}

		if (segments % 2 ==1){

			for (i = 0; i <= segments/2; i++){
				positions[i] = positions[(2*i)];

				if ((i*2)+1 == segments){
					lengths[i] = lengths[(2*i)];
				}
				else{
					lengths[i] = lengths[(2*i)] + lengths[(2*i)+1];
				}
			}
		}
		else {
			for (i = 0; i < segments/2; i++){
				positions[i] = positions[(2*i)];
				if ((i*2)+1 == segments){
					lengths[i] = lengths[(2*i)];
				}
				else{
					lengths[i] = lengths[(2*i)] + lengths[(2*i)+1];
				}
			}
		}	
		if (segments % 2 == 1){segments = (segments/2)+1;}
		else{segments = segments/2;}
		free(thrds);
		free(tids2);
	}

	for (i = 0; i < size; i++){
		printf("%d\n", data[i]);
	}

/****************************FREE STUFF*******************************************************/
	free(data);
	free(tests);
	free(tids);
	free(positions);
	free(lengths);	

	//printf("This is the number of threads we want to create:%d \n This is the size of example1.txt %d \n", threads,size); 
 
	return 0;
}